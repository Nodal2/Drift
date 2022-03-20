#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoords;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_textureCoords;

uniform mat4 u_world;
uniform mat4 u_view;
uniform mat4 u_projection;


void main()
{
    gl_Position = u_projection * u_view * u_world * vec4(position.xyz, 1.0);
    v_position = vec3(u_world * vec4(position, 1.0));
    v_normal = mat3(transpose(inverse(u_world))) * normal;
    v_textureCoords = textureCoords;
}