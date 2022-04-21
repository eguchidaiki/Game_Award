#pragma once
#include "PlayerBody.h"
#include "PlayerFoot.h"
#include <RVector.h>
#include <TexManager.h>
#include "Sprite.h"
#include "goalParticle.h"

class Player final
{
public: //繧ｷ繝ｳ繧ｰ繝ｫ繝医Φ蛹
	static Player* Get();
private:
	Player();
	Player(const Player&) = delete;
	~Player();
	Player operator=(const Player&) = delete;

public: //繝｡繝ｳ繝宣未謨ｰ
	// 蛻晄悄蛹
	void Init();
	// 更新
	void Update(Stage& stage, int offsetX, int offsetY);
	// 描画
	void Draw(int offsetX, int offsetY);

	/// <summary>
	/// 縺ｩ縺ｮ菴薙ｒ譛牙柑蛹悶☆繧九°
	/// </summary>
	/// <param name="one">body_one</param>
	/// <param name="two">body_two</param>
	/// <param name="three">body_three</param>
	void bodysetup(bool one, int one_type, bool two, int two_type, bool three, int three_type, bool four, int four_type);
	void bodysetup(const unsigned char foldCount[4]);

	//鬘斐�蠖薙◆繧雁愛螳
	void IsHitPlayerBody(Stage& stage);
	//謚ｼ縺怜�縺怜�逅(菴ｿ縺｣縺ｦ縺ｪ縺)
	void ExtrudePlayer(RVector3 ExtrudePos, float ExtrudeDis, BodyType ExtrudeType);
	//蜷�ｽ薙�鬘斐�關ｽ荳句愛螳
	bool IsFall();
	//蜷�ｽ薙�迥ｶ諷九ｒ驟榊�縺ｫ繧ｻ繝�ヨ(繧ｻ繝�ヨ縺吶ｋ鬆�分縺ｯstage.FoldAndOpen縺ｮ蛻､螳夐)
	void SetBodyStatus(PlayerBody* arrangement, int size);

public: //繝｡繝ｳ繝仙､画焚
	//蠎翫�鬮倥＆
	float FloorHeight;

	//鬘斐�荳ｭ蠢�ｺｧ讓
	RVector3 CenterPosition;

	//蜷代＞縺ｦ縺�ｋ譁ｹ蜷
	bool IsLeft;
	bool IsRight;

	//歩くかどうか
	bool IsWalk = false;

	//マウスドラッグで折るかどうか
	bool IsDragFold = false;
	XMFLOAT2 DragDis = {};

	//体(折るほう)
	PlayerBody Body_One;
	PlayerBody Body_Two;
	PlayerBody Body_Three;
	PlayerBody Body_Four;

	//body_two繧貞━蜈育噪縺ｫ髢九￥縺
	bool IsOpenTwo;

	//縺ｩ縺ｮ譁ｹ蜷代ｒ謚倥ｋ縺(4譁ｹ蜷)
	bool IsLeftFold;
	bool IsUpFold;
	bool IsRightFold;
	bool IsDownFold;

	//縺ｩ縺ｮ譁ｹ蜷代ｒ謚倥ｋ縺(4譁ｹ蜷)
	bool IsLeftOpen;
	bool IsUpOpen;
	bool IsRightOpen;
	bool IsDownOpen;

	//髢九￥縺ｾ縺ｧ縺ｫ謨ｰ繝輔Ξ繝ｼ繝遨ｺ縺代ｋ逕ｨ
	int OpenCount = 0;

	//繧ｫ繧ｦ繝ｳ繝医ｒ蟋九ａ繧九°縺ｩ縺�°
	bool IsOpenCountStart = false;

	//遘ｻ蜍暮溷ｺｦ
	float SideMoveSpeed = 3.0f;

	//繧ｸ繝｣繝ｳ繝
	bool IsJump;
	float JumpSpeed;
	float FallSpeed;

	//關ｽ荳句愛螳(鬘斐�縺ｿ
	bool IsFaceFall;

	//菴薙→鬘斐☆縺ｹ縺ｦ繧定��縺励◆關ｽ荳句愛螳
	bool IsAllFall;

	//關ｽ荳倶ｸｭ繝ｻ繧ｸ繝｣繝ｳ繝嶺ｸｭ縺ｫ繧ｸ繝｣繝ｳ繝怜�蜉帙′蜈･縺｣縺ｦ縺�ｋ縺九←縺�°
	bool IsInputjump;

	//縺ｩ繧後°荳縺､縺ｧ繧ゆｽ薙ｒ蜍輔°縺励※縺�◆繧液rue
	bool Player_IsAction;

	//逕ｻ蜒上ワ繝ｳ繝峨Ν(鬘)
	UINT FaceHandle[2];

	//繧ｴ繝ｼ繝ｫ縺ｫ隗ｦ繧後※縺�ｋ縺九←縺�°
	bool IsGoal;

	//繝悶Ο繝�け縺ｫ蠖薙◆縺｣縺ｦ縺�ｋ縺九←縺�°
	bool IsColide;

	//荳九↓菴薙′縺ゅｋ縺九←縺�°
	bool IsDownBody;

	//雜ｳ
	PlayerFoot leg;

	//謠冗判逕ｨ繧ｹ繝励Λ繧､繝
	Sprite PlayerSprite;
	Sprite PlayerSpriteAction;

	//マウスのクリック座標
	XMFLOAT2 PressPos = { 0,0 };
	XMFLOAT2 ReleasePos = { 0,0 };

	// 繧ｴ繝ｼ繝ｫ縺ｮ繝代�繝�ぅ繧ｯ繝ｫ
	GoalParticle goalParticle;

};
