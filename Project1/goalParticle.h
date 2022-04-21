#pragma once
#include "RVector.h"
#include "ParticleManager.h"
#include "Sprite.h"

class GoalParticle
{
public: //サブクラス
	class ParticleClass : public ParticlePrototype
	{
	public:
		//開始位置保存用
		RVector3 spos;

		ParticleClass();
		~ParticleClass();

		void Init();
		void Update();
		ParticlePrototype* clone(RVector3 start);
	};

public: //メンバ関数
	GoalParticle();
	~GoalParticle();

	void Init(ParticleGrainState* pgState, const float spawnRange, const size_t& spawnCount);
	void Init(const RVector3& pos, const float spawnRange, const size_t& spawnCount,
		const RVector3& vel, const RVector3& accel, const int aliveTime = 60, const XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, const float scale = 1.0f);
	void Create();
	void Update();
	void Draw();

private: //メンバ変数
	ParticleManager* particleManager;

	UINT particleHandle;
	Sprite particleSprite;
};
