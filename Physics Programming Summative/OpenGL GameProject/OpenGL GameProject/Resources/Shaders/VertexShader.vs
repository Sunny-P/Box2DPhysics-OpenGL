#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 fragNormal;
out vec2 fragTexCoord;

uniform float deltaTime;

uniform mat4 PVMatrix;

uniform mat4 modelMat;

void main(void)
{
	gl_Position = PVMatrix * modelMat * vec4(position, 1.0);

	fragNormal = normal;
	fragTexCoord = texCoord;
}