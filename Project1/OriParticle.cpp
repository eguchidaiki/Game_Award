#include "OriParticle.h"

#include <RVector.h>

OriParticle::OriParticle()
{
}

OriParticle::~OriParticle()
{
}

void OriParticle::Init()
{
	//色
	color.w = 1.0f;

	e_color = { 1,1,1,0.1f };

	//パーティクル角度
	rot = NY_random::floatrand_sl(360.0f, 0.0f);

	//速度ベクトル補正
	float power = NY_random::floatrand_sl(20.0f, 10.0f);
	vel.y = sinf((rot-90.0f) * (3.14 / 180.0f));
	vel.x = cosf((rot-90.0f) * (3.14 / 180.0f));
	vel.z = 0.0f;
	vel *= power;

	//加速度ベクトル設定
	acc.x = vel.x / 40.0f;
	acc.y = vel.y / 40.0f;

	//初期座標を中心から一定距離離す
	pos += vel * 10.0f;

	scale = 1.0f;
	s_scale = 3.0f;
	e_scale = 3.0f;

	drawsize.x = 6.0f;
	drawsize.y = 18.0f;

	endFrame = 20;
}

void OriParticle::Update()
{
	pos += vel;
	vel -= acc;

	float rate = static_cast<float>(nowFrame) / static_cast<float>(endFrame);
	scale = s_scale + (e_scale - s_scale) * rate;
	color = s_color + (e_color - s_color) * rate;
}

ParticlePrototype2D* OriParticle::clone()
{
	OriParticle* p = new OriParticle;
	//座標をクローン元パーティクルの座標に合わせる
	p->pos = this->pos;
	return p;
}
