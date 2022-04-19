#include "RVector.h"

#include "Raki_WinAPI.h"
#include "NY_Camera.h"

bool RV3Colider::ColisionSphereToPlane(const Sphere &sphere, const Plane &plane, RVector3 *coliPos)
{
	//���ʂƋ��̒��S�Ƃ̋��������߂�
	float dist = dot(sphere.center, plane.normal) - plane.distance;
	//�����̐�Βl�����a���傫����΂������ĂȂ�
	if (fabsf(dist) > sphere.rad) { return false; }
	//�������Ă���ꍇ�͋^����_���v�Z
	*coliPos = -dist * plane.normal + sphere.center;
	//�Փ�
	return true;
}

RVector3 RV3Colider::CalcScreen2World(const XMFLOAT2& scrPos, float fz)
{
	XMVECTOR pos;
	//�ˉe�ϊ��s��ƃr���[�|�[�g�s��̋t�s����i�[����ϐ�
	XMMATRIX InvPrj, InvVP, InvV;
	//�e�s��̋t�s����o��
	InvPrj = XMMatrixInverse(nullptr, camera->GetMatrixProjection());
	//�r���[�|�[�g�s��͂��Ƃ͂Ȃ��H�̂ł����Œ�`���ċt�s����o��
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
