#include <camera.hpp>

constexpr float kDefFov = 30.0f; // degrees

Camera::Camera() :
	position_{ glm::vec3{0.0f,0.0f,0.0f} },
	target_{ glm::vec3{0.0f,0.0f,0.0f} },
	up_{ glm::vec3{0.0f,1.0f,0.0f} },
	yaw_{ 0.0f },
	pitch_{ 0.0f },
	fov_{ kDefFov }
{}

glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(position_, target_, up_);
}

FPSCamera::FPSCamera(glm::vec3 position, float yaw, float pitch) :
	right_{ 0.0f,0.0f,0.0f },
	kWorldUp{ 0.0f, 1.0f, 0.0f } {
	position_ = position;
	yaw_ = yaw;
	pitch_ = pitch;
}

// constrain test
void FPSCamera::Rotate(float yaw, float pitch) {
	yaw_ += glm::radians(yaw);
	pitch_ += glm::radians(pitch);
	float lower_bound = -glm::pi<float>() / 2.0f + 0.1f;
	float upper_bound = glm::pi<float>() / 2.0f - 0.1f;
	pitch_ = glm::clamp(pitch_, lower_bound, upper_bound);
	UpdateCameraVectors();
}

void FPSCamera::Move(const glm::vec3& offset_position) {
	position_.x += offset_position.x;
	position_.z += offset_position.z;
	UpdateCameraVectors();
}

void FPSCamera::UpdateCameraVectors() {
	glm::vec3 look;
	look.x = cosf(pitch_) * sinf(yaw_);
	look.y = sinf(pitch_);
	look.z = cosf(pitch_) * cosf(yaw_);

	look_ = glm::normalize(look);
	right_ = glm::normalize(glm::cross(look_, kWorldUp));
	up_ = glm::normalize(glm::cross(right_, look_));

	target_ = position_ + look_;
}

OrbitCamera::OrbitCamera() :
	radius_{ 10.0f } {}

void OrbitCamera::SetLookAt(const glm::vec3& target) {
	target_ = target;
	UpdateCameraVectors();
}

void OrbitCamera::SetRadius(float radius) {
	radius_ = glm::clamp(radius, 2.0f, 80.0f);
	UpdateCameraVectors();
}

// constrain test
void OrbitCamera::Rotate(float yaw, float pitch) {
	yaw_ = glm::radians(yaw);
	pitch_ = glm::radians(pitch);
	float lower_bound = -glm::pi<float>() / 2.0f + 0.1f;
	float upper_bound = glm::pi<float>() / 2.0f - 0.1f;
	pitch_ = glm::clamp(pitch_, lower_bound, upper_bound);
	UpdateCameraVectors();
}

void OrbitCamera::UpdateCameraVectors() {
	position_.x = target_.x + radius_ * cosf(pitch_) * sinf(yaw_);
	position_.y = target_.y + radius_ * sinf(pitch_);
	position_.z = target_.z + radius_ * cosf(pitch_) * cosf(yaw_);
}
