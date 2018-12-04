#ifndef VIRTUAL_SHOW_ROOM_HPP_
#define VIRTUAL_SHOW_ROOM_HPP_

#include <nxt.hpp>

class VirtualShowRoom : public nxt::Application {
public:
	VirtualShowRoom();
	~VirtualShowRoom();

	virtual void Init() override final;
	virtual void ProcessInput(float dt) override final;
	virtual void Render() override final;
	virtual void SetCallbacks() override final;
	virtual void Run() override final;

private:
	glm::fvec3 light_position_;

	glm::fmat4 view_{};
	glm::fmat4 floor_model_{};
	glm::fmat4 cupboard_model_{};
	glm::fmat4 table_model_{};
	glm::fmat4 tv_model_{};
	glm::fmat4 sofa_model_{};
	glm::fmat4 lowboard_model_{};
	glm::fmat4 lamp_model_{};

	static glm::fmat4 projection;
	static std::unique_ptr<nxt::Camera> camera;
	std::vector<std::unique_ptr<nxt::Audio>> audio_list_;
	std::vector<std::unique_ptr<nxt::MeshRenderer>> meshes_;
};

nxt::Application* nxt::CreateApplication();

#endif // VIRTUAL_SHOW_ROOM_HPP_
