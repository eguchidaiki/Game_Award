#include "goalParticle.h"

GoalParticle::ParticleClass::ParticleClass()
{
}

GoalParticle::ParticleClass::~ParticleClass()
{
}

void GoalParticle::ParticleClass::Init()
{
}

void GoalParticle::ParticleClass::Update()
{
}

ParticlePrototype* GoalParticle::ParticleClass::clone(RVector3 start)
{
	return nullptr;
}

GoalParticle::GoalParticle()
{
	Create();
}

GoalParticle::~GoalParticle()
{
}

void GoalParticle::Init(ParticleGrainState pgState)
{
	particleManager->Add(pgState);
}

void GoalParticle::Init(RVector3 pos, RVector3 vel, RVector3 accel, XMFLOAT4 color, float scale, int aliveTime)
{
	static ParticleGrainState pgState{};
	pgState.position = pos;
	pgState.vel = vel;
	pgState.acc = accel;
	pgState.color_start = color;
	pgState.color_end = color;
	pgState.scale_start = scale;
	pgState.scale_end = scale;
	particleManager->Add(pgState);
}

void GoalParticle::Create()
{
	particleHandle = TexManager::LoadTexture("./Resources/GoalParticle.png");
	particleSprite.Create(particleHandle);

	particleManager = ParticleManager::Create();
	//particleManager->Prototype_Set(new GoalParticle::ParticleClass());
}

void GoalParticle::Update()
{
	particleManager->Update();
}

void GoalParticle::Draw()
{
	particleManager->Draw(particleHandle);
}
