#version 420 core

layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
	TexCoords = normalize(aPos);
	vec4 outPosition = projection * view * vec4(aPos, 1.0);
	gl_Position = outPosition.xyww;
}