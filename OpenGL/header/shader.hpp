#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

class Shader {
private:
	GLuint handle_{};
	GLuint vert_shader_{}, frag_shader_{};
	std::string LoadFile(const std::string);
public:
	//Shader() = delete;
	Shader();
	Shader(std::string, std::string);
	virtual ~Shader();
	void Use() const;
	GLuint GetHandle() { return handle_; }
	static bool Init();
};

#endif // SHADER_HPP_

