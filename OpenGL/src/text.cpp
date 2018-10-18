#include <text.hpp>

void Text::LoadFonts() {
	characters_.clear();
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cerr << "ERROR::FREETYPE: COULD NOT INIT FREETYPE LIBRARY" << std::endl;
	}
	FT_Face face;
	if (FT_New_Face(ft, filename_.c_str(), 0, &face)) {
		std::cerr << "ERROR::FREETYPE: FAILED TO LOAD FONT" << std::endl;
	}
	FT_Set_Pixel_Sizes(face, 0, default_pixel_size_);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// more than ASCII set to also get further glyphs
	for (GLubyte c = 0; c < 170; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cerr << "ERROR::FREETYTPE: FAILED TO LOAD GLYPH" << std::endl;
			continue;
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<GLuint>(face->glyph->advance.x)
		};
		characters_.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void Text::InitBuffers() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	VertexBufferLayout vbl{};
	vbl.Push<GLfloat>(kPositionAndTexture);

	ib_ = std::make_shared<IndexBuffer>(indices_.data(), indices_.size());
	ib_->Unbind();

	vb_ = std::make_shared<VertexBuffer>(
		nullptr,
		sizeof(GLfloat) * kVerticesPerQuad * kPositionAndTexture,
		DrawType::DYNAMIC
	);

	va_ = std::make_shared<VertexArray>(*vb_, vbl);
	vb_->Unbind();
	va_->Unbind();
}

Text::Text(std::shared_ptr<Shader> shader, size_t width, size_t height) :
	shader_{ shader }, indices_{ 0, 1, 2, 0, 2, 3 } {
	projection_ = glm::ortho(
		0.0f,
		static_cast<GLfloat>(width),
		static_cast<GLfloat>(height),
		0.0f
	);
	InitBuffers();
}

Text::~Text() {
	for (std::map<GLchar, Character>::iterator it{ begin(characters_) }; it != end(characters_); ++it) {
		glDeleteTextures(1, &(it->second.texture_id));
	}
}

void Text::SetFileName(std::string filename, size_t pixel_size) {
	default_pixel_size_ = pixel_size;
	filename_ = filename;
	LoadFonts();
}

void Text::Draw(
		std::string text,
		GLfloat x,
		GLfloat y,
		GLfloat scale,
		glm::vec3 color,
		std::shared_ptr<Shader> shader) {

	if (shader.get() != nullptr) {
		shader->SetMat4("projection", projection_);
		shader->SetVec3("text_color", color);
	}
	else {
		shader_->SetMat4("projection", projection_);
		shader_->SetVec3("text_color", color);
	}
	glActiveTexture(GL_TEXTURE0);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); ++c) {
		Character ch = characters_[*c];

		GLfloat xpos = x + ch.bearing.x * scale;
		GLfloat ypos = y + (characters_['H'].bearing.y - ch.bearing.y) * scale;

		GLfloat w = ch.character_size.x * scale;
		GLfloat h = ch.character_size.y * scale;

		GLfloat vertices[kVerticesPerQuad][kPositionAndTexture] = {
			{ xpos,     ypos + h, 0.0f, 1.0f },
			{ xpos + w, ypos + h, 1.0f, 1.0f },
			{ xpos + w, ypos,     1.0f, 0.0f },
			{ xpos,     ypos,     0.0f, 0.0f },
		};
		glBindTexture(GL_TEXTURE_2D, ch.texture_id);

		vb_->Bind();
		vb_->BufferSubData(vertices, sizeof(vertices));
		vb_->Unbind();
		Renderer::Render(*va_, *ib_, *shader_);

		x += (ch.advance >> 6) * scale;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
