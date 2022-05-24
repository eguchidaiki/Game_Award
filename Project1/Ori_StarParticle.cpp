#include "Ori_StarParticle.h"
#include <NY_random.h>

Ori_StarParticle::Ori_StarParticle()
{
}

Ori_StarParticle::~Ori_StarParticle()
{
}

void Ori_StarParticle::Init()
{
	color.w = 1.0f;
	e_color = { 1,1,1,0.0f };

	rot = NY_random::floatrand_sl(360.0f, 0.0f);

	float power = NY_random::floatrand_sl(20.0f, 10.0f);
	vel.y = sinf((rot - 90.0f) * (3.14 / 180.0f));
	vel.x = cosf((rot - 90.0f) * (3.14 / 180.0f));
	vel.z = 0.0f;

	vel *= power;

	acc.x = vel.x / 40.0f;
	acc.y = vel.y / 40.0f;

	//初期座標を中心から一定距離離す
	pos += vel * 10.0f;

	scale = 1.0f;
	s_scale = 3.0f;
	e_scale = 3.0f;

	drawsize.x = 16.0f;
	drawsize.y = 16.0f;

	endFrame = 40;
}

void Ori_StarParticle::Update()
{
	pos += vel;
	vel -= acc;

	float rate = static_cast<float>(nowFrame) / static_cast<float>(endFrame);

	scale = s_scale + (e_scale - s_scale) * rate;
	color = s_color + (e_color - s_color) * rate;
}

ParticlePrototype2D* Ori_StarParticle::clone()
{
	Ori_StarParticle* p = new Ori_StarParticle;
	//座標をクローン元パーティクルの座標に合わせる
	p->pos = this->pos;
	return p;
}
