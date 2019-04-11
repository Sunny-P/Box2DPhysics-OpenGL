#version 450 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 color;

uniform float deltaTime;
uniform sampler2D tex;

uniform vec3 ambient;
uniform vec3 lightColour;
uniform vec3 lightPos;

uniform vec3 camPos;
uniform float lightSpecStr;
uniform float shininess;

void main(void)
{
	// Light Direction
	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(fragPos - lightPos);

	// Diffuse colouring
	float diffuseStr = max(dot(norm, -lightDir), 0.0f);
	vec3 diffuse = diffuseStr * lightColour;

	// Specular highlight
	vec3 negViewDir = normalize(camPos - fragPos);
	//vec3 reflectDir = reflect(lightDir, norm);
	//float spec = pow(max(dot(negViewDir, reflectDir), 0.0f), shininess);
	vec3 halfwayVec = normalize(-lightDir + negViewDir);
	float spec = pow(max(dot(norm, halfwayVec), 0.0f), shininess);
	vec3 specular = lightSpecStr * spec * lightColour;

	color = vec4(ambient + diffuse + specular, 1.0f) * texture(tex, fragTexCoord);
}