#define STB_IMAGE_IMPLEMENTATION
#include <texture2d.hpp>

bool Texture2D::Load(const std::string& file_name, bool gen_mipmaps) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLenum format;
	int width, height, components;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *image_data = stbi_load(
		file_name.c_str(),
		&width,
		&height,
		&components,
		0
	);

	if (image_data == NULL) {
		std::cerr << "ERROR LOADING TEXTURE '" << file_name << "'" << std::endl;
		return false;
	}
	else {
		switch (components) {
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		}
	}

	glGenTextures(1, &handle_);
	glBindTexture(GL_TEXTURE_2D, handle_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		format,
		width,
		height,
		0,
		format,
		GL_UNSIGNED_BYTE,
		image_data
	);

	if (gen_mipmaps) { glGenerateMipmap(GL_TEXTURE_2D); }

	stbi_image_free(image_data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

bool Texture2D::Load(const std::vector<std::string>& faces) {
	is_cube_map_ = true;
	glGenTextures(1, &handle_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, handle_);

	GLenum format;
	int width, height, components;
	for (size_t i = 0; i < faces.size(); i++) {
		unsigned char *data = stbi_load(
			faces[i].c_str(),
			&width,
			&height,
			&components,
			0
		);

		switch (components) {
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		}

		if (data) {
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				format,
				width,
				height,
				0,
				format,
				GL_UNSIGNED_BYTE,
				data
			);
		}
		else {
			std::cerr << "CUBEMAP TEXTURE FAILED TO LOAD AT PATH: " << faces[i] << std::endl;
		}
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return true;
}

void Texture2D::Bind(const GLchar* uniform, GLuint texunit) const {
	GLint loc = glGetUniformLocation(shader_->GetHandle(), uniform);
	glUniform1i(loc, texunit);

	assert(texunit >= 0 && texunit < MAX_NUMBER_TEX_UNITS);
	glActiveTexture(GL_TEXTURE0 + texunit);
	if (is_cube_map_) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, handle_);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, handle_);
	}
}

void Texture2D::Unbind(GLuint texunit) const {
	assert(texunit >= 0 && texunit < MAX_NUMBER_TEX_UNITS);
	glActiveTexture(GL_TEXTURE0 + texunit);
	if (is_cube_map_) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
