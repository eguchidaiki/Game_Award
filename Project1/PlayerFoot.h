#pragma once
#include "PlayerBody.h"
//#include "Vector3.h"
#include "Easing.h"
#include "Sprite.h"
#include<RVector.h>

class PlayerBody;

class PlayerFoot
{
public: //メンバ関数
	PlayerFoot();
	~PlayerFoot();

	void Init();
	void Set();
	void Update(RVector3& DownPos, bool IsDownBody, int BodyDis);
	void Draw(int offsetX, int offsetY, bool isleft, bool isright);

public: //メンバ変数
	RVector3 FootLeftUpPosition;
	Easing ease;

	bool FootIsAction;
	bool IsFootUp;

	//画像ハンドル
	UINT FootHandle;
	Sprite FootSprite;
};