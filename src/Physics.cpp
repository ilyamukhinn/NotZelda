#include "Physics.hpp"
#include "Components.hpp"

Vec2f Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
	if (a->has<CTransform>() &&
		b->has<CTransform>() &&
		a->has<CBoundingBox>() &&
		b->has<CBoundingBox>()) {
		return CalcOverlap(a->get<CTransform>().m_pos,
			a->get<CBoundingBox>().m_halfSize,
			b->get<CTransform>().m_pos,
			b->get<CBoundingBox>().m_halfSize);
	}

	return Vec2f(0.0f, 0.0f);
}

Vec2f Physics::GetOverlap(const Vec2f& aPos, const Vec2f& aHalfSize, std::shared_ptr<Entity> e) {
	return CalcOverlap(aPos, aHalfSize, 
		e->get<CTransform>().m_pos, 
		e->get<CBoundingBox>().m_halfSize);
}

Vec2f Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
	if (a->has<CTransform>() &&
		b->has<CTransform>() &&
		a->has<CBoundingBox>() &&
		b->has<CBoundingBox>()) {
		return CalcOverlap(a->get<CTransform>().m_prevPos,
			a->get<CBoundingBox>().m_halfSize,
			b->get<CTransform>().m_prevPos,
			b->get<CBoundingBox>().m_halfSize);
	}

	return Vec2f(0.0f, 0.0f);
}

Vec2f Physics::CalcOverlap(const Vec2f& aPos, const Vec2f& aHalfSize,
	const Vec2f& bPos, const Vec2f& bHalfSize) {
	Vec2f delta = Vec2f(abs(aPos.x - bPos.x), abs(aPos.y - bPos.y));
	float oX = aHalfSize.x + bHalfSize.x - delta.x;
	float oY = aHalfSize.y + bHalfSize.y - delta.y;
	return Vec2f(oX, oY);
}

bool Physics::IsInside(const Vec2f& pos, std::shared_ptr<Entity> e) {
	auto ePos = e->get<CTransform>().m_pos;
	auto eHalfSize = e->get<CBoundingBox>().m_halfSize;
	float x1{ ePos.x - eHalfSize.x }, x4{ ePos.x + eHalfSize.x };
	float y1{ ePos.y - eHalfSize.y }, y4{ ePos.y + eHalfSize.y };

	return (pos.x >= x1 && pos.y >= y1 && pos.x <= x4 && pos.y <= y4);
}

Intersect Physics::LineIntersect(const Vec2f& a, const Vec2f& b,
	const Vec2f& c, const Vec2f& d) {
	Vec2f r = b - a;
	Vec2f s = d - c;
	float rxs = r ^ s;
	Vec2f cma = c - a;
	float t = (cma ^ s) / rxs;
	float u = (cma ^ r) / rxs;

	if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f) {
		return { true, Vec2f(a.x + t * r.x, a.y + t * r.y) };
	}

	return { false, Vec2f{0.0f, 0.0f} };
}

bool Physics::EntityIntersect(const Vec2f& a, const Vec2f& b, std::shared_ptr<Entity> e) {
	auto ePos = e->get<CTransform>().m_pos;
	auto eHalfSize = e->get<CBoundingBox>().m_halfSize;
	float x1{ ePos.x - eHalfSize.x }, x2{ ePos.x + eHalfSize.x }, x3{ x1 }, x4{ x2 };
	float y1{ ePos.y - eHalfSize.y }, y2{ y1 }, y3{ ePos.y + eHalfSize.y }, y4{ y3 };

	return (
		LineIntersect(a, b, { x1, y1 }, { x2, y2 }).isIntersect ||
		LineIntersect(a, b, { x1, y1 }, { x3, y3 }).isIntersect ||
		LineIntersect(a, b, { x2, y2 }, { x4, y4 }).isIntersect ||
		LineIntersect(a, b, { x3, y3 }, { x4, y4 }).isIntersect
		);
}