#version 450 core

in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 color;

uniform float deltaTime;
uniform sampler2D tex;

void main(void)
{
	color = texture(tex, fragTexCoord);
}