#ifndef PHYSICS_H
#define PHYSICS_H

#include <memory>

#include "Entity.hpp"

struct Intersect {
	bool isIntersect;
	Vec2f pos;
};

namespace Physics {
	Vec2f GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
	Vec2f GetOverlap(const Vec2f& aPos, const Vec2f& aHalfSize, std::shared_ptr<Entity> e);

	Vec2f GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
	Vec2f CalcOverlap(const Vec2f& aPos, const Vec2f& aHalfSize,
		const Vec2f& bPos, const Vec2f& bHalfSize);
	bool IsInside(const Vec2f& pos, std::shared_ptr<Entity> e);
	Intersect LineIntersect(const Vec2f& a, const Vec2f& b,
		const Vec2f& c, const Vec2f& d);
	bool EntityIntersect(const Vec2f& a, const Vec2f& b, std::shared_ptr<Entity> e);
}

#endif // !PHYSICS_H
