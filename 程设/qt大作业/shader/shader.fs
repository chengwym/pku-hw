#version 450 core

in vec2 f_tex_coords;

out vec4 out_color;

uniform sampler2D texture_1;
uniform vec2 tiling;
uniform vec4 color;

void main() {
    out_color = texture(texture_1, f_tex_coords * tiling) * color;
}
