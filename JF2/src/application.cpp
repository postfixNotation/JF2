#define FPS 1
#define PHONG 1
#define STARS 0
#define VSDEBUG 1

#include <application.hpp>

Application *application = new Application;

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
