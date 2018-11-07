#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 out_frag_pos;
out vec3 out_normal;
out vec2 out_tex_coord;

void main() {
	out_frag_pos = vec3(u_model * vec4(position, 1.0f));
	out_normal = mat3(transpose(inverse(u_model))) * normal;
	out_tex_coord = tex_coord;

	gl_Position = u_projection * u_view *  u_model * vec4(position, 1.0f);
}
