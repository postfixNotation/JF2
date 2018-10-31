#include <input_handler.hpp>

void InputHandler::Init() {
	button_a_  = new CloseCommand();
	button_b_  = new NullCommand();
}

void InputHandler::HandleInput() {
	if (IsPressed(KEY_ESCAPE))	button_a_->execute();
}

bool InputHandler::IsPressed(KeyNum button) {
	return Context::Instance().KeyDown(button);
}

// Change to smart pointers
InputHandler::~InputHandler() {
	delete button_a_;
	delete button_b_;
}
