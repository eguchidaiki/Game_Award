#include "SChangeDir.h"

SChangeDir::SChangeDir()
{
	std::string resourcePass = "Resources/SceneChange/";
	for (int i = 0; i < changeSprite.size(); i++) {
		string number = std::to_string(i + 6);
		string name = ".png";
		string fullPath = resourcePass + number + name;
		changeSprite[i].Create(TexManager::LoadTexture(fullPath));
	}
}

SChangeDir::~SChangeDir()
{
}

void SChangeDir::Init()
{
	isChangeActivate = false;
	isChangeingDirecting = false;
	isChangedDirecting = false;
	frame = 0;
	animeDisplayNum = 0;
}

void SChangeDir::Update()
{
	if (isChangeingDirecting) {
		frame++;
		if (frame % CHANGE_DIRECT_FRAME == 0) {
			animeDisplayNum++;
		}
		if (animeDisplayNum == changeSprite.size() - 1) {
			Init();
			animeDisplayNum = changeSprite.size() - 1;
			isChangeActivate = true;
		}
	}
	else if (isChangedDirecting) {
		frame++;
		if (frame % CHANGE_DIRECT_FRAME == 0) {
			animeDisplayNum--;
		}
		if (animeDisplayNum < 0) {
			Init();
			//ƒV[ƒ“Ø‚è‘Ö‚¦Œã‚Ìê‡‚Í‰Šú‰»‚¾‚¯‚Å‚¢‚¢
		}
	}
}

void SChangeDir::Draw()
{
	if (isChangedDirecting || isChangeingDirecting || isChangeActivate) {
		changeSprite[animeDisplayNum].DrawExtendSprite(0, 0, 1280, 720);
		changeSprite[animeDisplayNum].Draw();
	}
}

void SChangeDir::PlayChangingDirection()
{
	Init();
	animeDisplayNum = 0;
	isChangeingDirecting = true;
}

void SChangeDir::PlayChangedDirection()
{
	Init();
	animeDisplayNum = changeSprite.size() - 1;
	isChangedDirecting = true;
}
