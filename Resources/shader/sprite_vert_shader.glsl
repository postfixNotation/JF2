#version 330 core
layout (location = 0) in vec4 in_vert_pos_tex;
out vec2 to_frag_tex;

uniform vec2 offset[10];
uniform mat4 model;
uniform mat4 projection;

void main() {
	vec4 model_times_pos = model * vec4(in_vert_pos_tex.xy, 0.0f, 1.0f);
	model_times_pos.xy += offset[gl_InstanceID];
	to_frag_tex = in_vert_pos_tex.zw;
	gl_Position = projection * model_times_pos;
}
