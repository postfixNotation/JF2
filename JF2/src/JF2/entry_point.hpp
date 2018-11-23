#ifndef ENTRY_POINT_HPP_
#define ENTRY_POINT_HPP_

extern jf2::Application* jf2::CreateApplication();

int main(int argc, const char **argv) {
	auto app = jf2::CreateApplication();
	app->Init();
	app->SetCallbacks();
	app->Run();

	delete app;
	return 0;
}

#endif // ENTRY_POINT_HPP_
