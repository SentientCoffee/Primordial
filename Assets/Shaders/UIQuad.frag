#version 420 core
out vec4 FragColour;

in vec2 TexCoords;

uniform sampler2D quadTexture1;
uniform sampler2D quadTexture2;
uniform sampler2D quadTexture3;
uniform sampler2D quadTexture4;

void main(){

    vec4 tex1 = texture(quadTexture1,TexCoords);
    vec4 tex2 = texture(quadTexture2,TexCoords);
    vec4 tex3 = texture(quadTexture3,TexCoords);
    vec4 tex4 = texture(quadTexture4,TexCoords);

    

    FragColour = tex1 + tex2;

}