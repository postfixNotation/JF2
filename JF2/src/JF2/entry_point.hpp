#ifndef ENTRY_POINT_HPP_
#define ENTRY_POINT_HPP_

extern Application* CreateApplication();

int main(int argc, const char **argv) {
	auto app = CreateApplication();
	app->Init();
	app->SetCallbacks();
	app->Run();

	delete app;
	return 0;
}

#endif // ENTRY_POINT_HPP_
