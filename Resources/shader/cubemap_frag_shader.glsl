#version 330 core

in vec3 tex_pos_frag;
out vec4 frag_color;

uniform samplerCube skybox;

void main() {
	frag_color = texture(skybox, tex_pos_frag);
}
