#version 420 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
  
  
 struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normalMap;
    sampler2D emissionMap;
    float     shininess;
};  
  
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    bool UI;
    bool active;
};  

uniform int numLights;
#define MAX_LIGHTS 50
uniform PointLight pointLight[MAX_LIGHTS];
uniform Material material;
in vec3 FragPos; 
in vec2 TexCoords;
in vec3 TestViewDir;
in mat3 TBN;


//functions
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  

void main()
{

vec3 result = vec3(0.0);
vec3 norm = texture(material.normalMap,TexCoords).rgb;
norm = normalize(norm*2.0-1.0);
norm = normalize(TBN*norm);

vec3 viewDir = normalize(TestViewDir);


    for(int i =0 ; i < numLights;i++){
       // if(dot(viewDir,norm) <= 0.1f)
       // result += toon(pointLight[i],norm,FragPos,viewDir);
       // else
       if(pointLight[i].active)
            result += calculatePointLight(pointLight[i],norm,FragPos,viewDir);
    }

vec3 emission = texture(material.emissionMap,TexCoords).rgb;
emission*= 1.0f;
result += emission;

FragColor = vec4(result, 1.0);

 float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0f)//bloom threshold
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos,vec3 viewDir) {

  	vec3 lightDir = normalize(light.position - fragPos);
	  float x = lightDir.x;
	  float z = lightDir.z;
    if(light.UI){
		vec3 frontVec = vec3(0.0f,0.0f,-1.0f);
		float angel = dot(lightDir,frontVec);
		angel -= 3.14/2.0f;
		angel -= (3*3.14f)/2.0f;
		x = -cos(angel);
		z = -sin(angel);
		

	}
	lightDir = vec3(x,lightDir.y,z);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    
    // attenuation
   float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    


    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    ambient*=attenuation;
    diffuse*=attenuation;
    specular*=attenuation;

    return (diffuse + specular);
}

vec3 toon(PointLight light, vec3 normal, vec3 fragPos,vec3 viewDir) {

   vec3 lightDir = normalize(light.position - fragPos);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    
    // attenuation
   float distance = length(light.position - fragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    


    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

   // ambient*=attenuation;
    diffuse*=attenuation;
    specular*=attenuation;

    return (ambient + diffuse + specular);
}