#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangs;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D heightMap;

out vec3 FragPos;
out vec3 TestViewDir;
out vec2 TexCoords;
out mat3 TBN;
void main()
{
    vec3 pos = aPos;
    TexCoords = aTexCoords;
    //pos.y = texture(heightMap,TexCoords).r;
    mat4 temp = view*model;
    
    vec4 temp4 = temp*(vec4(pos,1.0));
    TestViewDir = -temp4.xyz;

    gl_Position = projection * view * model * vec4(pos, 1.0);
    FragPos = vec3(model * vec4(pos, 1.0));

//https://learnopengl.com/Advanced-Lighting/Normal-Mapping
    vec3 T = normalize(vec3(model* vec4(aTangs,0.0)));
    vec3 N = normalize(vec3(model* vec4(aNormal,0.0)));

    vec3 B = normalize(cross(T,N));
    TBN = mat3(T,B,N);

} 