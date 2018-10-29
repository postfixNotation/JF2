#version 330 core

layout (location = 0) in vec4 vert_pos;
out vec2 tex_coords;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(vert_pos.xy, 0.0f, 1.0f);
	tex_coords = vert_pos.zw;
}

