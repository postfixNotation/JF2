#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

uniform float xoffset[3];
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 out_frag_pos;
out vec3 out_normal;
out vec2 out_tex_coord;

void main() {
	vec4 temp_position = vec4(position + vec3(xoffset[gl_InstanceID], 0.0f, 0.0f), 1.0f);
	out_frag_pos = vec3(u_model * temp_position);
	out_normal = mat3(transpose(inverse(u_model))) * normal;
	out_tex_coord = tex_coord;

	gl_Position = u_projection * u_view *  u_model * temp_position;
}
