#version 330 core
out vec4 FragmentColor;

in vec3 ourColor;
in vec2 ourTexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float coef;

void main()
{
	
	FragmentColor = mix(texture(texture1, ourTexCoord), texture(texture2, ourTexCoord), coef);
}