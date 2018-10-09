#version 330 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 normal_pos;
layout (location = 2) in vec2 tex_pos;

out vec3 tex_pos_frag;

uniform mat4 view;
uniform mat4 projection;

void main() {
	tex_pos_frag = vert_pos;
	vec4 temp_pos = projection * view * vec4(vert_pos, 1.0);
	gl_Position = temp_pos.xyww;
}

