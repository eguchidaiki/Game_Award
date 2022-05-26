#include "GoalPaperRain.h"

#include <NY_random.h>

void GoalPaperRain::Init()
{
	//画面上のランダムな位置に設定
	pos.x = NY_random::floatrand_sl(1280, 0);
	//yは画面外
	pos.y = NY_random::floatrand_sl(0, -50);

	//xがどのあたりから出現したかによって速度ベクトルを設定する処理

	float spawnPosRate = pos.x / 1280.0f; //どのあたりかを割合で算出
	float veldig = 180.0f * spawnPosRate; //上から進入角度を求める

	vel.x = cosf(veldig - 90.0f * 3.14f / 180.0f);
	vel.y = sinf(veldig - 90.0f * 3.14f / 180.0f);
	vel.z = 0.0f;

	//速度補正
	float power = NY_random::floatrand_sl(5.0f, 1.0f);
	vel *= power;

	//描画サイズ設定
	drawsize.x = 8.0f;
	drawsize.y = 16.0f;

	//回転速度
	radacc = NY_random::floatrand_sl(6.0f, -6.0f);

	//色
	color = { NY_random::floatrand_sl(1.0f, 0.0f) ,NY_random::floatrand_sl(1.0f, 0.0f) ,NY_random::floatrand_sl(1.0f, 0.0f) ,1.0f };

	endFrame = 180.0f;
}

void GoalPaperRain::Update()
{
	//座標
	pos += vel;
	//回転角
	rot += radacc;
}

ParticlePrototype2D* GoalPaperRain::clone()
{
	GoalPaperRain* p = new GoalPaperRain;
	return p;
}
