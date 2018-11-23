#ifndef SANDBOX_HPP_
#define SANDBOX_HPP_

#include <jf2.hpp>

class Sandbox : public jf2::Application {
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
	static std::unique_ptr<jf2::Camera> camera;
	std::vector<std::unique_ptr<jf2::Audio>> audio_list_;
	std::vector<std::unique_ptr<jf2::MeshRenderer>> meshes_;
	std::vector<std::unique_ptr<jf2::SpriteRenderer>> sprites_;
};

jf2::Application* jf2::CreateApplication();

#endif // SANDBOX_HPP_
