#include <sprite_renderer.hpp>

SpriteRenderer::SpriteRenderer(
    const std::shared_ptr<Shader> &shader,
    const GLfloat &width,
    const GLfloat &height) :
    indices_{ 0, 1, 2, 0, 2, 3 }, shader_{ shader } {
    projection_ = glm::ortho(
        0.0f,
        width,
        height,
        0.0f,
        -1.0f,
        1.0f);
    InitRenderData();
}

SpriteRenderer::~SpriteRenderer() {}

void SpriteRenderer::Draw(
	const std::shared_ptr<Texture2D> &texture,
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

    texture->Bind("image_sampler", 0);
    Renderer::Render(*va_, *ib_, *shader_);
    texture->Unbind(0);
}

void SpriteRenderer::InitRenderData() {
	constexpr GLubyte kNumberComponents{ 4 }; //position and texture coordinate
    std::vector<GLfloat> vertices { 
        1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 
        0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 0.0f,
    };
    VertexBuffer vb{
        reinterpret_cast<const GLvoid*>(vertices.data()),
        static_cast<GLuint>(sizeof(GLfloat)) * static_cast<GLuint>(vertices.size())
    };
	VertexBufferLayout vbl{};
    vbl.Push<GLfloat>(kNumberComponents);
    va_ = std::make_shared<VertexArray>(vb, vbl);
    ib_ = std::make_shared<IndexBuffer>(indices_.data(), indices_.size());
    ib_->Unbind();
    vb.Unbind();
    va_->Unbind();
}
