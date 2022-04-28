#include "goalParticle.h"
#include "NY_random.h"

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
}

GoalParticle::~GoalParticle()
{
}

void GoalParticle::Init(ParticleGrainState* pgState, const float spawnRange, const size_t& spawnCount)
{
	for (size_t i = 0; i < spawnCount; i++)
	{
		pgState->position.x += NY_random::floatrand_sl(-spawnRange, +spawnRange);
		pgState->position.y += NY_random::floatrand_sl(-spawnRange, +spawnRange);
		pgState->position.z += NY_random::floatrand_sl(-spawnRange, +spawnRange);
		particleManager->Add(*pgState);
	}
}

void GoalParticle::Init(const RVector3& pos, const float spawnRange, const size_t& spawnCount, const RVector3& vel, const RVector3& accel, const int aliveTime, const XMFLOAT4& color, const float scale)
{
	for (size_t i = 0; i < spawnCount; i++)
	{
		static ParticleGrainState pgState{};
		pgState.position.x = pos.x + NY_random::floatrand_sl(+spawnRange, -spawnRange);
		pgState.position.y = pos.y + NY_random::floatrand_sl(+spawnRange, -spawnRange);
		pgState.position.z = pos.z + NY_random::floatrand_sl(+spawnRange, -spawnRange);
		pgState.vel = vel;
		pgState.acc = accel;
		pgState.color_start = color;
		pgState.color_end = color;
		pgState.scale_start = scale;
		pgState.scale_end = scale;
		pgState.aliveTime = aliveTime;
		particleManager->Add(pgState);
	}
}

void GoalParticle::Create()
{
	if (particleSprite.spdata->size.x * particleSprite.spdata->size.y == 0)
	{
		particleHandle = TexManager::LoadTexture("./Resources/GoalParticle.png");
		particleSprite.Create(particleHandle);
	}

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
