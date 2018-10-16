#include <sprite_renderer.hpp>

SpriteRenderer::SpriteRenderer(
    std::shared_ptr<Shader> &shader,
    const GLfloat &width,
    const GLfloat &height) :
    indices_{ 0, 1, 2, 0, 2, 3 }, shader_{ shader } {
    projection_ = glm::ortho(
        0.0f,
        width,
        height,
        0.0f,
        -1.0f,
        1.0f
    );
    InitRenderData();
}

SpriteRenderer::~SpriteRenderer() { glDeleteVertexArrays(1, &vao_); }

void SpriteRenderer::DrawSprite(
	std::shared_ptr<Texture2D> &texture,
	glm::vec2 position,
	glm::vec2 size,
	GLfloat rotate,
	glm::vec3 color) {

    glm::mat4 model{};
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    shader_->SetMat4("model", model);
    shader_->SetMat4("projection", projection_);
    shader_->SetVec3("sprite_color", color);

    texture->BindTextureUnit("image_sampler", 0);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glDrawElements(
        GL_TRIANGLES,
        indices_.size(),
        GL_UNSIGNED_INT,
        nullptr
    );
    texture->UnbindTextureUnit(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::InitRenderData() {
	constexpr GLubyte kNumberComponents{ 4 }; //position and texture coordinate
    GLuint vbo;

    std::vector<GLfloat> vertices { 
        1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 
        0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 0.0f,
    };

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo_);
    glGenVertexArrays(1, &vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(GLfloat) * vertices.size(),
		vertices.data(),
		GL_STATIC_DRAW
	);

    glBindVertexArray(vao_);
    glVertexAttribPointer(
		0,
		kNumberComponents,
		GL_FLOAT,
		GL_FALSE,
		kNumberComponents * sizeof(GLfloat),
        reinterpret_cast<const GLvoid*>(0)
	);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * indices_.size(),
        indices_.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
