#ifndef CLIENT_DEV_HPP_
#define CLIENT_DEV_HPP_

#include <jf2.hpp>

class ClientDev : public jf2::Application {
public:
	ClientDev();
	~ClientDev();

	virtual void Init() override final;
	virtual void ProcessInput(float dt) override final;
	virtual void Render() override final;
	virtual void SetCallbacks() override final;
	virtual void Run() override final;

private:
	std::shared_ptr<jf2::SpriteRenderer> p_sprite_;
	std::unique_ptr<jf2::ParallaxRenderer> p_parallax_;
};

jf2::Application* jf2::CreateApplication();

#endif // CLIENT_DEV_HPP_
