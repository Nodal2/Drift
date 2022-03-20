#version 330 core

out vec4 f_color;

uniform vec3 u_lightColor;

void main()
{
    f_color = vec4(u_lightColor, 1.0);
}