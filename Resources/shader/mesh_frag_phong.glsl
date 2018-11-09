#version 330 core

in vec3 out_frag_pos;
in vec3 out_normal;
in vec2 out_tex_coord;
out vec4 frag_color;

uniform sampler2D u_tex_sampler;
uniform vec3 u_light_pos;		// diffuse
uniform vec3 u_light_color;		// diffuse
uniform vec3 u_view_pos;		// specular

const float kAmbientFactor = 0.2f;
const float kSpecularFactor = 0.8f;
const float kShininess = 32.0f;

void main() {
	// Ambient
	vec3 ambient = u_light_color * kAmbientFactor;

	// Diffuse
	vec3 normal = normalize(out_normal);
	vec3 light_dir = normalize(u_light_pos - out_frag_pos);
	float n_dot_l = max(dot(normal, light_dir), 0.0f);
	vec3 diffuse = u_light_color * n_dot_l;

	// Specular
	vec3 view_dir = normalize(u_view_pos - out_frag_pos);
	// Following lines correspond to *Phong* light shading
	// vec3 reflect_dir = reflect(-light_dir, normal);
	// float r_dot_v = max(dot(reflect_dir, view_dir), 0.0f);
	// vec3 specular = u_light_color * kSpecularFactor * pow(r_dot_v, kShininess);
	// Following lines correspond to *Blinn-Phong* light shading
	vec3 half_dir = normalize(light_dir + view_dir);
	float n_dot_h = max(dot(normal, half_dir), 0.0f);
	vec3 specular = u_light_color * kSpecularFactor * pow(n_dot_h, kShininess);

	vec4 texel = texture(u_tex_sampler, out_tex_coord);
	frag_color = vec4(ambient + diffuse + specular, 1.0f) * texel;
}
