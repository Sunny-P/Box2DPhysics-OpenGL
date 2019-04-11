#version 450 core

in vec3 fragNormal;
in vec2 fragTexCoord;
in vec4 worldPos;

out vec4 color;

uniform float deltaTime;
uniform sampler2D tex;
uniform vec3 camPos;

void main(void)
{
    float d = distance(worldPos.xyz, camPos); 
    float lerp = (d - 1.0f)/10.f; 
    lerp = clamp(lerp, 0.0, 1.0);

    vec4 fogColor = vec4(0.6f, 0.6f, 0.6f, 1.0f);

    vec4 textureColour = texture(tex, fragTexCoord);

    color = mix(textureColour, fogColor, lerp);
}