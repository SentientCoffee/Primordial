#version 420 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
//out vec4 FragColor;

//https://learnopengl.com/PBR/Lighting
//most of this code isn't mine
struct GBuffer {
    sampler2D gPos;
    sampler2D gNormal;
    sampler2D gAlbedo;
    sampler2D gMetalRoughnessAO;
    sampler2D gEmissive;
};  

struct PointLight {    
    vec3 position;
    vec3 colour;

    bool active;
};  

const float PI = 3.14159265359f;
#define MAX_LIGHTS 40
uniform int numLights;
uniform PointLight lights[MAX_LIGHTS];
uniform GBuffer gBuffer;

uniform vec3 camPos;
in vec2 TexCoords;

vec3 fresnelSchlick(float cosTheta,vec3 F0){
    return F0 + (1.0f - F0) * pow(1.0f - cosTheta,5.0f);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}


void main(){

    vec3 albedo = texture(gBuffer.gAlbedo,TexCoords).rgb;
    vec3 norm = texture(gBuffer.gNormal,TexCoords).rgb;
    vec3 metalRoughnessAO = texture(gBuffer.gMetalRoughnessAO,TexCoords).rgb;
    float metallic = metalRoughnessAO.r;
    float roughness = metalRoughnessAO.g;
    float ambientOcc = metalRoughnessAO.b;
    vec3 FragPos = texture(gBuffer.gPos,TexCoords).rgb;
    vec3 V = normalize(camPos - FragPos);

    vec3 F0 = vec3(0.04f);
    F0 = mix(F0,albedo,metallic);

    vec3 Lo = vec3(0.0f);
    for(int i = 0; i < numLights;i++){
        vec3 L = normalize(lights[i].position - FragPos);
        vec3 H = normalize(V + L);

        float distance = length(lights[i].position - FragPos);
        float attenuation = 1.0f/(0.01f + 0.001f*distance + 0.01f*(distance*distance));
        vec3 radiance = lights[i].colour*attenuation;
        
        vec3 F = fresnelSchlick(max(dot(H,V),0.0),F0);
        
        float NDF = DistributionGGX(norm, H, roughness);
        float G   = GeometrySmith(norm, V, L, roughness);

        vec3 numerator = NDF*G*F;
        float denominator = 4.0f*max(dot(norm,V),0.0)*max(dot(norm,L),0.0f) + 0.001;
        vec3 specular = numerator/denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0f) - kS;

        kD *= 1.0f - metallic;

        float NdotL = max(dot(norm,L),0.0f);
        Lo += (kD*albedo/PI + specular) * radiance * NdotL;
    }

    //0.03 should be here, but to see stuff right now we leave it out
    vec3 ambient = vec3(0.6f).rgb*albedo * (ambientOcc != 0 ? ambientOcc:1);
    vec3 color   = ambient + Lo;
    color = vec3(1.0) - exp(-color*1.0f);//1 is exposure
    color += 4.0f*texture(gBuffer.gEmissive,TexCoords).rgb;
    FragColor = vec4(color,1.0f);

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0f)//bloom threshold
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

}