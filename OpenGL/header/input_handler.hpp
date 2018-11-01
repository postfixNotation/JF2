#ifndef INPUT_HANDLER_HPP_
#define INPUT_HANDLER_HPP_

#include <command.hpp>
#include <memory>

typedef enum {
	KEY_ESCAPE			= GLFW_KEY_ESCAPE,
	KEY_TAB				= GLFW_KEY_TAB,
	KEY_Q				= GLFW_KEY_Q,
	KEY_W				= GLFW_KEY_W,
	KEY_E				= GLFW_KEY_E,
	KEY_R				= GLFW_KEY_R,
	KEY_A				= GLFW_KEY_A,
	KEY_S				= GLFW_KEY_S,
	KEY_D				= GLFW_KEY_D,
	KEY_F				= GLFW_KEY_F,
	KEY_Y				= GLFW_KEY_Y,
	KEY_X				= GLFW_KEY_X,
	KEY_C				= GLFW_KEY_C,
	KEY_V				= GLFW_KEY_V,
	KEY_MOUSE_LEFT		= GLFW_KEY_V,
	KEY_MOUSE_RIGHT		= GLFW_KEY_V,
} KeyNum;

class InputHandler {
public:
	void Init();
	void HandleInput();
	bool IsPressed(KeyNum button);

private:
	std::unique_ptr<Command> button_a_;
	std::unique_ptr<Command> button_b_;
	std::unique_ptr<Command> button_c_;
	std::unique_ptr<Command> button_d_;
};

#endif // INPUT_HANDLER_HPP_