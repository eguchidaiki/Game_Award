#pragma once
#include <ParticleManager2D.h>

class GoalPaperRain : public ParticlePrototype2D
{
public:
	GoalPaperRain(){}
	~GoalPaperRain(){}

	void Init() override;

	void Update() override;

	ParticlePrototype2D* clone() override;

private:
	float radacc = 0.0f;

};

