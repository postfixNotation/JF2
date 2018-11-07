#version 330 core

in vec2 out_tex_coord;
out vec4 frag_color;

uniform sampler2D u_tex_sampler;

void main () {
	frag_color = texture(u_tex_sampler, out_tex_coord);
}
