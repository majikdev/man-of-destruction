#version 330 core

// Inputs and outputs.

in vec2 var_coords;
out vec4 out_colour;

// Texture sampler.

uniform sampler2D sample;

void main()
{
    out_colour = texture(sample, var_coords);

    // Discard transparent pixels.

    if (out_colour.a < 0.5f)
    {
        discard;
    }
}