#ifndef AUDIO_HPP_
#define AUDIO_HPP_

#include <memory>
#include <SFML/Audio.hpp>

constexpr float kMaxVolume{ 100.0f };
constexpr float kMinVolume{ 0.0f };

class Audio {
public:
	virtual bool Open(const std::string& file) = 0;
	virtual void Play(bool loop = false) = 0;
	virtual void Stop() = 0;
	virtual void Volume(float volume) = 0;
	virtual void Pitch(float pitch) = 0;

	virtual ~Audio() {}
protected:
	float Clamp(float value, float max, float min) {
		if (value > max) return max;
		else if (value < min) return min;
		return value;
	}
};

#endif // AUDIO_HPP_
