#include <camera.hpp>

glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(position_, target_, up_);
}

glm::mat4 Camera::GetProjectionMatrix(double ratio) const {
	return glm::perspective(glm::radians(fov_), ratio, near_, far_);
}

void Camera::Rotate(double delta_yaw, double delta_pitch) {
	yaw_ += glm::radians(kMouseSensitivity * delta_yaw);
	pitch_ += glm::radians(kMouseSensitivity * delta_pitch);
	double lower_bound = -glm::pi<double>() / 2.0f + 0.1f;
	double upper_bound = glm::pi<double>() / 2.0f - 0.1f;
	pitch_ = glm::clamp(pitch_, lower_bound, upper_bound);
	UpdateVectors();
}

void Camera::HandleMouseCursor(double xpos, double ypos) {
	static glm::vec2 last_pos{ xpos, ypos };
	Rotate(last_pos.x - xpos, last_pos.y - ypos);
	last_pos = std::move(glm::vec2{ xpos, ypos });
}

FPSCamera::FPSCamera(glm::vec3 position, double yaw, double pitch) {
	position_ = position;
	yaw_ = yaw;
	pitch_ = pitch;
}

void FPSCamera::Move(const glm::vec3& delta) {
	position_ += delta;
	UpdateVectors();
}

void FPSCamera::HandleKeyboard(const CameraMovement &m, double dt) {
	switch (m) {
	case CameraMovement::FORWARD:
		Move(kMoveSpeed * static_cast<float>(dt) * GetLook());
		break;
	case CameraMovement::BACKWARD:
		Move(kMoveSpeed * static_cast<float>(dt) * -GetLook());
		break;
	case CameraMovement::RIGHT:
		Move(kMoveSpeed * static_cast<float>(dt) * GetRight());
		break;
	case CameraMovement::LEFT:
		Move(kMoveSpeed * static_cast<float>(dt) * -GetRight());
		break;
	}
}

void FPSCamera::HandleScroll(double delta_scroll) {
	double fov = GetFov() + delta_scroll * kZoomSensitivity;
	SetFov(glm::clamp(fov, kMinFOV, kMaxFOV));
}

void FPSCamera::UpdateVectors() {
	glm::vec3 look;
	look.x = static_cast<float>(cos(pitch_) * sin(yaw_));
	look.y = static_cast<float>(sin(pitch_));
	look.z = static_cast<float>(cos(pitch_) * cos(yaw_));

	look_ = glm::normalize(look);
	right_ = glm::normalize(glm::cross(look_, kWorldUp));
	up_ = glm::normalize(glm::cross(right_, look_));

	target_ = position_ + look_;
}

void OrbitCamera::SetLookAt(const glm::vec3& target) {
	target_ = target;
	UpdateVectors();
}

void OrbitCamera::SetRadius(double delta) {
	radius_ += delta;
	radius_ = glm::clamp(radius_, 2.0, 80.0);
	UpdateVectors();
}

void OrbitCamera::HandleScroll(double delta_scroll) {
	SetRadius(kZoomSensitivity * delta_scroll);
}

void OrbitCamera::HandleKeyboard(const CameraMovement &m, double dt) {}

void OrbitCamera::UpdateVectors() {
	position_.x = static_cast<float>(target_.x + radius_ * cos(pitch_) * sin(yaw_));
	position_.y = static_cast<float>(target_.y + radius_ * sin(pitch_));
	position_.z = static_cast<float>(target_.z + radius_ * cos(pitch_) * cos(yaw_));

	glm::vec3 look{ target_ - position_ };
	glm::vec3 right{ glm::normalize(glm::cross(look, kWorldUp)) };
	up_ = glm::normalize(glm::cross(right, look));
}
