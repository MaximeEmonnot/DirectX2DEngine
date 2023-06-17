export module MathEngine;

import <functional>;
import <string>;

export
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
		if (GetLength() > 0) {
			float magnitude = 1 / GetLength();
			v.x *= magnitude;
			v.y *= magnitude;
			return v;
		}
		return Vec2D();
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

export typedef Vec2D<int> IVec2D;
export typedef Vec2D<long long> LLVec2D;
export typedef Vec2D<float> FVec2D;

export
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

	Rect operator-() const
	{
		Rect out = *this;
		out.pos = -out.pos;
		return out;
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
export typedef Rect<int> IRect;
export typedef Rect<float> FRect;