#ifndef SANDBOX_HPP_
#define SANDBOX_HPP_

#include <application.hpp>

class Sandbox : public Application {
public:
	Sandbox();
	~Sandbox();

	void Init();
	void ProcessInput(double dt);
	void Render();
	void SetCallbacks();
	void Run(const Context& context);

private:
	glm::vec3 light_position_;
	glm::mat4 view_, model_;
	static glm::mat4 projection_;
	static std::unique_ptr<Camera> camera_;
	std::vector<std::unique_ptr<Audio>> audio_list_;
	std::vector<std::unique_ptr<MeshRenderer>> meshes_;
	std::vector<std::unique_ptr<SpriteRenderer>> sprites_;
};

#endif // SANDBOX_HPP_
