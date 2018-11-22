#include "camera.hpp"

glm::fmat4 Camera::GetViewMatrix(bool translation) const {
	if (translation) return glm::lookAt<float>(position_, target_, up_);
	else return glm::fmat4(glm::fmat3(glm::lookAt(position_, target_, up_)));
}

glm::fmat4 Camera::GetProjectionMatrix(float ratio) const {
	return glm::perspective<float>(glm::radians<float>(fov_), ratio, near_, far_);
}

void Camera::Rotate(float delta_yaw, float delta_pitch) {
	yaw_ += glm::radians<float>(kMouseSensitivity * delta_yaw);
	pitch_ += glm::radians<float>(kMouseSensitivity * delta_pitch);
	constexpr float lower_bound = -glm::pi<float>() / 2.0f + 0.1f;
	constexpr float upper_bound = glm::pi<float>() / 2.0f - 0.1f;
	pitch_ = glm::clamp<float>(pitch_, lower_bound, upper_bound);
	UpdateVectors();
}

void Camera::HandleMouseCursor(float xpos, float ypos) {
	static glm::fvec2 last_pos{ xpos, ypos };
	Rotate(last_pos.x - xpos, last_pos.y - ypos);
	last_pos = std::move(glm::fvec2{ xpos, ypos });
}

FPSCamera::FPSCamera(glm::fvec3 position, float yaw, float pitch) {
	position_ = position;
	yaw_ = yaw;
	pitch_ = pitch;
}

void FPSCamera::Move(const glm::fvec3& delta) {
	position_ += delta;
	UpdateVectors();
}

void FPSCamera::HandleKeyboard(const CameraMovement &m, float dt) {
	switch (m) {
	case CameraMovement::FORWARD:
		Move(kMoveSpeed * dt * GetLook());
		break;
	case CameraMovement::BACKWARD:
		Move(kMoveSpeed * dt * -GetLook());
		break;
	case CameraMovement::RIGHT:
		Move(kMoveSpeed * dt * GetRight());
		break;
	case CameraMovement::LEFT:
		Move(kMoveSpeed * dt * -GetRight());
		break;
	}
}

void FPSCamera::HandleScroll(float delta_scroll) {
	float fov = GetFov() + delta_scroll * kZoomSensitivity;
	SetFov(glm::clamp<float>(fov, kMinFOV, kMaxFOV));
}

void FPSCamera::UpdateVectors() {
	glm::fvec3 look;
	look.x = cosf(pitch_) * sinf(yaw_);
	look.y = sinf(pitch_);
	look.z = cosf(pitch_) * cosf(yaw_);

	look_ = glm::normalize<float>(look);
	right_ = glm::normalize<float>(glm::cross<float>(look_, kWorldUp));
	up_ = glm::normalize<float>(glm::cross<float>(right_, look_));

	target_ = position_ + look_;
}

void OrbitCamera::SetLookAt(const glm::fvec3& target) {
	target_ = target;
	UpdateVectors();
}

void OrbitCamera::SetRadius(float delta) {
	radius_ += delta;
	radius_ = glm::clamp<float>(radius_, kMinRadius, kMaxRadius);
	UpdateVectors();
}

void OrbitCamera::HandleScroll(float delta_scroll) {
	SetRadius(kZoomSensitivity * delta_scroll);
}

void OrbitCamera::HandleKeyboard(const CameraMovement &m, float dt) {}

void OrbitCamera::UpdateVectors() {
	position_.x = target_.x + radius_ * cosf(pitch_) * sinf(yaw_);
	position_.y = target_.y + radius_ * sinf(pitch_);
	position_.z = target_.z + radius_ * cosf(pitch_) * cosf(yaw_);

	glm::fvec3 look{ target_ - position_ };
	glm::fvec3 right{ glm::normalize<float>(glm::cross<float>(look, kWorldUp)) };
	up_ = glm::normalize<float>(glm::cross<float>(right, look));
}
