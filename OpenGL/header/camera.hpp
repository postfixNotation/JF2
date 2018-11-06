#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <utility>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

//-----MOVE INTO CLASS AS STATIC MEMEBERS
constexpr float kDefFov = 45.0f; // degrees
constexpr float kMoveSpeed = 5.0f;
constexpr float kInitOrbitRadius = 10.0f;
constexpr float kZoomSensitivity = -2.0f;
constexpr float kMouseSensitivity = 0.1f;
constexpr double kMaxFOV = 120.0;
constexpr double kMinFOV = 1.0;

enum class CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {
private:
	float fov_{ kDefFov };

protected:
	Camera() {}
	virtual void UpdateVectors() = 0;

	const glm::vec3 kWorldUp{ 0.0f, 1.0f, 0.0f };
	glm::vec3 up_{ 0.0f, 1.0f, 0.0f };
	glm::vec3 position_{ 0.0f };
	glm::vec3 target_{ 0.0f };
	// radians
	float yaw_{ 0.0f };
	float pitch_{ 0.0f };

public:
	virtual void Rotate(float delta_yaw, float delta_pitch) = 0; // in degrees
	virtual void HandleScroll(double delta_scroll) = 0;
	virtual void HandleMouseCursor(double xpos, double ypos) = 0;
	virtual void HandleKeyboard(const CameraMovement &m, double dt) = 0;
	float GetFov() const { return fov_; }
	void SetFov(float fov) { fov_ = fov; } // in degrees
	glm::mat4 GetViewMatrix() const;
};

class FPSCamera : public Camera {
private:
	glm::vec3 look_;
	glm::vec3 right_{ 1.0f, 0.0f, 0.0f };
	virtual void UpdateVectors() override final;

public:
	FPSCamera(
		glm::vec3 position = glm::vec3{ 0.0f, 0.0f, 0.0f },
		float yaw = glm::pi<float>(),
		float pitch = 0.0f);

	virtual void Rotate(float delta_yaw, float delta_pitch) override final; // in degrees
	const glm::vec3& GetLook() const { return look_; }
	const glm::vec3& GetRight() const { return right_; }
	const glm::vec3& GetUp() const { return up_; }
	void SetPosition(const glm::vec3&& position) { position_ = std::move(position); }
	const glm::vec3& GetPosition() const { return position_; }
	void Move(const glm::vec3& delta);
	void HandleKeyboard(const CameraMovement &m, double dt) override final;
	void HandleMouseCursor(double xpos, double ypos) override final;
	void HandleScroll(double delta_scroll) override final;
};

class OrbitCamera : public Camera {
private:
	float radius_{ kInitOrbitRadius };
	virtual void UpdateVectors() override final;

public:
	OrbitCamera() { UpdateVectors(); }

	void Rotate(float delta_yaw, float delta_pitch) override final;
	void HandleKeyboard(const CameraMovement &m, double dt) override final;
	void HandleMouseCursor(double xpos, double ypos) override final;
	void HandleScroll(double delta_scroll) override final;
	void SetLookAt(const glm::vec3& target);
	void SetRadius(float delta);
};

#endif // CAMERA_HPP_

