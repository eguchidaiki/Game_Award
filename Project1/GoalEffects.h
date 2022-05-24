#pragma once
#include "GoalPaperRain.h"
#include "GoalCracker.h"

class GoalEffects
{
public:
	GoalEffects();
	~GoalEffects();

	void Init();

	void Play();

	void Stop();

	void Update();

	void Draw();

private:
	//プロトタイプ
	GoalPaperRain* rainProto;
	//パーティクル
	ParticleManager2D rainParticle;
	//テクスチャ
	UINT prainGraph;

	enum NowEffectState
	{
		EFFECT_STANDBY,
		EFFECT_PLAYING,
	};
	NowEffectState state = EFFECT_STANDBY;

};

