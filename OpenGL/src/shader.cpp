#include <shader.hpp>

Shader::Shader() {}

Shader::Shader(std::string vert_file, std::string frag_file) {
	std::string vertex_string = LoadFile(vert_file);
	std::string fragment_string = LoadFile(frag_file);

	const GLchar* vertex_src = vertex_string.c_str();
	const GLchar* fragment_src = fragment_string.c_str();

	vert_shader_ = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader_, 1, &vertex_src, nullptr);
	glCompileShader(vert_shader_);

	frag_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader_, 1, &fragment_src, nullptr);
	glCompileShader(frag_shader_);

	handle_ = glCreateProgram();
	glAttachShader(handle_, vert_shader_);
	glAttachShader(handle_, frag_shader_);
	glLinkProgram(handle_);

	glDeleteShader(vert_shader_);
	glDeleteShader(frag_shader_);
}

Shader::~Shader() { glDeleteProgram(handle_); }

void Shader::Use() const { glUseProgram(handle_); }

bool Shader::Init() {
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cerr << "ERROR: COULD NOT INITIALIZE GLEW" << std::endl;
		return false;
	}
	return true;
}

std::string Shader::LoadFile(const std::string file) {
	std::ifstream ifs;
	std::stringstream ss;

	try {
		ifs.open(file, std::ifstream::in);

		if (!ifs.fail()) {
			ss << ifs.rdbuf();
		}

		ifs.close();
	}
	catch (std::exception ex) {
		std::cout << "EXCEPTION: LOADING SHADER SOURCE" << std::endl;
		std::cout << ex.what() << std::endl;
	}

	return ss.str();
}
