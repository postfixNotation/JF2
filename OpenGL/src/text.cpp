#include <text.hpp>

void Text::LoadFonts() {
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cerr << "ERROR::FREETYPE: COULD NOT INIT FREETYPE LIBRARY" << std::endl;
	}
	FT_Face face;
	if (FT_New_Face(ft, filename_.c_str(), 0, &face)) {
		std::cerr << "ERROR::FREETYPE: FAILED TO LOAD FONT" << std::endl;
	}
	FT_Set_Pixel_Sizes(face, 0, 112);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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
			face->glyph->advance.x
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

	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(GLfloat) * kVerticesPerQuad * kPositionAndTexture,
		nullptr,
		GL_DYNAMIC_DRAW
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		kPositionAndTexture,
		GL_FLOAT,
		GL_FALSE,
		kPositionAndTexture * sizeof(GLfloat),
		0
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text::UseProjection() const {
	glUseProgram(shader_handle_);
	glUniformMatrix4fv(
		glGetUniformLocation(shader_handle_, "projection"),
		1,
		GL_FALSE,
		glm::value_ptr(projection_)
	);
}

Text::Text() {
	InitBuffers();
}

Text::Text(GLuint shader_handle) {
	shader_handle_ = shader_handle;
	InitBuffers();
}

Text::~Text() {}

void Text::SetShaderHandle(GLuint shader_handle) {
	shader_handle_ = shader_handle;
}

void Text::SetFileName(std::string filename) {
	filename_ = filename;
	LoadFonts();
}

void Text::SetProjectionMatrix(glm::mat4 projection) {
	projection_ = projection;
}

void Text::RenderText(GLuint handle, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
	SetShaderHandle(handle);
	UseProjection();
	glUniform3f(
		glGetUniformLocation(handle, "textColor"),
		color.x,
		color.y,
		color.z
	);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao_);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = characters_[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		GLfloat vertices[kVerticesPerQuad][kPositionAndTexture] = {
			{ xpos,     ypos + h, 0.0, 0.0 },
			{ xpos,     ypos,     0.0, 1.0 },
			{ xpos + w, ypos,     1.0, 1.0 },
			{ xpos,     ypos + h, 0.0, 0.0 },
			{ xpos + w, ypos,     1.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 0.0 }
		};
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferSubData(
			GL_ARRAY_BUFFER,
			0,
			sizeof(vertices),
			vertices
		);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6); // add ibo -> glDrawElements(...)
		x += (ch.Advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
	RenderText(shader_handle_,
		text,
		x,
		y,
		scale,
		color
	);
}
