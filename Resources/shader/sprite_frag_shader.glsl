#version 330 core
in vec2 to_frag_tex;
out vec4 color;

uniform sampler2D image_sampler;
uniform vec3 sprite_color;

void main() {    
    color = vec4(sprite_color, 1.0f) * texture(image_sampler, to_frag_tex);
}
