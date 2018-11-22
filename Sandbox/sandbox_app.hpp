#ifndef SANDBOX_HPP_
#define SANDBOX_HPP_

#include <jf2.hpp>

class Sandbox : public Application {
public:
	Sandbox();
	~Sandbox();

	virtual void Init() override final;
	virtual void ProcessInput(float dt) override final;
	virtual void Render() override final;
	virtual void SetCallbacks() override final;
	virtual void Run() override final;

private:
	glm::fvec3 light_position_;
	glm::fmat4 view_, model_;
	static glm::fmat4 projection;
	static std::unique_ptr<Camera> camera;
	std::vector<std::unique_ptr<Audio>> audio_list_;
	std::vector<std::unique_ptr<MeshRenderer>> meshes_;
	std::vector<std::unique_ptr<SpriteRenderer>> sprites_;
};

Application* CreateApplication();

#endif // SANDBOX_HPP_
