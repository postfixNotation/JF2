#ifndef MUSIC_HPP_
#define MUSIC_HPP_

#include <audio.hpp>

class Music final : public Audio {
public:
	Music() {
		handle_ = std::unique_ptr<sf::Music>(new sf::Music());
	}

	bool Open(const std::string& file) final override { return (handle_->openFromFile(file)); }
	void Play(bool loop) final override {
		handle_->setLoop(loop);
		handle_->play();
	}
	void Stop() final override { handle_->stop(); }
	void Volume(float volume) final override {
		handle_->setVolume(Clamp(volume, kMaxVolume, kMinVolume));
	}
	void Pitch(float pitch) final override { handle_->setPitch(pitch); }

private:
	std::unique_ptr<sf::Music> handle_;

};

#endif // MUSIC_HPP_
