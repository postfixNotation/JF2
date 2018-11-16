#include <application.hpp>

extern Application* CreateApplication();

int main(int argc, const char **argv) {
	auto app = CreateApplication();
	app->Init();
	app->SetCallbacks();
	app->Run();

	delete app;
	return 0;
}
