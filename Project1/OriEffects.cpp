#include "OriEffects.h"

OriEffects::OriEffects()
{
}

OriEffects::~OriEffects()
{
}

void OriEffects::Init()
{
	starGraph = TexManager::LoadTexture("Resources/OriStar2.png");
	oriGraph = TexManager::LoadTexture("Resources/OriFire.png");

	starProto = new Ori_StarParticle;
	oriProto = new OriParticle;

	starParticle.Initialize(starGraph);
	starParticle.Prototype_Set(starProto);

	oriParticle.Initialize(oriGraph);
	oriParticle.Prototype_Set(oriProto);

}

void OriEffects::Play(float centerPosX, float centerPosY)
{
	starParticle.GetPrototypeInstance()->pos = RVector3(centerPosX, centerPosY, 0.0f);
	oriParticle.GetPrototypeInstance()->pos = RVector3(centerPosX, centerPosY, 0.0f);

	for (int i = 0; i < 16; i++) {
		oriParticle.Prototype_Add();

		if (i < 4) {
			starParticle.Prototype_Add();
		}
	}
}

void OriEffects::Update()
{
	starParticle.Prototype_Update();
	oriParticle.Prototype_Update();
}

void OriEffects::Draw()
{
	Update();

	starParticle.Prototype_Draw();
	oriParticle.Prototype_Draw();
}

void OriEffects::Finalize()
{
}
