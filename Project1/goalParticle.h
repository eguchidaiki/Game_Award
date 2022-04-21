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

	void Init(ParticleGrainState pgState);
	void Init(RVector3 pos, RVector3 vel, RVector3 accel, XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, float scale = 1.0f, int aliveTime = 60);
	void Create();
	void Update();
	void Draw();

private: //メンバ変数
	ParticleManager* particleManager;

	UINT particleHandle;
	Sprite particleSprite;
};
