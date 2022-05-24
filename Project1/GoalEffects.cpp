#include "GoalEffects.h"

GoalEffects::GoalEffects()
{
}

GoalEffects::~GoalEffects()
{
}

void GoalEffects::Init()
{
	rainProto = new GoalPaperRain;
	prainGraph = TexManager::LoadTexture("Resources/WhitePixle.png");
	rainParticle.Initialize(prainGraph);
	rainParticle.Prototype_Set(rainProto);
}

void GoalEffects::Play()
{
	if (state == EFFECT_STANDBY) { state = EFFECT_PLAYING; }
}

void GoalEffects::Stop()
{
	state = EFFECT_STANDBY;
}

void GoalEffects::Update()
{
	//毎フレームパーティクル生成
	for (int i = 0; i < 6; i++) {
		rainParticle.Prototype_Add();
	}
}

void GoalEffects::Draw()
{
	if (state != EFFECT_PLAYING) { return; }

	Update();

	rainParticle.Prototype_Draw();

}