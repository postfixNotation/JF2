#include <input_handler.hpp>

void InputHandler::Init() {
	button_a_  = std::unique_ptr<CloseCommand>(new CloseCommand());
	button_b_  = std::unique_ptr<NullCommand>(new NullCommand());
	button_c_  = std::unique_ptr<PolygonCommand>(new PolygonCommand());
	button_d_  = std::unique_ptr<FillCommand>(new FillCommand());
}

void InputHandler::HandleInput() {
	if (IsPressed(KEY_ESCAPE))		button_a_->execute();
	else if (IsPressed(KEY_TAB))	button_b_->execute();
	else if (IsPressed(KEY_Q))		button_c_->execute();
	else if (IsPressed(KEY_E))		button_d_->execute();
}

bool InputHandler::IsPressed(KeyNum button) {
	return Context::Instance().KeyDown(button);
}
