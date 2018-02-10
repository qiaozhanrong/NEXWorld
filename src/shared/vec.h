#ifndef VEC_H_
#define VEC_H_

#include <cmath>
#include <utility>

template <typename T>
class Vec3 {
public:
	T x, y, z;

	Vec3() : x(), y(), z() {}
	Vec3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}
	Vec3(T v) : x(v), y(v), z(v) {}

	template <typename U, std::enable_if_t<std::is_convertible<T, U>::value, int> = 0>
	Vec3(const Vec3<U>& rhs) : x(T(rhs.x)), y(T(rhs.y)), z(T(rhs.z)) {}

	T lengthSqr() const { return x * x + y * y + z * z; }
	T length() const { return sqrt(lengthSqr()); }

	T euclideanDistance(const Vec3& rhs) const { return (*this - rhs).length(); }
	T chebyshevDistance(const Vec3& rhs) const { return max(max(abs(x - rhs.x), abs(y - rhs.y)), abs(z - rhs.z)); }
	T manhattanDistance(const Vec3& rhs) const { return abs(x - rhs.x) + abs(y - rhs.y) + abs(z - rhs.z); }
	void normalize() { (*this) /= length(); }

	Vec3& operator+= (const Vec3& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Vec3& operator-= (const Vec3& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	Vec3& operator*= (T value) {
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}

	Vec3& operator/= (T value) {
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}

	bool operator< (const Vec3& rhs) const {
		if (x != rhs.x) return x < rhs.x;
		if (y != rhs.y) return y < rhs.y;
		if (z != rhs.z) return z < rhs.z;
		return false;
	}

	Vec3 operator+ (T value) const { return Vec3(x + value, y + value, z + value); }
	Vec3 operator- (T value) const { return Vec3(x - value, y - value, z - value); }
	Vec3 operator* (T value) const { return Vec3(x * value, y * value, z * value); }
	Vec3 operator/ (T value) const { return Vec3(x / value, y / value, z / value); }
	friend Vec3 operator- (const Vec3& vec) { return Vec3(-vec.x, -vec.y, -vec.z); }
	bool operator== (const Vec3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
	bool operator!= (const Vec3& rhs) const { return !(rhs == *this); }

	void swap(Vec3& rhs) {
		std::swap(x, rhs.x);
		std::swap(y, rhs.y);
		std::swap(z, rhs.z);
	}

	template <typename Func>
	static void range(T begin, T end, Func func) {
		Vec3 a;
		for (a.x = begin; a.x < end; a.x++)
			for (a.y = begin; a.y < end; a.y++)
				for (a.z = begin; a.z < end; a.z++)
					func(a);
	}

	template <typename Func>
	static void range(const Vec3& begin, const Vec3& end, Func func) {
		Vec3 a;
		for (a.x = begin.x; a.x < end.x; a.x++)
			for (a.y = begin.y; a.y < end.y; a.y++)
				for (a.z = begin.z; a.z < end.z; a.z++)
					func(a);
	}
};

using Vec3i = Vec3<int>;
using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;

#endif // !VEC_H_