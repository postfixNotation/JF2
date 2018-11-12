#ifndef SPRITE_RENDERER_HPP_
#define SPRITE_RENDERER_HPP_

#include <vector>
#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <texture2d.hpp>
#include <renderer.hpp>

class SpriteRenderer {
private:
	std::vector<GLuint> indices_;
	glm::mat4 projection_;

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
		const glm::vec2 &position,
		glm::vec2 size = glm::vec2{10, 10},
		GLfloat rotate = 0.0f,
		glm::vec3 color = glm::vec3{1.0f});
    void Draw(
		const std::shared_ptr<Texture2D> &texture,
		const glm::vec2 &position,
		const std::vector<glm::vec2> &offsets,
		glm::vec2 size = glm::vec2{10, 10},
		GLfloat rotate = 0.0f,
		glm::vec3 color = glm::vec3{1.0f});
};

#endif // SPRITE_RENDERER_HPP_
