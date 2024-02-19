#include "Collision.h"
#include <algorithm>
#include <array>

LineSegment::LineSegment(const Vector3& start, const Vector3& end)
	:mStart(start)
	, mEnd(end)
{
}

Vector3 LineSegment::PointOnSegment(float t) const
{
	return mStart + (mEnd - mStart) * t;
}

float LineSegment::MinDistSq(const Vector3& point) const
{
	//�x�N�g������������
	Vector3 ab = mEnd - mStart;
	Vector3 ba = -1.0f * ab;
	Vector3 ac = point - mStart;
	Vector3 bc = point - mEnd;

	//case1�@C��A�̑O�ɓ˂��o���Ă���
	if (Vector3::Dot(ab, ac) < 0.0f)
	{
		return ac.LengthSq();
	}

	//case2�@C��B�̂��Ƃɓ˂��o���Ă���
	else if (Vector3::Dot(ba, bc) < 0.0f)
	{
		return bc.LengthSq();
	}

	//case3�@C������Ɏˉe����
	else
	{
		float scalar = Vector3::Dot(ac, ab) / Vector3::Dot(ab, ab);
		Vector3 p = scalar * ab;

		return (ac - p).LengthSq();
	}
}

float LineSegment::MinDistSq(const LineSegment& s1, const LineSegment& s2)
{
	Vector3 u = s1.mEnd - s1.mStart;
	Vector3 v = s2.mEnd - s2.mStart;
	Vector3 w = s1.mStart - s2.mStart;

	float a = Vector3::Dot(u, u);
	float b = Vector3::Dot(u, v);
	float c = Vector3::Dot(v, v);
	float d = Vector3::Dot(u, w);
	float e = Vector3::Dot(v, w);
	float D = a * c - b * b;
	float sc, sN, sD = D;
	float tc, tN, tD = D;

	if (Math::NearZero(D))
	{
		sN = 0.0f;
		sD = 1.0f;
		tN = e;
		tD = c;
	}

	else
	{
		sN = (b * e - c * d);
		tN = (a * e - b * d);
		if (sN < 0.0f)
		{
			sN = 0.0f;
			tN = e;
			tD = c;
		}

		else if (sN > sD)
		{
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.0f)
	{
		tN = 0.0f;

		if (-d < 0.0f) sN = 0.0f;
		else if (-d > a) sN = sD;
		else
		{
			sN = -d;
			sD = a;
		}
	}

	else if (tN > tD)
	{
		tN = tD;
		if ((-d + b) < 0.0f)
			sN = 0;
		else if ((-d + b) > a)
			sN = sD;
		else
		{
			sN = (-d + b);
			sD = a;
		}
	}

	sc = (Math::NearZero(sN) ? 0.0f : sN / sD);
	tc = (Math::NearZero(tN) ? 0.0f : tN / tD);

	Vector3 dP = w + (sc * u) - (tc * v);

	return dP.LengthSq();
}

Plane::Plane(const Vector3& normal, float d)
	:mNormal(normal)
	,mD(d)
{
}

Plane::Plane(const Vector3& a, const Vector3& b, const Vector3& c)
{
	//ab��ac�̃x�N�g�����v�Z
	Vector3 ab = b - a;
	Vector3 ac = c - a;

	//�N���X�ςƐ��K���Ŗ@���𓾂�
	mNormal = Vector3::Cross(ab, ac);
	mNormal.Normalize();

	mD = -Vector3::Dot(a, mNormal);
}

float Plane::SignedDist(const Vector3& point) const
{
	return Vector3::Dot(point, mNormal) - mD;
}

Sphere::Sphere(const Vector3& center, float radius)
	:mCenter(center)
	,mRadius(radius)
{

}

bool Sphere::Contains(const Vector3& point) const
{
	//���S�Ɠ_�Ƃ̋����̓������߂�
	float distSq = (mCenter - point).LengthSq();
	return distSq <= (mRadius * mRadius);
}

AABB::AABB(const Vector3& min, const Vector3& max)
	:mMin(min)
	,mMax(max)
{

}

void AABB::UpdateMinMax(const Vector3& point)
{
	//�e������Ɨ����čX�V����
	mMin.x = Math::Min(mMin.x, point.x);
	mMin.y = Math::Min(mMin.y, point.y);
	mMin.z = Math::Min(mMin.z, point.z);

	mMax.x = Math::Max(mMax.x, point.x);
	mMax.y = Math::Max(mMax.y, point.y);
	mMax.z = Math::Max(mMax.z, point.z);
}

void AABB::Rotate(const Quaternion& q)
{
	//�{�b�N�X�̊p��8�_���\�z
	std::array<Vector3, 8> points;
	//�ŏ��̓_�́A��Ɋp�ɂ���
	points[0] = mMin;

	//2min 1max
	points[1] = Vector3(mMax.x, mMin.y, mMin.z);
	points[2] = Vector3(mMin.x, mMax.y, mMin.z);
	points[3] = Vector3(mMin.x, mMin.y, mMax.z);

	//2max 1min
	points[4] = Vector3(mMin.x, mMax.y, mMax.z);
	points[5] = Vector3(mMax.x, mMin.y, mMax.z);
	points[6] = Vector3(mMax.x, mMax.y, mMin.z);

	//�ő�̓_�́A�p�ɂ���
	points[7] = Vector3(mMax);

	//�ŏ��̓_����]
	Vector3 p = Vector3::Transform(points[0], q);

	//��]�����ŏ��̓_�ɍŏ�/�ő�����Z�b�g���܂�
	mMin = p;
	mMax = p;

	//�c��̓_�ɂ����min/max���X�V
	for (size_t i = 1; i < points.size(); i++)
	{
		p = Vector3::Transform(points[i], q);
		UpdateMinMax(p);
	}
}

bool AABB::Contains(const Vector3& point) const
{
	bool outside = point.x<mMin.x||
		point.y < mMin.y ||
		point.z < mMin.z ||
		point.x > mMax.x ||
		point.y > mMax.y ||
		point.z > mMax.z;
	
	//�ǂ���^�łȂ���Γ_�̓{�b�N�X�̒��ɂ���
	return !outside;
}

float AABB::MinDistSq(const Vector3& point) const
{
	//�e���̍����v�Z����
	float dx = Math::Max(mMin.x - point.x, 0.0f);
	dx = Math::Max(dx, point.x - mMax.x);
	float dy = Math::Max(mMin.y - point.y, 0.0f);
	dy = Math::Max(dy, point.y - mMax.y);
	float dz = Math::Max(mMin.z - point.z, 0.0f);
	dz = Math::Max(dy, point.z - mMax.z);
	//�����̓������߂�
	return dx * dx + dy * dy + dz * dz;
}

Capsule::Capsule(const Vector3& start, const Vector3& end, float radius)
	:mSegment(start, end)
	, mRadius(radius)
{
}

Vector3 Capsule::PointOnSegment(float t) const
{
	return mSegment.PointOnSegment(t);
}

bool Capsule::Contains(const Vector3& point) const
{
	//�_�Ɛ����Ƃ̍ŒZ�����̓������߂�
	float distSq = mSegment.MinDistSq(point);
	return distSq <= (mRadius * mRadius);
}

bool ConvexPolygon::Contains(const Vector2& point) const
{
	float sum = 0.0f;
	Vector2 a, b;
	for (size_t i = 0; i < mVertices.size() - 1; i++)
	{
		//�_�����1�̒��_�ւ̃x�N�g��
		a = mVertices[i] - point;
		a.Normalize();
		//�_�����2�̒��_�ւ̃x�N�g��
		b = mVertices[i + 1] - point;
		b.Normalize();
		//2�̃x�N�g���������p�x�����v�ɉ��Z
		sum += Math::Acos(Vector2::Dot(a, b));
	}
	//�Ō�ƍŏ��̒��_�̊ԂŊp�x���v�Z
	a = mVertices.back() - point;
	a.Normalize();
	b = mVertices.front() - point;
	b.Normalize();
	sum += Math::Acos(Vector2::Dot(a, b));
	//���v���A��2pai�ł����true��Ԃ�
	return Math::NearZero(sum - Math::TwoPi);
}

bool Intersect(const Sphere& a, const Sphere& b)
{
	float distSq = (a.mCenter - b.mCenter).LengthSq();
	float sumRadii = a.mRadius + b.mRadius;
	return distSq <= (sumRadii * sumRadii);
}

bool Intersect(const AABB& a, const AABB& b)
{
	bool no = a.mMax.x < b.mMin.x ||
		a.mMax.y < b.mMin.y ||
		a.mMax.z < b.mMin.z ||
		b.mMax.x < a.mMin.x ||
		b.mMax.y < a.mMin.y ||
		b.mMax.z < a.mMin.z;
	//�����̂ǂ���^�łȂ���΁A��������ɈႢ�Ȃ�
	return !no;
}

bool Intersect(const Capsule& a, const Capsule& b)
{
	float distSq = LineSegment::MinDistSq(a.mSegment,
		b.mSegment);
	float sumRadii = a.mRadius + b.mRadius;
	return distSq <= (sumRadii * sumRadii);
}

bool Intersect(const Sphere& s, const AABB& box)
{
	float distSq = box.MinDistSq(s.mCenter);
	return distSq <= (s.mRadius * s.mRadius);
}

bool Intersect(const LineSegment& l, const Sphere& s, float& outT)
{
	//��������X,Y�Ca�Cb�Cc���v�Z
	Vector3 X = l.mStart - s.mCenter;
	Vector3 Y = l.mEnd - l.mStart;
	float a = Vector3::Dot(Y, Y);
	float b = 2.0f * Vector3::Dot(X, Y);
	float c = Vector3::Dot(X, X) - s.mRadius * s.mRadius;
	//���ʎ����v�Z
	float disc = b * b - 4.0f * a * c;
	if (disc < 0.0f)
	{
		return false;
	}
	else
	{
		disc = Math::Sqrt(disc);
		//t�̉��imin/max�j�����߂�
		float tMin = (-b - disc) / (2.0f * a);
		float tMax = (-b + disc) / (2.0f * a);
		//t�������̗̈�ɂ���̂��`�F�b�N
		if (tMin >= 0.0f && tMin <= 1.0f)
		{
			outT = tMin;
			return true;
		}
		else if (tMax >= 0.0f && tMax <= 1.0f)
		{
			outT = tMax;
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Intersect(const LineSegment& l, const Plane& p, float& outT)
{
	//�ŏ��ɁA���̉������݂���̂��e�X�g����
	float denom = Vector3::Dot(l.mEnd - l.mStart, p.mNormal);
	if (Math::NearZero(denom))
	{
		//�����̉\��������̂́A�B��
		//start/end�����ʏ�̓_�ł��鎞
		if (Math::NearZero(Vector3::Dot(l.mStart, p.mNormal)
			- p.mD))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		float numer = -Vector3::Dot(l.mStart, p.mNormal) -
			p.mD;
		outT = numer / denom;
		//���������̋��E�ɂ��邩
		if (outT >= 0.0f && outT <= 1.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool TestSidePlane(float start, float end, float negd, const Vector3& norm,
	std::vector<std::pair<float, Vector3>>& out)
{
	float denom = end - start;
	if (Math::NearZero(denom))
	{
		return false;
	}
	else
	{
		float numer = -start + negd;
		float t = numer / denom;
		//t���͈͓��ɂ��邱�Ƃ��e�X�g
		if (t >= 0.0f && t <= 1.0f)
		{
			out.emplace_back(t, norm);
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Intersect(const LineSegment& l, const AABB& b, float& outT,
	Vector3& outNorm)
{
	//�\���̂���t�̒l��S�ĕۑ�����z��
	std::vector<std::pair<float, Vector3>> tValues;
	//x���ʂ��e�X�g
	TestSidePlane(l.mStart.x, l.mEnd.x, b.mMin.x, Vector3::NegUnitX,
		tValues);
	TestSidePlane(l.mStart.x, l.mEnd.x, b.mMax.x, Vector3::UnitX,
		tValues);
	//y���ʂ��e�X�g
	TestSidePlane(l.mStart.y, l.mEnd.y, b.mMin.y, Vector3::NegUnitY,
		tValues);
	TestSidePlane(l.mStart.y, l.mEnd.y, b.mMax.y, Vector3::UnitY,
		tValues);
	//z���ʂ��e�X�g
	TestSidePlane(l.mStart.z, l.mEnd.z, b.mMin.z, Vector3::NegUnitZ,
		tValues);
	TestSidePlane(l.mStart.z, l.mEnd.z, b.mMax.z, Vector3::UnitZ,
		tValues);

	//t�̒l�����������Ƀ\�[�g
	std::sort(tValues.begin(), tValues.end(), [](
		const std::pair<float, Vector3>& a,
		const std::pair<float, Vector3>& b) {
			return a.first < b.first;
		});
	//�{�b�N�X�ɁA��_���܂܂��̂��e�X�g
	Vector3 point;
	for (auto& t : tValues)
	{
		point = l.PointOnSegment(t.first);
		if (b.Contains(point))
		{
			outT = t.first;
			outNorm = t.second;
			return true;
		}
	}

	//�{�b�N�X�̓����Ɍ�_������Ȃ�
	return false;
}

bool Intersect(const OBB& o1, const OBB& o2)
{

	return false;
}

bool SweptSphere(const Sphere& P0, const Sphere& P1,
	const Sphere& Q0, const Sphere& Q1, float& outT)
{
	//����X, Y, a, b, c���v�Z
	Vector3 X = P0.mCenter - Q0.mCenter;
	Vector3 Y = P1.mCenter - P0.mCenter -
		(Q1.mCenter - Q0.mCenter);
	float a = Vector3::Dot(Y, Y);
	float b = 2.0f * Vector3::Dot(X, Y);
	float sumRadii = P0.mRadius + Q0.mRadius;
	float c = Vector3::Dot(X, X) - sumRadii * sumRadii;
	//���ʎ�������
	float disc = b * b - 4.0f * a * c;
	if (disc < 0.0f)
	{
		return false;
	}
	else
	{
		disc = Math::Sqrt(disc);
		//�������ق��̉��������d�v
		outT = (-b - disc) / (2.0f * a);
		if (outT >= 0.0f && outT <= 0.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

