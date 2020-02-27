#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangs;

layout (location = 4) in vec3 bPos;
layout (location = 5) in vec3 bNormal;
layout (location = 6) in vec3 bTangs;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D heightMap;

uniform float dt;
uniform bool shouldPlay;

out vec3 FragPos;
out vec2 TexCoords;
out mat3 TBN;

void main()
{
    vec3 apos =      aPos;
    vec3 anormal =   aNormal;
    vec3 atangs =    aTangs;

    vec3 bpos =      bPos;
    vec3 bnormal =   bNormal;
    vec3 btangs =    bTangs;

    if(shouldPlay){
        apos = mix(apos,bpos,dt);
        anormal = mix(anormal,bnormal,dt);
        atangs = mix(atangs,btangs,dt);
    }

    TexCoords = aTexCoords;

    //https://learnopengl.com/Advanced-Lighting/Normal-Mapping
    vec3 T = normalize(vec3(model* vec4(atangs,0.0)));
    vec3 N = normalize(vec3(model* vec4(anormal,0.0)));
    vec3 B = normalize(cross(T,N));
    TBN = mat3(T,B,N);

    gl_Position = projection * view * model * vec4(apos, 1.0);
    FragPos = vec3(model * vec4(apos, 1.0));
} 