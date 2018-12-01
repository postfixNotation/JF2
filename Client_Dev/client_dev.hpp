#ifndef CLIENT_DEV_HPP_
#define CLIENT_DEV_HPP_

#include <nxt.hpp>

class ClientDev : public nxt::Application {
public:
	ClientDev();
	~ClientDev();

	virtual void Init() override final;
	virtual void ProcessInput(float dt) override final;
	virtual void Render() override final;
	virtual void SetCallbacks() override final;
	virtual void Run() override final;

private:
	std::shared_ptr<nxt::SpriteRenderer> p_sprite_;
	std::unique_ptr<nxt::ParallaxRenderer> p_parallax_;
};

nxt::Application* nxt::CreateApplication();

#endif // CLIENT_DEV_HPP_
