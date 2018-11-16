#include <sandbox.hpp>

Application *application = new Sandbox;

int main(int argc, const char **argv) {
	application->Init();
	application->SetCallbacks();

	// application->Run(window);
	while (!Context::Instance()) {
		application->ProcessInput(Context::Instance().GetTimePerFrame());
		// remove singleton, pass context as parameter
		application->Render();
	}

	delete application;
	Context::Instance().Terminate();
	return 0;
}
