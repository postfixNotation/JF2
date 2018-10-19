#ifndef RESOURCE_MANAGER_HPP_
#define RESOURCE_MANAGER_HPP_

#include <map>
#include <memory>
#include <string>
#include <utility>

#include <GL/glew.h>

#include <shader.hpp>
#include <texture2d.hpp>
#include <text_renderer.hpp>

struct ResourceManager {
	ResourceManager() = delete;

    static std::map<std::string, std::shared_ptr<Shader>> shaders;
    static std::map<std::string, std::shared_ptr<Texture2D>> textures;
    static std::map<std::string, std::shared_ptr<TextRenderer>> text_renderers;

    static const std::shared_ptr<Shader>& LoadShader(
		const std::string& vert_file,
		const std::string& frag_file,
		std::string name
	);
    static const std::shared_ptr<Texture2D>& LoadTexture(
		const std::shared_ptr<Shader>& shader,
		const std::string& file_name,
		std::string name,
		bool gen_mipmaps = true
	);
    static const std::shared_ptr<TextRenderer>& LoadTextRenderer(
		const std::shared_ptr<Shader>& shader,
		size_t width,
		size_t height,
		size_t pixel_size,
		const std::string& file_name,
		std::string name
	);

    static const std::shared_ptr<Shader>& GetShader(const std::string& name);
    static const std::shared_ptr<Texture2D>& GetTexture(const std::string& name);
	static const std::shared_ptr<TextRenderer>& GetTextRenderer(const std::string& name);
};

#endif // RESOURCE_MANAGER_HPP_
