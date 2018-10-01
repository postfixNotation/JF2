#version 450 core

in vec2 tex_pos_frag;
out vec4 frag_color;
layout (binding = 0) uniform sampler2D tex_sampler;

void main () {
  frag_color = texture(tex_sampler, tex_pos_frag);
}
