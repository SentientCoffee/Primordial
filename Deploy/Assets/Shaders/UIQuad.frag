#version 420 core

out vec4 FragColour;

in vec2 TexCoords;

uniform sampler2D quadTexture1;
uniform sampler2D quadTexture2;
uniform float alpha = 1.0;

void main() {
    vec4 tex1 = texture(quadTexture1,TexCoords);
    vec4 tex2 = texture(quadTexture2,TexCoords);

    vec4 c = tex1 + tex2;

    FragColour = vec4(c.rgb, c.a * alpha);
}