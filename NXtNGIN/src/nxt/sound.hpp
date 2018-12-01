#ifndef SOUND_HPP_
#define SOUND_HPP_

#include "audio.hpp"

namespace nxt {
	class Sound final : public Audio {
	public:
		Sound() {
			handle_ = std::unique_ptr<sf::Sound>(new sf::Sound());
		}

		bool Open(const std::string& file) final override {
			buffer_ = std::unique_ptr<sf::SoundBuffer>(new sf::SoundBuffer());
			return (buffer_->loadFromFile(file));
		}
		void Play(bool loop) final override {
			handle_->setLoop(loop);
			handle_->setBuffer(*buffer_);
			handle_->play();
		}
		void Stop() final override { handle_->stop(); }
		void Volume(float volume) final override {
			handle_->setVolume(Clamp(volume, kMaxVolume, kMinVolume));
		}
		void Pitch(float pitch) final override { handle_->setPitch(pitch); }

	private:
		std::unique_ptr<sf::Sound> handle_;
		std::unique_ptr<sf::SoundBuffer> buffer_;
	};
}

#endif // SOUND_HPP_
