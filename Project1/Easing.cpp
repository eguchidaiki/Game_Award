#include "Easing.h"
#include "Vector3.h"

Easing::Easing() :
	maxTime(2.0f),
	timeRate(0.0f),
	addTime(0.1f),
	isMove(false),
	splineIndex(0)
{
}

Easing::~Easing()
{
}

float Easing::easeOut(const float start, const float end, const float time)
{
	float position = time * (2 - time);
	return start * (1.0f - position) + end * position;
}

Vector3 Easing::SplineCurve(const std::vector<Vector3>& points, const size_t& startIndex, const float time)
{
	// ï‚ä‘Ç∑Ç◊Ç´ì_ÇÃêî
	size_t n = points.size();
	static Vector3 p0, p1, p2, p3;

	if (startIndex > n)
	{
		return points[n - 1];
	}
	if (startIndex < 0)
	{
		return points[0];
	}

	if (startIndex == 0)
	{
		p0 = points[0];
	}
	else
	{
		p0 = points[startIndex - 1];
	}
	p1 = points[startIndex];
	p2 = points[startIndex + 1];
	if (startIndex == n - 2)
	{
		p3 = points[n - 1];
	}
	else
	{
		p3 = points[startIndex + 2];
	}

	Vector3 position =
		0.5 * (2 * p1 + (-p0 + p2) * time + (2 * p0 - 5 * p1 + 4 * p2 - p3) * time * time + (-p0 + 3 * p1 - 3 * p2 + p3) * time * time * time);

	return position;
}

Vector3 Easing::SplineLoop(const std::vector<Vector3>& points, const size_t& startIndex, const float time)
{
	// ï‚ä‘Ç∑Ç◊Ç´ì_ÇÃêî
	size_t n = points.size();
	static Vector3 p[4];

	for (size_t i = 0; i < 4; i++)
	{
		if (static_cast<int>(startIndex + i - 1) < 0)
		{
			p[i] = points[startIndex + i - 1 + n];
		}
		else if (static_cast<int>(startIndex + i - 1) >= n)
		{
			p[i] = points[(startIndex + i - 1) - n];
		}
		else
		{
			p[i] = points[startIndex + i - 1];
		}
	}

	Vector3 position =
		0.5 * (2 * p[1] + (-p[0] + p[2]) * time + (2 * p[0] - 5 * p[1] + 4 * p[2] - p[3]) * time * time + (-p[0] + 3 * p[1] - 3 * p[2] + p[3]) * time * time * time);

	return position;
}
