#pragma once
#include <Audio.h>

class TitleAudio
{
public:
	TitleAudio();
	~TitleAudio();

	static TitleAudio* Get() {
		static TitleAudio ins;
		return &ins;
	}

	void Init();
	void Play();
	void Stop();

	bool isPlayed = false;
	SoundData titleSound;
};

