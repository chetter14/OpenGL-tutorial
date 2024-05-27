#version 330 core
out vec4 FragmentColor;

in vec3 ourColor;
in vec2 ourTexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	
	FragmentColor = mix(texture(texture1, ourTexCoord), texture(texture2, vec2(1 - ourTexCoord.x, ourTexCoord.y)), 0.2);
}