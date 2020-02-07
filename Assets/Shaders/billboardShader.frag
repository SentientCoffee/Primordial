#version 420 core

uniform sampler2D image;

in vec2 TexCoords;

out vec4 colour;

void main()
{
    vec4 result = texture(image,TexCoords).rgba;
    colour = result;
}