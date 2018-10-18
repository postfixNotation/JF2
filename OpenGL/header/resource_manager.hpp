#ifndef RESOURCE_MANAGER_HPP_
#define RESOURCE_MANAGER_HPP_

#include <map>
#include <string>
#include <utility>

#include <GL/glew.h>

#include <shader.hpp>
#include <texture2d.hpp>

struct ResourceManager {
	ResourceManager() = delete;

    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture2D> textures;

    static Shader LoadShader(
		const std::string& vert_file,
		const std::string& frag_file,
		std::string name
	);
    static const Texture2D& LoadTexture(
		const std::shared_ptr<Shader>& shader,
		const std::string& file_name,
		std::string name,
		bool gen_mipmaps = true
	);

    static const Shader& GetShader(const std::string& name);
    static const Texture2D& GetTexture(const std::string& name);
};

#endif // RESOURCE_MANAGER_HPP_
