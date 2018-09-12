#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera {
private:
	// Camera parameters
	float fov_; // degrees

protected:
	Camera();
	virtual void UpdateCameraVectors() = 0;

	glm::vec3 up_;
	glm::vec3 position_;
	glm::vec3 target_;
	// Euler Angles (in radians)
	float yaw_;
	float pitch_;

public:
	virtual void Rotate(float, float) = 0; // abstract method, angles in degrees
	glm::mat4 GetViewMatrix() const;
	float GetFov() const { return fov_; }
	void SetFov(float fov) { fov_ = fov; } // in degrees
};

class FPSCamera : public Camera {
private:
	glm::vec3 look_;
	glm::vec3 right_;
	const glm::vec3 kWorldUp;
	virtual void UpdateCameraVectors() override final;

public:
	FPSCamera(
		glm::vec3 position = glm::vec3{ 0.0f,0.0f,0.0f },
		float yaw = glm::pi<float>(),
		float pitch = 0.0f
	);

	virtual void Rotate(float, float) override final;
	const glm::vec3& GetLook() const { return look_; }
	const glm::vec3& GetRight() const { return right_; }
	const glm::vec3& GetUp() const { return up_; }
	void SetPosition(const glm::vec3& position) { position_ = position; }
	const glm::vec3& GetPosition() const { return position_; }
	void Move(const glm::vec3&);
};

class OrbitCamera : public Camera {
private:
	void UpdateCameraVectors() override final;
	// Camera parameters
	float radius_;

public:
	OrbitCamera();

	virtual void Rotate(float, float);
	void SetLookAt(const glm::vec3&);
	void SetRadius(float);
};

#endif // CAMERA_HPP_
