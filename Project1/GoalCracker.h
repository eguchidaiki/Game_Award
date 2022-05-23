#pragma once
#include <ParticleManager2D.h>

class GoalCracker : public ParticlePrototype2D
{
public:
	GoalCracker(){}
	~GoalCracker(){}

	void Init() override;

	void Update() override;

	ParticlePrototype2D* clone() override;

private:






};

