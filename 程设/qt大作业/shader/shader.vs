#version 450 core

layout (location = 0) in vec2 v_pos;
layout (location = 1) in vec2 v_tex_coords;

out vec2 f_tex_coords;

uniform mat4 projection;
uniform mat4 model;

void main() {
    gl_Position = projection * model * vec4(v_pos, 0.0f, 1.0f);
    f_tex_coords = v_tex_coords;
}
