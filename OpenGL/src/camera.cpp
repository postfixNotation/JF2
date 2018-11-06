#include <camera.hpp>

glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(position_, target_, up_);
}

FPSCamera::FPSCamera(glm::vec3 position, float yaw, float pitch) {
	position_ = position;
	yaw_ = yaw;
	pitch_ = pitch;
}

void FPSCamera::Rotate(float delta_yaw, float delta_pitch) {
	yaw_ += glm::radians(kMouseSensitivity * delta_yaw);
	pitch_ += glm::radians(kMouseSensitivity * delta_pitch);
	float lower_bound = -glm::pi<float>() / 2.0f + 0.1f;
	float upper_bound = glm::pi<float>() / 2.0f - 0.1f;
	pitch_ = glm::clamp(pitch_, lower_bound, upper_bound);
	UpdateVectors();
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
		break;
	}
}

void FPSCamera::HandleMouseCursor(double xpos, double ypos) {
	static glm::vec2 last_pos{ xpos, ypos };
	Rotate(last_pos.x - xpos, last_pos.y - ypos);
	last_pos = std::move(glm::vec2{ xpos, ypos });
}

void FPSCamera::HandleScroll(double delta_scroll) {
	double fov = GetFov() + delta_scroll * kZoomSensitivity;
	SetFov(static_cast<float>(glm::clamp(fov, kMinFOV, kMaxFOV)));
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

void OrbitCamera::HandleScroll(double delta_scroll) {
	SetRadius(static_cast<float>(kZoomSensitivity * delta_scroll));
}

void OrbitCamera::HandleMouseCursor(double xpos, double ypos) {}

void OrbitCamera::HandleKeyboard(const CameraMovement &m, double dt) {}

void OrbitCamera::Rotate(float delta_yaw, float delta_pitch) {
	yaw_ = glm::radians(delta_yaw);
	pitch_ = glm::radians(delta_pitch);
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
