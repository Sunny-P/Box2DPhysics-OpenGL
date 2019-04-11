#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;

uniform mat4 MVP;
uniform mat4 modelMat;

void main(void)
{
	gl_Position = MVP * vec4(position, 1.0f);
	fragPos = vec3(modelMat * vec4(position, 1.0f));
	fragNormal = mat3(transpose(inverse(modelMat))) * normal;
	fragTexCoords = texCoords;
}