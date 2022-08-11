#pragma once
#include <functional>
#include <string>

template <typename T>
class Vec2D {
public:
	class Hash
	{
	public:
		size_t operator()(const Vec2D& v) const {
			std::hash<T> hasher;
			return (hasher(v.x) << 32 | hasher(v.y));
		}
	};

public:
	Vec2D() = default;
	constexpr Vec2D(T x, T	y)
		:
		x(x),
		y(y)
	{}
	constexpr Vec2D(Vec2D pos0, Vec2D pos1)
		:
		x(pos1.x - pos0.x),
		y(pos1.y - pos0.y)
	{
	}
	Vec2D& operator= (const Vec2D& rhs) {
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	template <typename T2>
	Vec2D(Vec2D<T2> v)
		:
		Vec2D(static_cast<T>(v.x), static_cast<T>(v.y))
	{}


	Vec2D operator-() {
		x *= -1;
		y *= -1;
		return *this;
	}

	Vec2D operator+(const Vec2D& rhs) const {
		Vec2D out = *this;
		out.x += rhs.x;
		out.y += rhs.y;
		return out;
	}
	void operator+=(const Vec2D& rhs) {
		*this = *this + rhs;
	}
	Vec2D operator-(const Vec2D& rhs) const {
		Vec2D out = *this;
		out.x -= rhs.x;
		out.y -= rhs.y;
		return out;
	}
	void operator-=(const Vec2D& rhs) {
		*this = *this - rhs;
	}
	Vec2D operator*(T scale) const {
		Vec2D out = *this;
		out.x *= scale;
		out.y *= scale;
		return out;
	}
	void operator*=(T scale) {
		*this = *this * scale;
	}
	Vec2D operator/(T scale) const {
		T invScale = 1 / scale;
		return *this * invScale;
	}
	void operator/=(T scale) {
		*this = *this / scale;
	}

	bool operator==(const Vec2D& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Vec2D& rhs) const {
		return !(*this == rhs);
	}

	float GetLength() const {
		return sqrtf(x * x + y * y);
	}

	Vec2D GetNormalized() const {
		Vec2D v = *this;
		float magnitude = 1 / GetLength();
		v.x *= magnitude;
		v.y *= magnitude;
		return v;
	}

	void Normalize() {
		*this = GetNormalized();
	}

	float DotProduct(const Vec2D& rhs) {
		Vec2D v0 = this->GetNormalized();
		Vec2D v1 = rhs.GetNormalized();

		return v0.x * v1.x + v0.y * v1.y;
	}

	std::string ToString() const
	{
		return "X = " + std::to_string(x) + " Y = " + std::to_string(y);
	}

	static float Distance(const Vec2D<T>& lhs, const Vec2D<T>& rhs)
	{
		Vec2D<T> v(lhs, rhs);
		return v.GetLength();
	}

public:

	T x = static_cast<T>(0);
	T y = static_cast<T>(0);
};

typedef Vec2D<int> IVec2D;
typedef Vec2D<long long> LLVec2D;
typedef Vec2D<float> FVec2D;