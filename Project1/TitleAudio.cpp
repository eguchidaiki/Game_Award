#include "TitleAudio.h"

TitleAudio::TitleAudio()
{

}

TitleAudio::~TitleAudio()
{

}

void TitleAudio::Init()
{
	titleSound = Audio::LoadSound_wav("Resources/sound/BGM/bgm01.wav");
	Audio::SetPlayRoopmode(titleSound, 255);
	isPlayed = false;
}

void TitleAudio::Play()
{
	if (isPlayed) {
		return;
	}
	Audio::PlayLoadedSound(titleSound);
}

void TitleAudio::Stop()
{
	Audio::StopLoadedSound(titleSound);
	isPlayed = false;
}
