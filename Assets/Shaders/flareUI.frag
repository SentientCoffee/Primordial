#version 420 core

in vec2 TexCoords;

out vec4 colour;

uniform sampler2D image;
uniform float alpha;

void main() {
	vec4 t = texture(image, TexCoords);
	colour = vec4(t.rgb, t.a * alpha);
}