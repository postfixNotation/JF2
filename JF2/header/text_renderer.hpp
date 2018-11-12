#ifndef TEXT_HPP_
#define TEXT_HPP_

#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <renderer.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
	GLuint texture_id;
	glm::ivec2 character_size;
	glm::ivec2 bearing;
	GLuint advance;
};

class TextRenderer {
private:
	std::string filename_;
	size_t default_pixel_size_;
	std::vector<GLuint> indices_;
	std::map<GLchar, Character> characters_;
	static constexpr GLuint kVerticesPerQuad{ 4 };
	static constexpr GLuint kPositionAndTexture{ 4 };

	glm::mat4 projection_;

	std::shared_ptr<Shader> shader_;
	std::shared_ptr<VertexArray> va_;
	std::shared_ptr<IndexBuffer> ib_;
	std::shared_ptr<VertexBuffer> vb_;

	void LoadFonts();
	void InitBuffers();

public:
	TextRenderer(std::shared_ptr<Shader>, size_t width, size_t height);
	~TextRenderer();
	// pixel size of 112 is maximum for many Google fonts
	void SetFileName(std::string filename, size_t pixel_size = 48);
	void Draw(
		std::string text,
		GLfloat x,
		GLfloat y,
		GLfloat scale = 1.0f,
		glm::vec3 color = glm::vec3{ 1.0f },
		std::shared_ptr<Shader> shader = std::shared_ptr<Shader>{ nullptr }
	);

};
#endif // TEXT_HPP_