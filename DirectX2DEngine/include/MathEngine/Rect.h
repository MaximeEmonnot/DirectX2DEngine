#pragma once

#include "Vec2D.h"

template <typename T>
class Rect {
public:
	Rect()
	{}
	constexpr Rect(T x, T y, T width, T height)
		:
		pos(Vec2D<T>(x, y)),
		width(width),
		height(height)
	{}
	Rect(Vec2D<T> pos, T width, T height)
		:
		pos(pos),
		width(width),
		height(height)
	{}

	template<typename T2> operator Rect<T2>() const
	{
		return Rect<T2>(static_cast<Vec2D<T2>>(pos), static_cast<T2>(width), static_cast<T2>(height));
	}

	Rect operator+(const Vec2D<T>& rhs) const {
		Rect out = *this;
		out.pos += rhs;
		return out;
	}
	void operator+= (const Vec2D<T>& rhs) {
		*this = *this + rhs;
	}

	Rect operator-(const Vec2D<T>& rhs) const {
		Rect out = *this;
		out.pos -= rhs;
		return out;
	}
	void operator-= (const Vec2D<T>& rhs) {
		*this = *this - rhs;
	}

	Rect operator* (T scale) const
	{
		Rect out = *this;
		out.width *= scale;
		out.height *= scale;
		return out;
	}
	void operator*= (T scale)
	{
		*this = *this * scale;
	}

	Rect operator/ (T scale) const
	{
		T inv_scale = 1 / scale;
		return *this * inv_scale;
	}
	void operator/= (T scale)
	{
		*this = *this / scale;
	}

	bool operator==(const Rect& rhs) const {
		return pos == rhs.pos && width == rhs.width && height == rhs.height;
	}
	bool operator!=(const Rect& rhs) const {
		return !(this == rhs);
	}

	bool ContainsVec2D(const Vec2D<T>& p) const {
		return pos.x - width / 2 <= p.x
			&& pos.x + width / 2 >= p.x
			&& pos.y + height / 2 >= p.y
			&& pos.y - height / 2 <= p.y;
	}

	bool IsCollidingRect(const Rect& rhs) const
	{
		return !(pos.x + width / 2 < rhs.pos.x - rhs.width / 2
			|| pos.x - width / 2 > rhs.pos.x + rhs.width / 2
			|| pos.y - height / 2 > rhs.pos.y + rhs.height / 2
			|| pos.y + height / 2 < rhs.pos.y - rhs.height / 2);
	}

	Vec2D<T> GetNormalFromPoint(const Vec2D<T>& point)
	{
		Vec2D<T> out;
		if (!ContainsVec2D(point))
		{
			const Vec2D<T> top_point = Vec2D<T>(pos.x, pos.y + height / 2);
			const Vec2D<T> bottom_point = Vec2D<T>(pos.x, pos.y - height / 2);
			const Vec2D<T> right_point = Vec2D<T>(pos.x + width / 2, pos.y);
			const Vec2D<T> left_point = Vec2D<T>(pos.x - width / 2, pos.y);

			if (point.x < left_point.x) out += Vec2D<T>(-1, 0);
			else if (point.x > right_point.x) out += Vec2D<T>(1, 0);
			if (point.y > top_point.y) out += Vec2D<T>(0, 1);
			else if (point.y < bottom_point.y) out += Vec2D<T>(0, -1);
		}
		else
			return Vec2D<T>(0, 1);
		out.Normalize();
		return out;
	}

	std::string ToString() const
	{
		return pos.ToString() + " Width = " + std::to_string(width) + " Height = " + std::to_string(height);
	}

public:
	Vec2D<T> pos = Vec2D<T>(0, 0);
	T width = static_cast<T>(0);
	T height = static_cast<T>(0);
};
typedef Rect<int> IRect;
typedef Rect<float> FRect;