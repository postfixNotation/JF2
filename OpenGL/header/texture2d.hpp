#ifndef TEXTURE_2D_HPP_
#define TEXTURE_2D_HPP_

#define MAX_NUMBER_TEX_UNITS 32

#include <iostream>
#include <vector>
#include <string>

#include <stb-master/stb_image.h>
#include <GL/glew.h>

#include <shader.hpp>

class Texture2D {
private:
	Shader *shader_;
	GLuint texture_handle_{};
public:
	Texture2D(Shader*);
	Texture2D(const Texture2D&) = delete;
	Texture2D& operator= (const Texture2D&) = delete;
	virtual ~Texture2D();

	bool LoadTexture(const std::string& file_name, bool gen_mipmaps = true);
	bool LoadCubemap(const std::vector<std::string>);

	void BindTextureUnit(const GLchar* uniform, GLuint texunit = 0);
	void BindCubeTextureUnit(const GLchar* uniform, GLuint texunit = 0);
	void UnbindTextureUnit(GLuint texunit = 0) const;
	void UnbindCubeTextureUnit(GLuint texunit = 0) const;
};

#endif // TEXTURE_2D_HPP_

