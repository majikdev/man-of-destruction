#version 330 core

// Inputs and outputs.

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_coords;
out vec2 var_coords;

// Global uniforms.

uniform mat4 projection;
uniform vec3 position;
uniform vec3 scale;
uniform vec4 coords;

void main()
{
    gl_Position = vec4(in_pos * scale + position, 1.0f) * projection;
    var_coords = in_coords * coords.zw + coords.xy;
}