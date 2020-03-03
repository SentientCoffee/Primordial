#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangs;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 PlayerPosition;

uniform sampler2D heightMap;

uniform float u;

out vec3 FragPos;
out vec2 TexCoords;
out mat3 TBN;
out vec3 playerPos;

uniform float posVarience;

void main()
{
    vec3 pos = aPos;
    pos.y += posVarience;

    FragPos = vec3(model * vec4(pos, 1.0));
    FragPos -= PlayerPosition;
    FragPos*=-1.0f;
    FragPos = vec3(vec4(FragPos,1.0f)*view).xyz;
   
    gl_Position = projection * model * vec4(pos, 1.0);
    
    TexCoords = aTexCoords;

//https://learnopengl.com/Advanced-Lighting/Normal-Mapping
    vec3 T = normalize(vec3(model* vec4(aTangs,0.0)));
    vec3 N = normalize(vec3(model* vec4(aNormal,0.0)));

    vec3 B = normalize(cross(T,N));
    TBN = mat3(T,B,N);

    playerPos = PlayerPosition;

} 