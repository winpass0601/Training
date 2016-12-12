#pragma once

#define PI (3.14159265)

namespace rinfw {
namespace core {
namespace math {

struct Float3 {
	union {
		struct {
			float x, y, z;
		};
		float p[3];
	};
	Float3() {
		x = y = z = 0;
	}
	Float3(Float3& obj) {
		this->x = obj.x;
		this->y = obj.y;
		this->z = obj.z;
	}
	Float3(float _x, float _y, float _z) {
		this->x = _x;
		this->y = _y;
		this->z = _z;
	}
	inline Float3 operator+(Float3& obj) {
		Float3 tmp;
		tmp.p[0] = this->p[0] + obj.p[0];
		tmp.p[1] = this->p[1] + obj.p[1];
		tmp.p[2] = this->p[2] + obj.p[2];
		return tmp;
	};
	inline Float3 operator-(Float3& obj) {
		Float3 tmp;
		tmp.p[0] = this->p[0] - obj.p[0];
		tmp.p[1] = this->p[1] - obj.p[1];
		tmp.p[2] = this->p[2] - obj.p[2];
		return tmp;
	};
	inline Float3 operator=(Float3& obj) {
		this->p[0] = obj.p[0];
		this->p[1] = obj.p[1];
		this->p[2] = obj.p[2];
		return *this;
	}
	inline Float3 operator=(const Float3& obj) {
		this->p[0] = obj.p[0];
		this->p[1] = obj.p[1];
		this->p[2] = obj.p[2];
		return *this;
	}
	inline Float3 operator+=(Float3& obj) {
		this->p[0] += obj.p[0];
		this->p[1] += obj.p[1];
		this->p[2] += obj.p[2];
		return *this;
	};
	inline Float3 operator*(Float3& obj) {
		Float3 tmp;
		tmp.p[0] = this->p[0] * obj.p[0];
		tmp.p[1] = this->p[1] * obj.p[1];
		tmp.p[2] = this->p[2] * obj.p[2];
		return tmp;
	};
	inline Float3 operator/(float obj) {
		Float3 tmp;
		tmp.p[0] = this->p[0] / obj;
		tmp.p[1] = this->p[1] / obj;
		tmp.p[2] = this->p[2] / obj;
		return tmp;
	};
	inline Float3 operator*(float obj) {
		Float3 tmp;
		tmp.p[0] = this->p[0] * obj;
		tmp.p[1] = this->p[1] * obj;
		tmp.p[2] = this->p[2] * obj;
		return tmp;
	};
};

struct Float4 {
	union {
		struct {
			float x, y, z, w;
		};
		float p[4];
		unsigned char u[16];
	};
	Float4() {
		x = y = z = w = 0;
	}
	Float4(Float4& obj) {
		this->x = obj.x;
		this->y = obj.y;
		this->z = obj.z;
		this->w = obj.w;
	}
	Float4(float _x, float _y, float _z, float _w) {
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->w = _w;
	}
	inline Float4 operator-(Float4& obj) {
		Float4 tmp;
		tmp.p[0] = this->p[0] - obj.p[0];
		tmp.p[1] = this->p[1] - obj.p[1];
		tmp.p[2] = this->p[2] - obj.p[2];
		tmp.p[3] = this->p[3] - obj.p[3];
		return tmp;
	};
	inline Float4 operator=(Float4& obj) {
		this->p[0] = obj.p[0];
		this->p[1] = obj.p[1];
		this->p[2] = obj.p[2];
		this->p[3] = obj.p[3];
		return *this;
	}
	inline Float4 operator+=(Float4& obj) {
		this->p[0] += obj.p[0];
		this->p[1] += obj.p[1];
		this->p[2] += obj.p[2];
		this->p[3] += obj.p[3];
		return *this;
	};
	inline Float4 operator*(Float4& obj) {
		Float4 tmp;
		tmp.p[0] = this->p[0] * obj.p[0];
		tmp.p[1] = this->p[1] * obj.p[1];
		tmp.p[2] = this->p[2] * obj.p[2];
		tmp.p[3] = this->p[3] * obj.p[3];
		return tmp;
	};
	inline Float4 operator/(float obj) {
		Float4 tmp;
		tmp.p[0] = this->p[0] / obj;
		tmp.p[1] = this->p[1] / obj;
		tmp.p[2] = this->p[2] / obj;
		tmp.p[3] = this->p[3] / obj;
		return tmp;
	};
	inline Float4 operator*(float obj) {
		Float4 tmp;
		tmp.p[0] = this->p[0] * obj;
		tmp.p[1] = this->p[1] * obj;
		tmp.p[2] = this->p[2] * obj;
		tmp.p[3] = this->p[3] * obj;
		return tmp;
	};
};

struct Matrix44 {
	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};

	inline Matrix44 operator*(Matrix44& obj) {
		Matrix44 tmp;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				tmp.m[i][j] = 0;
				for (int k = 0; k < 4; ++k) {
					tmp.m[i][j] += this->m[i][k] * obj.m[k][j];
				}
			}
		}
		return tmp;
	};
	inline Matrix44 operator*=(Matrix44& obj) {
		Matrix44 tmp = *this;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				this->m[i][j] = 0;
				for (int k = 0; k < 4; ++k) {
					this->m[i][j] += tmp.m[i][k] * obj.m[k][j];
				}
			}
		}
		return *this;
	};
};

typedef Float3 Point;
}
}
}
