#version 450 core

in vec3 fragColour;
in vec2 fragTexCoord;

out vec4 color;

uniform float deltaTime;
uniform sampler2D tex;

void main(void)
{
	color = vec4(fragColour, 1.0);
}