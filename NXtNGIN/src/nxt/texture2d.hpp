#ifndef TEXTURE_2D_HPP_
#define TEXTURE_2D_HPP_

#define MAX_NUMBER_TEX_UNITS 32

#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include <stb-master/stb_image.h>
#include <GL/glew.h>

#include "shader.hpp"

namespace nxt {
	class Texture2D {
	private:
		GLuint handle_{};
		bool is_cube_map_{ false };
		std::shared_ptr<Shader> shader_;
	public:
		Texture2D(std::shared_ptr<Shader> shader) : shader_{ shader } {}
		Texture2D() : Texture2D{ nullptr } {}
		Texture2D(
			std::shared_ptr<Shader> shader,
			const std::vector<std::string>& faces)
			: Texture2D(shader) {
			Load(faces);
		}
		Texture2D(
			std::shared_ptr<Shader> shader,
			const std::string& file_name,
			bool gen_mipmaps = true)
			: Texture2D(shader) {
			Load(file_name, gen_mipmaps);
		}

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		~Texture2D() { glDeleteTextures(1, &handle_); }

		bool Load(const std::string& file_name, bool gen_mipmaps = true);
		bool Load(const std::vector<std::string>& faces);

		void Bind(const GLchar* uniform, GLuint texunit = 0) const;
		void Unbind(GLuint texunit = 0) const;
	};
}

#endif // TEXTURE_2D_HPP_
