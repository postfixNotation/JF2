#include <sandbox.hpp>

Application *application = new Sandbox;

int main(int argc, const char **argv) {
	application->Init();
	application->SetCallbacks();
	application->Run(Context::Instance());

	delete application;
	Context::Instance().Terminate();
	return 0;
}
