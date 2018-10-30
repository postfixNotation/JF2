#ifndef INPUT_HANDLER_HPP_
#define INPUT_HANDLER_HPP_

#include <command.hpp>
#include <GLFW/glfw3.h>

class InputHandler {
public:
	void HandleInput();
	typedef enum {
		BUTTON_W = GLFW_KEY_W,
		BUTTON_S = GLFW_KEY_S,
		BUTTON_A = GLFW_KEY_A,
		BUTTON_D = GLFW_KEY_D
	} Button;
	bool IsPressed(Button button);
	// Methods to bind commands...

private:
	Command* button_x_/*  = new JumpCommand */;
	Command* button_y_/*  = new FireCommand */;
	Command* button_a_/*  = new NullCommand */;
	Command* button_b_/*  = new NullCommand */;
};

void InputHandler::HandleInput() {
	if (IsPressed(BUTTON_W)) button_x_->execute();
	else if (IsPressed(BUTTON_S)) button_y_->execute();
	else if (IsPressed(BUTTON_A)) button_a_->execute();
	else if (IsPressed(BUTTON_D)) button_b_->execute();
}

bool InputHandler::IsPressed(Button button) {
	//return (glfwGetKey(window, button) == GLFW_PRESS);
	return true;
}

#endif // INPUT_HANDLER_HPP_
