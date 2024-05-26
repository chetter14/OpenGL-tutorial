#version 330 core
out vec4 FragmentColor;

in vec3 ourColor;
in vec2 ourTexCoord;

uniform sampler2D ourTexture;

void main()
{
	FragmentColor = texture(ourTexture, ourTexCoord) * vec4(ourColor, 1.0);
}