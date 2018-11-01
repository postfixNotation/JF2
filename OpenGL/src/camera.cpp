#include <camera.hpp>

glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(position_, target_, up_);
}

FPSCamera::FPSCamera(glm::vec3 position, float yaw, float pitch) {
	position_ = position;
	yaw_ = yaw;
	pitch_ = pitch;
}

void FPSCamera::Rotate(float yaw, float pitch) {
	yaw_ += glm::radians(yaw);
	pitch_ += glm::radians(pitch);
	float lower_bound = -glm::pi<float>() / 2.0f + 0.1f;
	float upper_bound = glm::pi<float>() / 2.0f - 0.1f;
	pitch_ = glm::clamp(pitch_, lower_bound, upper_bound);
	UpdateVectors();
}

void FPSCamera::Move(const glm::vec3& delta) {
	position_ += delta;
	UpdateVectors();
}

void FPSCamera::UpdateVectors() {
	glm::vec3 look;
	look.x = cosf(pitch_) * sinf(yaw_);
	look.y = sinf(pitch_);
	look.z = cosf(pitch_) * cosf(yaw_);

	look_ = glm::normalize(look);
	right_ = glm::normalize(glm::cross(look_, kWorldUp));
	up_ = glm::normalize(glm::cross(right_, look_));

	target_ = position_ + look_;
}

void OrbitCamera::SetLookAt(const glm::vec3& target) {
	target_ = target;
	UpdateVectors();
}

void OrbitCamera::SetRadius(float delta) {
	radius_ += delta;
	radius_ = glm::clamp(radius_, 2.0f, 80.0f);
	UpdateVectors();
}

void OrbitCamera::Rotate(float yaw, float pitch) {
	yaw_ = glm::radians(yaw);
	pitch_ = glm::radians(pitch);
	float lower_bound = -glm::pi<float>() / 2.0f + 0.1f;
	float upper_bound = glm::pi<float>() / 2.0f - 0.1f;
	pitch_ = glm::clamp(pitch_, lower_bound, upper_bound);
	UpdateVectors();
}

void OrbitCamera::UpdateVectors() {
	position_.x = target_.x + radius_ * cosf(pitch_) * sinf(yaw_);
	position_.y = target_.y + radius_ * sinf(pitch_);
	position_.z = target_.z + radius_ * cosf(pitch_) * cosf(yaw_);

	glm::vec3 look{ target_ - position_ };
	glm::vec3 right{ glm::normalize(glm::cross(look, kWorldUp)) };
	up_ = glm::normalize(glm::cross(right, look));
}
