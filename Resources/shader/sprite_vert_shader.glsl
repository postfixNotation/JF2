#version 330 core
layout (location = 0) in vec4 in_vert_pos_tex;
out vec2 to_frag_tex;

uniform mat4 model;
uniform mat4 projection;

void main() {
	to_frag_tex = in_vert_pos_tex.zw;
	gl_Position = projection * model * vec4(in_vert_pos_tex.xy, 0.0f, 1.0f);
}
