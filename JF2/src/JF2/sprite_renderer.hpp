#ifndef SPRITE_RENDERER_HPP_
#define SPRITE_RENDERER_HPP_

#include <vector>
#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture2d.hpp"
#include "renderer.hpp"

class SpriteRenderer {
private:
	std::vector<GLuint> indices_;
	glm::fmat4 projection_;

	std::shared_ptr<Shader> shader_;
	std::shared_ptr<IndexBuffer> ib_;
	std::shared_ptr<VertexArray> va_;

	void InitRenderData();
public:
	SpriteRenderer(
		const std::shared_ptr<Shader> &shader,
		const GLfloat &width,
		const GLfloat &height
	);
	~SpriteRenderer();
	void Draw(
		const std::shared_ptr<Texture2D> &texture,
		const glm::fvec2 &position,
		glm::fvec2 size = glm::fvec2{ 10.0f, 10.0f },
		GLfloat rotate = 0.0f,
		glm::fvec3 color = glm::fvec3{ 1.0f });
	void Draw(
		const std::shared_ptr<Texture2D> &texture,
		const glm::fvec2 &position,
		const std::vector<glm::fvec2> &offsets,
		glm::fvec2 size = glm::fvec2{ 10.0f, 10.0f },
		GLfloat rotate = 0.0f,
		glm::fvec3 color = glm::fvec3{ 1.0f });
};

#endif // SPRITE_RENDERER_HPP_
