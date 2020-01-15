#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 projection;

uniform vec3 PlayerPosition;

uniform sampler2D heightMap;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec3 TestViewDir;
void main()
{
    vec3 pos = aPos;
    //pos.y += texture(heightMap,aTexCoords).r;
    mat4 temp = model;

    vec4 temp4 = temp*(vec4(pos,1.0));
    TestViewDir = -temp4.xyz;

    gl_Position = projection * model * vec4(pos, 1.0);
    FragPos = vec3(model * vec4(pos, 1.0));
    FragPos -= PlayerPosition;
    FragPos*=-1.0f;
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
} 