#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 u_world;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_world * vec4(position.xyz, 1.0f);
}