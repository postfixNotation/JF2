#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <string>

enum class Size {
	DEBUG,
	MAXIMIZED,
	FULLSCREEEN
};

struct Config {
	size_t opengl_major;
	size_t opengl_minor;
	size_t number_of_samples;
	bool debug_context;
	bool cusor_enabled;
	std::string context_title;
	Size context_size;
};

#endif // CONFIG_HPP_
