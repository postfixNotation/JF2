#ifndef TEXT_HPP_
#define TEXT_HPP_

#include <map>
#include <vector>
#include <iostream>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
	GLuint texture_id;
	glm::ivec2 character_size;
	glm::ivec2 bearing;
	GLuint advance;
};

class Text {
private:
	// Text requires Transform(ation) class instance
	Shader *shader_;
	std::string filename_;
	size_t default_pixel_size_;
	std::map<GLchar, Character> characters_;
	static constexpr GLuint kVerticesPerQuad{ 6 };
	static constexpr GLuint kPositionAndTexture{ 4 };
	GLuint vao_, vbo_, ibo_;
	glm::mat4 projection_; // change to transformation instance
	std::vector<GLuint> indices_; // efficient memory use with IBOs

	void LoadFonts();
	void InitBuffers();
	void UseProjection() const;

public:
	Text(Shader*, size_t, size_t);
	~Text();
	// pixel size of 112 is maximum for many Google fonts
	void SetFileName(std::string filename, size_t pixel_size = 48);
	void RenderText(
		std::string text,
		GLfloat x,
		GLfloat y,
		GLfloat scale = 1.0f,
		glm::vec3 color = glm::vec3{ 1.0f }
	);

};
#endif // TEXT_HPP_

