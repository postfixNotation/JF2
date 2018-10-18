#ifndef SPRITE_RENDERER_HPP_
#define SPRITE_RENDERER_HPP_

#include <vector>
#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vertex_buffer_layout.hpp>
#include <vertex_buffer.hpp>
#include <vertex_array.hpp>
#include <index_buffer.hpp>
#include <texture2d.hpp>
#include <shader.hpp>

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
		std::shared_ptr<Shader> &shader,
		const GLfloat &width,
		const GLfloat &height
	);
    ~SpriteRenderer();
    void Draw(
		std::shared_ptr<Texture2D> &texture,
		glm::vec2 position,
		glm::vec2 size = glm::vec2{10, 10},
		GLfloat rotate = 0.0f,
		glm::vec3 color = glm::vec3{1.0f}
	);
};

#endif // SPRITE_RENDERER_HPP_
