#include "resource_manager.hpp"

std::map<std::string, std::shared_ptr<Shader>> ResourceManager::shaders;
std::map<std::string, std::shared_ptr<Texture2D>> ResourceManager::textures;
std::map<std::string, std::shared_ptr<TextRenderer>> ResourceManager::text_renderers;

const std::shared_ptr<Shader>& ResourceManager::LoadShader(
	const std::string& vert_file,
	const std::string& frag_file,
	std::string name) {

	return (shaders[name] = std::move(std::make_shared<Shader>(vert_file, frag_file)));
}

const std::shared_ptr<Texture2D>& ResourceManager::LoadTexture(
	const std::shared_ptr<Shader>& shader,
	const std::string& file_name,
	std::string name,
	bool gen_mipmaps) {

	return (textures[name] = std::move(std::make_shared<Texture2D>(shader, file_name, gen_mipmaps)));
}

const std::shared_ptr<Texture2D>& ResourceManager::LoadTexture(
	const std::shared_ptr<Shader>& shader,
	const std::vector<std::string>& faces,
	std::string name) {

	return (textures[name] = std::move(std::make_shared<Texture2D>(shader, faces)));
}

const std::shared_ptr<TextRenderer>& ResourceManager::LoadTextRenderer(
	const std::shared_ptr<Shader>& shader,
	size_t width,
	size_t height,
	size_t pixel_size,
	const std::string& file_name,
	std::string name) {

	text_renderers[name] = std::move(std::make_shared<TextRenderer>(shader, width, height));
	text_renderers[name]->SetFileName(file_name, pixel_size);
	return text_renderers[name];
}

const std::shared_ptr<Shader>& ResourceManager::GetShader(const std::string& name) {
	if (shaders.find(name) == shaders.end()) assert(false);
	return shaders[name];
}

const std::shared_ptr<Texture2D>& ResourceManager::GetTexture(const std::string& name) {
	if (textures.find(name) == textures.end()) assert(false);
	return textures[name];
}

const std::shared_ptr<TextRenderer>& ResourceManager::GetTextRenderer(const std::string& name) {
	if (text_renderers.find(name) == text_renderers.end()) assert(false);
	return text_renderers[name];
}
