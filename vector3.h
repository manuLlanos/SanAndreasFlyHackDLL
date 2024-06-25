#pragma once


class Vector3 {
public:
	float x;
	float y;
	float z;

	Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {
	};

	Vector3 operator+(Vector3 const& obj) {
		Vector3 res;
		res.x = x + obj.x;
		res.y = y + obj.y;
		res.z = z + obj.z;
		return res;
	}
	Vector3 operator-(Vector3 const& obj) {
		Vector3 res;
		res.x = x - obj.x;
		res.y = y - obj.y;
		res.z = z - obj.z;
		return res;
	}
	Vector3 operator*(float num) {
		Vector3 res;
		res.x = x * num;
		res.y = y * num;
		res.z = z * num;
		return res;
	}
};