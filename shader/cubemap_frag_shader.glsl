#version 330 core

in vec3 tex_pos_frag;
out vec4 frag_color;

uniform samplerCube skybox;

void main() {
	vec3 tex_pos = vec3(tex_pos_frag.x, -tex_pos_frag.y, tex_pos_frag.z);
	frag_color = texture(skybox, tex_pos);
}

