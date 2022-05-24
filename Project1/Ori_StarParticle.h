#pragma once
#include <ParticleManager2D.h>

class Ori_StarParticle : public ParticlePrototype2D
{
private:


public:
	Ori_StarParticle();
	~Ori_StarParticle();

	void Init() override;

	void Update() override;

	ParticlePrototype2D* clone() override;


private:


};

