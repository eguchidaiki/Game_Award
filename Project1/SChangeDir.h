#pragma once

#include <Sprite.h>
#include <array>

class SChangeDir final 
{
public:
	SChangeDir();
	~SChangeDir();


	static SChangeDir* Get() {
		static SChangeDir ins;
		return &ins;
	}

	//初期化
	void Init();

	//更新
	void Update();

	//全画面描画
	void Draw();

	void PlayChangingDirection();
	void PlayChangedDirection();

	bool isChangeActivate = false;
	bool isChangeingDirecting = false;
	bool isChangedDirecting = false;

	const int CHANGE_DIRECT_FRAME = 2;
	int frame = 0;

	std::array<Sprite, 21> changeSprite;
	int animeDisplayNum = 0;

};

