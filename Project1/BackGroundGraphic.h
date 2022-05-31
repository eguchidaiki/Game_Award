#pragma once

#include <array>
#include <Sprite.h>
#include <ParticleManager2D.h>

class CloudProto :public ParticlePrototype2D {
	void Init() override;

	void Update() override;

	ParticlePrototype2D* clone() override;

private: 

};

class BackGroundGraphic
{
public:

	//”wŒi
	Sprite backGroundSprite;
	Sprite birdSprite;
	//‰_
	std::array<ParticleManager2D, 3> cloudParticle;

	//ƒeƒNƒXƒ`ƒƒƒnƒ“ƒhƒ‹
	static UINT backGraph;
	static std::array<UINT,3> cloudGraph;
	static UINT birdGraph;

	BackGroundGraphic();
	~BackGroundGraphic();

	void Init();

	void Update();

	void Draw();

	//’¹
	float bird_center_x,	bird_center_y;
	float bird_width,		bird_height;
	
	const float bird_vel_x = 1.2f;
	int			frame = 0;

	const int CLOUD_SPAWNFRAME_01 = 240;
	const int CLOUD_SPAWNFRAME_02 = 480;
	const int CLOUD_SPAWNFRAME_03 = 720;
};

