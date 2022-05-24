#pragma once
#include <ParticleManager2D.h>
#include <NY_random.h>

class OriParticle : public ParticlePrototype2D
{
private:

public:
	OriParticle();
	~OriParticle();

	void Init() override;

	void Update() override;

	ParticlePrototype2D* clone() override;

private:



};

