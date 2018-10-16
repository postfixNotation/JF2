#version 330 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 normal_pos;
layout (location = 2) in vec2 tex_pos;

out vec2 tex_pos_frag;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main () {
	gl_Position = projection * view * model * vec4(vert_pos.xyz, 1.0f);
	tex_pos_frag = tex_pos;
}
