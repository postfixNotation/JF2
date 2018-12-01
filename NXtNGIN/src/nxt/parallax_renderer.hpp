#ifndef PARALLAX_RENDERER_HPP_
#define PARALLAX_RENDERER_HPP_

#include "sprite_renderer.hpp"

namespace nxt {
	class ParallaxRenderer {
	public:
		ParallaxRenderer(
			std::shared_ptr<SpriteRenderer> p_sprite_renderer,
			const glm::fvec2 &context_dimensions);
		~ParallaxRenderer();

		void Init(
			const std::shared_ptr<Shader> &p_shader,
			std::vector<std::string> image_path_list);

		void Draw(
			float perspective_coefficient,
			const glm::fvec2 &actor_pos);
	private:
		std::shared_ptr<SpriteRenderer> p_sprite_renderer_;
		std::vector<std::shared_ptr<Texture2D>> v_texture_list_;
		glm::fvec2 context_dimensions_;
		static float kMaxParallaxCoefficient;
	};
}

#endif // PARALLAX_RENDERER_HPP_
