#include "RVector.h"

#include "Raki_WinAPI.h"
#include "NY_Camera.h"

bool RV3Colider::ColisionSphereToPlane(const Sphere &sphere, const Plane &plane, RVector3 *coliPos)
{
	//平面と球の中心との距離を求める
	float dist = dot(sphere.center, plane.normal) - plane.distance;
	//距離の絶対値が半径より大きければあたってない
	if (fabsf(dist) > sphere.rad) { return false; }
	//あたっている場合は疑似交点を計算
	*coliPos = -dist * plane.normal + sphere.center;
	//衝突
	return true;
}

RVector3 RV3Colider::CalcScreen2World(const XMFLOAT2& scrPos, float fz)
{
	XMVECTOR pos;
	//射影変換行列とビューポート行列の逆行列を格納する変数
	XMMATRIX InvPrj, InvVP, InvV;
	//各行列の逆行列を出す
	InvPrj = XMMatrixInverse(nullptr, camera->GetMatrixProjection());
	//ビューポート行列はもとはない？のでここで定義して逆行列を出す
	InvVP = XMMatrixIdentity();
	InvVP.r[0].m128_f32[0] = Raki_WinAPI::window_width / 2.0f;
	InvVP.r[1].m128_f32[1] = -Raki_WinAPI::window_height / 2.0f;
	InvVP.r[3].m128_f32[0] = Raki_WinAPI::window_width / 2.0f;
	InvVP.r[3].m128_f32[1] = Raki_WinAPI::window_height / 2.0f;
	InvVP = XMMatrixInverse(nullptr, InvVP);

	InvV = XMMatrixInverse(nullptr, camera->GetMatrixView());

	XMMATRIX inverce = InvVP * InvPrj * InvV;
	XMVECTOR scr = { scrPos.x,scrPos.y,fz };

	pos = XMVector3TransformCoord(scr, inverce);

	RVector3 returnpos = { pos.m128_f32[0],pos.m128_f32[1],pos.m128_f32[2] };
	return returnpos;
}

const RVector3 Rv3Ease::lerp(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return start * (1.0f - t) + end * t;
}

const RVector3 Rv3Ease::InQuad(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return start * (1.0f - t * t) + end * (float)pow(t, 2);
}

const RVector3 Rv3Ease::OutQuad(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return start * (1.0f - (1.0f - (1.0f - t) * (1.0f - t))) + end * (1.0f - (1.0f - t) * (1.0f - t));
}

const RVector3 Rv3Ease::InOutQuad(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return t < 0.5f ? (start * (1.0f - t)) + end * (t * t) : start * (1.0f - t) + end * (1 - (float)pow(-2 * t + 2, 2) / 2);
}
