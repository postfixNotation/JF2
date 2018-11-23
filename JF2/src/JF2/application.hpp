#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

namespace jf2 {
	class Application {
	public:
		Application();
		virtual ~Application();

		virtual void Init() = 0;
		virtual void ProcessInput(float dt) = 0;
		virtual void Render() = 0;
		virtual void SetCallbacks() = 0;
		virtual void Run() = 0;
	};

	Application* CreateApplication();
}

#endif // APPLICATION_HPP_
