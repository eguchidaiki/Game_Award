#pragma once
#include <vector>
#include<RVector.h>
//#include "Vector3.h"

class Easing
{
public: //静的メンバ関数
	static float easeOut(const float start, const float end, const float time);
	static RVector3 SplineCurve(const std::vector<RVector3>& points, const size_t& startIndex, const float time);
	static RVector3 SplineLoop(const std::vector<RVector3>& points, const size_t& startIndex, const float time);

public: //メンバ関数
	Easing();
	~Easing();

public: //メンバ変数
	float maxTime;
	float timeRate;
	float addTime;

	bool isMove;
	bool isOldMove;

	size_t splineIndex;
};
