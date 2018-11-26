#include "parallax_renderer.hpp"

namespace jf2 {
	ParallaxRenderer::ParallaxRenderer(
		std::shared_ptr<SpriteRenderer> p_sprite_renderer,
		const glm::fvec2 &context_dimensions) :
		p_sprite_renderer_{ p_sprite_renderer }, context_dimensions_{ context_dimensions }{}

	ParallaxRenderer::~ParallaxRenderer() {}

	void ParallaxRenderer::Init(
		const std::shared_ptr<Shader> &p_shader,
		std::vector<std::string> image_path_list) {
		for (const std::string &path : image_path_list) {
			v_texture_list_.push_back(std::move(std::make_shared<Texture2D>(p_shader, path)));
		}
	}

	void ParallaxRenderer::Draw(const glm::fvec2 &position) {
		static std::vector<size_t> stride{ 0, 20, 40, 0, 50, 10, 30, 30, 10, 30, 10 };
		size_t i{};

		for (const std::shared_ptr<Texture2D> &p_texture : v_texture_list_) {
			p_sprite_renderer_->Draw(
				p_texture, glm::fvec2{ position.x + stride[i], position.y },
				context_dimensions_);
			i++;
		}

		//for (auto it = std::begin(stride); it != std::end(stride); ++it) {
		//	*it += i++;
		//}
	}
}
