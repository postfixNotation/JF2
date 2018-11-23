#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <string>

namespace jf2 {
	namespace context {
		enum class Size {
			DEBUG,
			FULLSCREEEN
		};

		struct Config {
			int opengl_major;
			int opengl_minor;
			int number_of_samples;
			bool debug_context;
			bool cusor_enabled;
			std::string context_title;
			Size context_size;
		};
	}
}

#endif // CONFIG_HPP_
