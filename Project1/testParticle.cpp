#include "testParticle.h"
#include <NY_random.h>

void BomParticle::Init()
{
	//開始座標
	pos = spos;

	//終了フレーム設定
	endFrame = 60;

	//速度設定
	float xvel = NY_random::floatrand_sl(3.0f, -3.0f);
	float yvel = NY_random::floatrand_sl(3.0f, -3.0f);
	float zvel = NY_random::floatrand_sl(3.0f, -3.0f);

	vel = RVector3(xvel, yvel, zvel);
}

void BomParticle::Update()
{
	//速度を毎フレーム加算
	pos += vel;
}

ParticlePrototype *BomParticle::clone(RVector3 startPos)
{
	return new BomParticle(startPos);
}
