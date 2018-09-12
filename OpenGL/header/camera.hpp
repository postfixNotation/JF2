#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera {
public:
	glm::mat4 GetViewMatrix() const;
	virtual void Rotate(float, float) = 0; // abstract method, angles in degrees

	const glm::vec3& GetLook() const;
	const glm::vec3& GetRight() const;
	const glm::vec3& GetUp() const;

	float GetFov() const { return fov_; }
	void SetFov(float fov) { fov_ = fov; }

protected:
	Camera();

	virtual void UpdateCameraVectors() = 0;

	glm::vec3 position_;
	glm::vec3 target_;
	glm::vec3 look_;
	glm::vec3 up_;
	glm::vec3 right_;
	const glm::vec3 kWorldUp;

	// Euler Angles (in radians)
	float yaw_;
	float pitch_;

	// Camera parameters
	float fov_; // degrees
};

class FPSCamera : public Camera {
public:
	FPSCamera(
		glm::vec3 position = glm::vec3{ 0.0f,0.0f,0.0f },
		float yaw = glm::pi<float>(),
		float pitch = 0.0f
	);

	virtual void Rotate(float, float) override final;
	void SetPosition(const glm::vec3&);
	void Move(const glm::vec3&);

private:
	virtual void UpdateCameraVectors() override final;
};

class OrbitCamera : public Camera {
public:
	OrbitCamera();

	virtual void Rotate(float, float);

	void SetLookAt(const glm::vec3&);
	void SetRadius(float);

private:
	void UpdateCameraVectors();
	float radius_;

};

#endif // CAMERA_HPP_
