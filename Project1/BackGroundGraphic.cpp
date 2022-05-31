#include "BackGroundGraphic.h"

#include <NY_random.h>

UINT BackGroundGraphic::backGraph		= -1;
UINT BackGroundGraphic::birdGraph		= -1;
std::array<UINT, 3> BackGroundGraphic::cloudGraph = { -1,-1,-1 };

void CloudProto::Init()
{
	//座標設定
	pos.x = NY_random::intrand_sl(1280, 1280 / 2);
	pos.y = NY_random::intrand_sl(720 / 2, 720 / 10);
	//開始アルファ設定
	s_color = DirectX::XMFLOAT4(1, 1, 1, 0);
	e_color = DirectX::XMFLOAT4(1, 1, 1, 1);
	//寿命設定
	endFrame = NY_random::intrand_sl(1200, 600);
	//左方向ベクトル設定
	vel.x = NY_random::floatrand_sl(1.0f, 0.1f);
}

void CloudProto::Update()
{
	pos += vel;

	//レート
	float rate = static_cast<float>(nowFrame) / static_cast<float>(endFrame);
	//色変換
	color = s_color + (e_color - s_color) * rate;

}

ParticlePrototype2D* CloudProto::clone()
{
	CloudProto* p = new CloudProto;
	//ランダム位置生成
	return p;
}

BackGroundGraphic::BackGroundGraphic()
{
	std::string path = "Resources/backGraph/";

	if (backGraph == -1) {
		std::string fullpath = path + "background/backGround01.png";
		backGraph = TexManager::LoadTexture(fullpath);
	}
	if (birdGraph == -1) {
		std::string fullpath = path + "bird/bird.png";
		birdGraph = TexManager::LoadTexture(fullpath);
	}
	for (int i = 0; i < 3;i++) {
		if (cloudGraph[i] == -1) {
			std::string fullpath = path + "cloud/cloud0";
			fullpath += std::to_string(i + 1);
			fullpath += ".png";

			cloudGraph[i] = TexManager::LoadTexture(fullpath);
		}
	}
	//背景
	backGroundSprite.Create(backGraph);
	//鳥
	birdSprite.CreateAndSetDivisionUVOffsets(7, 7, 1, 18, 18, birdGraph);
	//雲
	CloudProto* proto = new CloudProto;
	int num = 0;
	//パーティクルプロトタイプ初期化
	for (auto& p : cloudParticle) {
		p.Initialize(cloudGraph[num]);
		p.Prototype_Set(proto);
		num++;
	}
}

BackGroundGraphic::~BackGroundGraphic()
{

}

void BackGroundGraphic::Init()
{

}

void BackGroundGraphic::Update()
{

}

void BackGroundGraphic::Draw()
{



}
