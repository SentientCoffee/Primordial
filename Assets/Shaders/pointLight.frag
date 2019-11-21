#version 420 core
out vec4 FragColor;
  
  
 struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normalMap;
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
};  

uniform PointLight pointLight;
uniform Material material;
uniform vec3 viewPos;
in vec3 Normal; 
in vec3 FragPos; 
in vec2 TexCoords;


//functions
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir); 

void main()
{

vec3 result = vec3(1.0);
vec3 norm = texture(material.normalMap,TexCoords).rbg;
norm = normalize(norm*2.0-1.0);
vec3 viewDir = normalize(viewPos - FragPos);

result = calculatePointLight(pointLight,norm,FragPos,viewDir);

FragColor = vec4(result, 1.0);

}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos,vec3 viewDir) {

   vec3 lightDir = normalize(light.position - fragPos);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (1.0f + (distance * distance));

    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + attenuation * (diffuse + specular));
}