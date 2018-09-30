#define STB_IMAGE_IMPLEMENTATION
#include "texture2d.hpp"

Texture2D::Texture2D() : texture_handle_{ 0 } {}
Texture2D::~Texture2D() {
	glDeleteTextures(1, &texture_handle_);
}

bool Texture2D::LoadTexture(const std::string& file_name, bool gen_mipmaps) {
	int width, height, components;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *image_data = stbi_load(
		file_name.c_str(),
		&width,
		&height,
		&components,
		STBI_rgb_alpha
	);

	if (image_data == NULL) {
		std::cerr << "ERROR LOADING TEXTURE '" << file_name << "'" << std::endl;
		return false;
	}

	glGenTextures(1, &texture_handle_);
	glBindTexture(GL_TEXTURE_2D, texture_handle_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data
	);

	if (gen_mipmaps) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(image_data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

void Texture2D::BindTextureUnit(GLuint texunit) const {
	assert(texunit >= 0 && texunit < (GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - GL_TEXTURE0));
	glActiveTexture(GL_TEXTURE0 + texunit);
	glBindTexture(GL_TEXTURE_2D, texture_handle_);
}

void Texture2D::UnbindTextureUnit(GLuint texunit) const {
	assert(texunit >= 0 && texunit < (GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - GL_TEXTURE0));
	glActiveTexture(GL_TEXTURE0 + texunit);
	glBindTexture(GL_TEXTURE_2D, 0);
}
