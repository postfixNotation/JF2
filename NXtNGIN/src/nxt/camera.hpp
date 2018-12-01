#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <utility>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace nxt {
	enum class CameraMovement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class Camera {
	public:
		float GetFov() const { return fov_; }
		float GetNear() const { return near_; }
		float GetFar() const { return far_; }
		void SetFov(float fov) { fov_ = fov; }
		void SetNear(float near) { near_ = near; }
		void SetFar(float far) { far_ = far; }

		glm::fmat4 GetViewMatrix(bool translation = true) const;
		glm::fmat4 GetProjectionMatrix(float ratio) const;
		const glm::fvec3& GetPosition() const { return position_; }

		void HandleMouseCursor(float xpos, float ypos);
		virtual void HandleScroll(float delta_scroll) = 0;
		virtual void HandleKeyboard(const CameraMovement &m, float dt) = 0;
	protected:
		Camera() {}
		// degrees
		void Rotate(float delta_yaw, float delta_pitch);
		virtual void UpdateVectors() = 0;

		const glm::fvec3 kWorldUp{ 0.0f, 1.0f, 0.0f };
		glm::fvec3 up_{ 0.0f, 1.0f, 0.0f };
		glm::fvec3 position_{ 0.0f };
		glm::fvec3 target_{ 0.0f };
		// radians
		float yaw_{ 0.0f };
		float pitch_{ 0.0f };
		// degrees
		static constexpr float kDefFov = 45.0f;
		static constexpr float kInitOrbitRadius = 10.0f;
		static constexpr float kZoomSensitivity = -2.0f;
		static constexpr float kMouseSensitivity = 0.1f;
		static constexpr float kMaxRadius = 80.0f;
		static constexpr float kMinRadius = 2.0f;
		static constexpr float kMoveSpeed = 5.0f;
		static constexpr float kMaxFOV = 120.0f;
		static constexpr float kMinFOV = 1.0f;
		static constexpr float kNear = 0.1f;
		static constexpr float kFar = 100.0f;

	private:
		float fov_{ kDefFov };
		float near_{ kNear };
		float far_{ kFar };
	};

	class FPSCamera final : public Camera {
	public:
		FPSCamera(
			glm::fvec3 position = glm::fvec3{ 0.0f, 0.0f, 0.0f },
			float yaw = glm::pi<float>(),
			float pitch = 0.0f);
		const glm::fvec3& GetLook() const { return look_; }
		const glm::fvec3& GetRight() const { return right_; }
		const glm::fvec3& GetUp() const { return up_; }
		void SetPosition(const glm::fvec3&& position) { position_ = std::move(position); }
		void Move(const glm::fvec3& delta);
		void HandleKeyboard(const CameraMovement &m, float dt) override final;
		void HandleScroll(float delta_scroll) override final;
	private:
		glm::fvec3 look_;
		glm::fvec3 right_{ 1.0f, 0.0f, 0.0f };
		virtual void UpdateVectors() override final;
	};

	class OrbitCamera final : public Camera {
	public:
		OrbitCamera() { UpdateVectors(); }
		void HandleKeyboard(const CameraMovement &m, float dt) override final;
		void HandleScroll(float delta_scroll) override final;
		void SetLookAt(const glm::fvec3& target);
		void SetRadius(float delta);
	private:
		float radius_{ kInitOrbitRadius };
		virtual void UpdateVectors() override final;
	};
}

#endif // CAMERA_HPP_
