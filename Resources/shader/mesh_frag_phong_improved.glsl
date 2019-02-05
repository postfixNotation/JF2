#version 330 core

struct Material
{
	vec3 ambient;
	sampler2D diffuseMap;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 out_frag_pos;
in vec3 out_normal;
in vec2 out_tex_coord;

out vec4 frag_color;

uniform Material u_material;
uniform Light u_light;
uniform vec3 u_view_pos;

void main() {
	// Ambient
	vec3 ambient = u_light.ambient * u_material.ambient;

	// Diffuse
	vec3 normal = normalize(out_normal);
	vec3 light_dir = normalize(u_light.position - out_frag_pos);
	float n_dot_l = max(dot(normal, light_dir), 0.0f);
	vec3 diffuse = u_light.diffuse * n_dot_l * vec3(texture(u_material.diffuseMap, out_tex_coord));

	// Specular
	vec3 view_dir = normalize(u_view_pos - out_frag_pos);
	vec3 half_dir = normalize(light_dir + view_dir);
	float n_dot_h = max(dot(normal, half_dir), 0.0f);
	vec3 specular = u_light.specular * u_material.specular * pow(n_dot_h, u_material.shininess);

	frag_color = vec4(ambient + diffuse + specular, 1.0f);
}
