#ifndef SANDBOX_HPP_
#define SANDBOX_HPP_

#include <nxt.hpp>

class Sandbox : public nxt::Application
{
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
    static std::unique_ptr<nxt::Camera> camera;
    std::vector<std::unique_ptr<nxt::Audio>> audio_list_;
    std::vector<std::unique_ptr<nxt::MeshRenderer>> meshes_;
    std::vector<std::unique_ptr<nxt::SpriteRenderer>> sprites_;
};

nxt::Application* nxt::CreateApplication();

#endif // SANDBOX_HPP_
