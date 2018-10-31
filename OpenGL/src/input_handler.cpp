#include <input_handler.hpp>

void InputHandler::Init() {
	Command* button_a_  = new CloseCommand;
	Command* button_b_  = new NullCommand;
}

void InputHandler::HandleInput() {
	if (IsPressed(KEY_ESCAPE))	button_a_->execute();
}

bool InputHandler::IsPressed(KeyNum button) {
	return Context::Instance().KeyDown(button);
}