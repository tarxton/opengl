#version 330 core

out vec4 fragColor;
in vec3 ourColor, posCol;

void main()
{
    fragColor = vec4(posCol, 1.0f);
}