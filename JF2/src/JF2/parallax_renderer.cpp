#include "parallax_renderer.hpp"

namespace jf2 {
	float ParallaxRenderer::kMaxParallaxCoefficient{ 5.0f };

	ParallaxRenderer::ParallaxRenderer(
		std::shared_ptr<SpriteRenderer> p_sprite_renderer,
		const glm::fvec2 &context_dimensions) :
		p_sprite_renderer_{ p_sprite_renderer },
		context_dimensions_{ context_dimensions }{}

	ParallaxRenderer::~ParallaxRenderer() {}

	void ParallaxRenderer::Init(
		const std::shared_ptr<Shader> &p_shader,
		std::vector<std::string> image_path_list) {
		for (const std::string &path : image_path_list) {
			v_texture_list_.push_back(std::move(std::make_shared<Texture2D>(p_shader, path)));
		}
	}

	void ParallaxRenderer::Draw(
		float perspective_coefficient,
		const glm::fvec2 &actor_pos) {
		perspective_coefficient = glm::clamp<float>(perspective_coefficient, 0.0f, kMaxParallaxCoefficient);
		float delta_perspective = perspective_coefficient / v_texture_list_.size();
		perspective_coefficient = 0.0f;
		glm::vec2 screen_pos{};

		for (const std::shared_ptr<Texture2D> &p_texture : v_texture_list_) {
			screen_pos.x = -perspective_coefficient * actor_pos.x;
			screen_pos.y = -perspective_coefficient * actor_pos.y;

			p_sprite_renderer_->Draw(
				p_texture,
				screen_pos,
				context_dimensions_ * (perspective_coefficient + 1.0f));
			perspective_coefficient += delta_perspective;
		}

	}
} // namespace jf2
