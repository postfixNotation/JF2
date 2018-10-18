#include <resource_manager.hpp>

std::map<std::string, Shader> shaders;
std::map<std::string, Texture2D> textures;

Shader ResourceManager::LoadShader(
	const std::string& vert_file,
	const std::string& frag_file,
	std::string name
) {
	return (shaders[name] = std::move(Shader{ vert_file, frag_file }));
}

const Texture2D& ResourceManager::LoadTexture(
	const std::shared_ptr<Shader>& shader,
	const std::string& file_name,
	std::string name,
	bool gen_mipmaps
) {
	return (textures[name] = std::move(Texture2D{ shader, file_name, gen_mipmaps }));
}

const Shader& ResourceManager::GetShader(const std::string& name) {
	if (shaders.find(name) == shaders.end()) assert(false);
	return shaders[name];
}

const Texture2D& ResourceManager::GetTexture(const std::string& name) {
	if (textures.find(name) == textures.end()) assert(false);
	return textures[name];
}
