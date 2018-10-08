#ifndef TEXT_HPP_
#define TEXT_HPP_

#include <map>
#include <iostream>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
	GLuint TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	GLuint Advance;
};

class Text {
private:
	// Text requires Shader class instance
	std::string filename_;
	std::map<GLchar, Character> characters_;
	static constexpr GLuint kVerticesPerQuad{ 6 };
	static constexpr GLuint kPositionAndTexture{ 4 };
	GLuint shader_handle_; // change to Shader class instance
	GLuint vao_, vbo_; // add ibo
	glm::mat4 projection_;

	void LoadFonts();
	void InitBuffers();
	void UseProjection() const;

public:
	Text();
	Text(GLuint);
	~Text();
	void SetShaderHandle(GLuint);
	void SetFileName(std::string);
	void SetProjectionMatrix(glm::mat4);
	void RenderText(
		GLuint,
		std::string,
		GLfloat,
		GLfloat,
		GLfloat,
		glm::vec3
	);
	void RenderText(
		std::string,
		GLfloat,
		GLfloat,
		GLfloat,
		glm::vec3
	);
};
#endif // TEXT_HPP_
