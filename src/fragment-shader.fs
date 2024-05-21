#version 330 core
out vec4 FragmentColor;
in vec3 ourColor;

void main()
{
	FragmentColor = vec4(ourColor, 1.0f);
}