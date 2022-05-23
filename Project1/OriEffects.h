#pragma once

#include <ParticleManager2D.h>
#include "Ori_StarParticle.h"
#include "OriParticle.h"

class OriEffects
{
public:
	OriEffects();
	~OriEffects();

	//状態の初期化
	void Init();

	//エフェクト再生
	void Play(float centerPosX,float centerPosY);

	void Update();

	void Draw();

	void Finalize();

private:
	//エミッター
	ParticleManager2D starParticle;
	ParticleManager2D oriParticle;
	//テクスチャ
	UINT starGraph;
	UINT oriGraph;
	//プロトタイプ
	Ori_StarParticle* starProto;
	OriParticle* oriProto;




};

