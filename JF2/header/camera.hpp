#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <utility>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

enum class CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {
public:
	void HandleMouseCursor(double xpos, double ypos);
	double GetFov() const { return fov_; }
	glm::vec3 GetPosition() const { return position_; }
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix(double ratio, double near, double far) const;
	virtual void HandleScroll(double delta_scroll) = 0;
	virtual void HandleKeyboard(const CameraMovement &m, double dt) = 0;
protected:
	Camera() {}
	// degrees
	void Rotate(double delta_yaw, double delta_pitch);
	void SetFov(double fov) { fov_ = fov; }
	virtual void UpdateVectors() = 0;

	const glm::vec3 kWorldUp{ 0.0f, 1.0f, 0.0f };
	glm::vec3 up_{ 0.0f, 1.0f, 0.0f };
	glm::vec3 position_{ 0.0f };
	glm::vec3 target_{ 0.0f };
	// radians
	float yaw_{ 0.0f };
	float pitch_{ 0.0f };
	// degrees
	static constexpr double kDefFov = 45.0;
	static constexpr float kMoveSpeed = 5.0;
	static constexpr double kInitOrbitRadius = 10.0;
	static constexpr double kZoomSensitivity = -2.0;
	static constexpr double kMouseSensitivity = 0.1;
	static constexpr double kMaxFOV = 120.0;
	static constexpr double kMinFOV = 1.0;
private:
	double fov_{ kDefFov };
};

class FPSCamera final : public Camera {
public:
	FPSCamera(
		glm::vec3 position = glm::vec3{ 0.0f, 0.0f, 0.0f },
		float yaw = glm::pi<float>(),
		float pitch = 0.0f);
	const glm::vec3& GetLook() const { return look_; }
	const glm::vec3& GetRight() const { return right_; }
	const glm::vec3& GetUp() const { return up_; }
	const glm::vec3& GetPosition() const { return position_; }
	void SetPosition(const glm::vec3&& position) { position_ = std::move(position); }
	void Move(const glm::vec3& delta);
	void HandleKeyboard(const CameraMovement &m, double dt) override final;
	void HandleScroll(double delta_scroll) override final;
private:
	glm::vec3 look_;
	glm::vec3 right_{ 1.0f, 0.0f, 0.0f };
	virtual void UpdateVectors() override final;
};

class OrbitCamera final : public Camera {
public:
	OrbitCamera() { UpdateVectors(); }
	void HandleKeyboard(const CameraMovement &m, double dt) override final;
	void HandleScroll(double delta_scroll) override final;
	void SetLookAt(const glm::vec3& target);
	void SetRadius(double delta);
private:
	float radius_{ kInitOrbitRadius };
	virtual void UpdateVectors() override final;
};

#endif // CAMERA_HPP_
