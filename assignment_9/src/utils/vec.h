//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================
#ifndef GL_MATH_H
#define GL_MATH_H
//=============================================================================

#include <iostream>
#include <cassert>
#include <math.h>
#include <vector>


#ifdef _WIN32
#define M_PI 3.14159265
#include <algorithm>
#endif


//=============================================================================


/// \file glmath.h Implements the vector and matrix classes and their mathematical operations.


//=============================================================================


class mat4;


//=============================================================================


inline float deg2rad(const float deg) { return deg/180.0f*(float)M_PI; }
inline float rad2deg(const float rad) { return rad*180.0f/(float)M_PI; }

//=============================================================================

/// \class vec2
/// This class implements a simple 3D vector, that we use to represent
/// 3D points and 3D color. You can access the individual components either by
/// x,y,z or by r,g,b. The vec2 class provides all commonly used mathematical
/// operations.
class vec2
{
public:

	/// this union allows us to access vector elements by either x,y,z or r,g,b,
	/// but we have to store only three floats.
	union
	{
		/// XYZ coordinates
		struct
		{
			float x; ///< x coordinate
			float y; ///< y coordinate
		};
	};


public:

	/// default constructor
	vec2() {}

	/// construct with scalar value that is assigned to x, y, and z
	vec2(float _s) : x(_s), y(_s) {}

	/// construct with x,y,z values
	vec2(float _x, float _y) : x(_x), y(_y) {}

	/// return pointer to array/vector (for passing it to OpenGL)
	const float* data() const { return &x; }

	/// read/write the _i'th vector component (_i from 0 to 2)
	float& operator[](unsigned int _i)
	{
		assert(_i < 2);
		return (&x)[_i];
	}

	/// read the _i'th vector component (_i from 0 to 2)
	const float operator[](unsigned int _i) const
	{
		assert(_i < 2);
		return (&x)[_i];
	}


	/// multiply this vector by a scalar \c s
	vec2& operator*=(const float s)
	{
		x *= s;
		y *= s;
		return *this;
	}

	/// divide this vector by a scalar \c s
	vec2& operator/=(const float s)
	{
		x /= s;
		y /= s;
		return *this;
	}

	/// component-wise multiplication of this vector with vector \c v
	vec2& operator*=(const vec2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	/// subtract vector \c v from this vector
	vec2& operator-=(const vec2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	/// add vector \c v to this vector
	vec2& operator+=(const vec2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	vec2& operator/=(const vec2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}
};


//-----------------------------------------------------------------------------


/// unary minus: turn v into -v
inline const vec2 operator-(const vec2& v)
{
	return vec2(-v.x, -v.y);
}

/// multiply vector \c v by scalar \c s
inline const vec2 operator*(const float s, const vec2& v )
{
	return vec2(s * v.x,
				s * v.y);
}

/// multiply vector \c v by scalar \c s
inline const vec2 operator*(const vec2& v, const float s)
{
	return vec2(s * v.x,
				s * v.y);
}

/// component-wise multiplication of vectors \c v0 and \c v1
inline const vec2 operator*(const vec2& v0, const vec2& v1)
{
	return vec2(v0.x * v1.x,
				v0.y * v1.y);
}

/// divide vector \c v by scalar \c s
inline const vec2 operator/(const vec2& v, const float s)
{
	return vec2(v.x / s,
				v.y / s);
}

inline const vec2 operator/(const vec2& v0, const vec2& v1)
{
	return vec2(v0.x / v1.x,
				v0.y / v1.y);
}

/// add two vectors \c v0 and \c v1
inline const vec2 operator+(const vec2& v0, const vec2& v1)
{
	return vec2(v0.x + v1.x,
				v0.y + v1.y);
}

/// subtract vector \c v1 from vector \c v0
inline const vec2 operator-(const vec2& v0, const vec2& v1)
{
	return vec2(v0.x - v1.x,
				v0.y - v1.y);
}

/// compute the component-wise minimum of vectors \c v0 and \c v1
inline const vec2 min(const vec2& v0, const vec2& v1)
{
	return vec2(std::min(v0.x, v1.x),
				std::min(v0.y, v1.y));
}

/// compute the component-wise maximum of vectors \c v0 and \c v1
inline const vec2 max(const vec2& v0, const vec2& v1)
{
	return vec2(std::max(v0.x, v1.x),
				std::max(v0.y, v1.y));
}

/// compute the Euclidean norm (length) of a vector \c v
inline const float norm(const vec2& v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

/// normalize vector \c v by dividing it by its norm
inline const vec2 normalize(const vec2& v)
{
	const float n = norm(v);
	if (n != 0.0)
	{
		return vec2(v.x / n,
					v.y / n);
	}
	return v;
}

/// compute the distance between vectors \c v0 and \c v1
inline const float distance(const vec2& v0, const vec2& v1)
{
	return norm(v0-v1);
}

/// compute the Euclidean dot product of \c v0 and \c v1
inline const float dot(const vec2& v0, const vec2& v1)
{
	return (v0.x*v1.x + v0.y*v1.y);
}

/// reflect vector \c v at normal \c n
inline const vec2 reflect(const vec2& v, const vec2& n)
{
	return v - (2.0f * dot(n,v)) * n;
}

/// read the space-separated components of a vector from a stream
inline std::istream& operator>>(std::istream& is, vec2& v)
{
	is >> v.x >> v.y;
	return is;
}

/// output a vector by printing its comma-separated compontens
inline std::ostream& operator<<(std::ostream& os, const vec2& v)
{
	os << '(' << v.x << ", " << v.y << ')';
	return os;
}

//=============================================================================


/// \class vec3 glmath.h
/// This class implements a simple 3D vector, that we use to represent
/// 3D points and 3D color. You can access the individual components either by
/// x,y,z or by r,g,b. The vec3 class provides all commonly used mathematical
/// operations.
/// \sa glmath.h
class vec3
{
public:

	/// this union allows us to access vector elements by either x,y,z or r,g,b,
	/// but we have to store only three floats.
	union
	{
		/// XYZ coordinates
		struct
		{
			float x; ///< x coordinate
			float y; ///< y coordinate
			float z; ///< z coordinate
		};

		/// RGB color components
		struct
		{
			float r; ///< red component
			float g; ///< green component
			float b; ///< blue component
		};
	};


public:

	/// default constructor
	vec3() {}

	/// construct with scalar value that is assigned to x, y, and z
	vec3(float _s) : x(_s), y(_s), z(_s) {}

	/// construct with x,y,z values
	vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

	/// return pointer to array/vector (for passing it to OpenGL)
	const float* data() const { return &x; }

	/// read/write the _i'th vector component (_i from 0 to 2)
	float& operator[](unsigned int _i)
	{
		assert(_i < 3);
		return (&x)[_i];
	}

	/// read the _i'th vector component (_i from 0 to 2)
	const float operator[](unsigned int _i) const
	{
		assert(_i < 3);
		return (&x)[_i];
	}


	/// multiply this vector by a scalar \c s
	vec3& operator*=(const float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	/// divide this vector by a scalar \c s
	vec3& operator/=(const float s)
	{
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	/// component-wise multiplication of this vector with vector \c v
	vec3& operator*=(const vec3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	/// subtract vector \c v from this vector
	vec3& operator-=(const vec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	/// add vector \c v to this vector
	vec3& operator+=(const vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
};


//-----------------------------------------------------------------------------


/// unary minus: turn v into -v
inline const vec3 operator-(const vec3& v)
{
	return vec3(-v.x, -v.y, -v.z);
}

/// multiply vector \c v by scalar \c s
inline const vec3 operator*(const float s, const vec3& v )
{
	return vec3(s * v.x,
				s * v.y,
				s * v.z);
}

/// multiply vector \c v by scalar \c s
inline const vec3 operator*(const vec3& v, const float s)
{
	return vec3(s * v.x,
				s * v.y,
				s * v.z);
}

/// component-wise multiplication of vectors \c v0 and \c v1
inline const vec3 operator*(const vec3& v0, const vec3& v1)
{
	return vec3(v0.x * v1.x,
				v0.y * v1.y,
				v0.z * v1.z);
}

/// divide vector \c v by scalar \c s
inline const vec3 operator/(const vec3& v, const float s)
{
	return vec3(v.x / s,
				v.y / s,
				v.z / s);
}

/// add two vectors \c v0 and \c v1
inline const vec3 operator+(const vec3& v0, const vec3& v1)
{
	return vec3(v0.x + v1.x,
				v0.y + v1.y,
				v0.z + v1.z);
}

/// subtract vector \c v1 from vector \c v0
inline const vec3 operator-(const vec3& v0, const vec3& v1)
{
	return vec3(v0.x - v1.x,
				v0.y - v1.y,
				v0.z - v1.z);
}

/// compute the component-wise minimum of vectors \c v0 and \c v1
inline const vec3 min(const vec3& v0, const vec3& v1)
{
	return vec3(std::min(v0.x, v1.x),
				std::min(v0.y, v1.y),
				std::min(v0.z, v1.z));
}

/// compute the component-wise maximum of vectors \c v0 and \c v1
inline const vec3 max(const vec3& v0, const vec3& v1)
{
	return vec3(std::max(v0.x, v1.x),
				std::max(v0.y, v1.y),
				std::max(v0.z, v1.z));
}

/// compute the Euclidean norm (length) of a vector \c v
inline const float norm(const vec3& v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

/// normalize vector \c v by dividing it by its norm
inline const vec3 normalize(const vec3& v)
{
	const float n = norm(v);
	if (n != 0.0)
	{
		return vec3(v.x / n,
					v.y / n,
					v.z / n);
	}
	return v;
}

/// compute the distance between vectors \c v0 and \c v1
inline const float distance(const vec3& v0, const vec3& v1)
{
	return norm(v0-v1);
}

/// compute the Euclidean dot product of \c v0 and \c v1
inline const float dot(const vec3& v0, const vec3& v1)
{
	return (v0.x*v1.x + v0.y*v1.y + v0.z*v1.z);
}

/// compute the cross product of \c v0 and \c v1
inline const vec3 cross(const vec3& v0, const vec3& v1)
{
	return vec3(v0.y*v1.z - v0.z*v1.y,
				v0.z*v1.x - v0.x*v1.z,
				v0.x*v1.y - v0.y*v1.x);
}

/// reflect vector \c v at normal \c n
inline const vec3 reflect(const vec3& v, const vec3& n)
{
	return v - (2.0f * dot(n,v)) * n;
}

/// read the space-separated components of a vector from a stream
inline std::istream& operator>>(std::istream& is, vec3& v)
{
	is >> v.x >> v.y >> v.z;
	return is;
}

/// output a vector by printing its comma-separated compontens
inline std::ostream& operator<<(std::ostream& os, const vec3& v)
{
	os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
	return os;
}


//=============================================================================


/// \class vec4 glmath.h
/// This class implements a simple 4D vector, that we use to represent
/// points in 4D homogeneous coordinates XYZW or RGBA colors. You can access
/// the individual components either by x,y,z,w or by r,g,b,a. The vec4 class
/// provides all commonly used mathematical operations.
/// \sa glmath.h
class vec4
{
public:

	/// this union allows us to access vector elements by either x,y,z or r,g,b,
	/// but we have to store only three floats.
	union
	{
		/// XYZW coordinates
		struct
		{
			float x; ///< x coordinate
			float y; ///< y coordinate
			float z; ///< z coordinate
			float w; ///< w coordinate
		};

		/// RGBA color components
		struct
		{
			float r; ///< red component
			float g; ///< green component
			float b; ///< blue component
			float a; ///< alpha component
		};
	};


public:

	/// default constructor
	vec4() {}

	/// construct with scalar value that is assigned to x, y, z, w
	vec4(float _s) : x(_s), y(_s), z(_s), w(_s) {}

	/// construct with x,y,z,w values
	vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

	/// return pointer to array/vector (for passing it to OpenGL)
	const float* data() const { return &x; }

	// cast vec4 to vec3 by simply removing w-component
	operator vec3() { return vec3(x,y,z); }

	/// read/write the _i'th vector component (_i from 0 to 3)
	float& operator[](unsigned int _i)
	{
		assert(_i < 4);
		return (&x)[_i];
	}

	/// read the _i'th vector component (_i from 0 to 3)
	const float operator[](unsigned int _i) const
	{
		assert(_i < 4);
		return (&x)[_i];
	}


	/// multiply this vector by a scalar \c s
	vec4& operator*=(const float s)
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}

	/// divide this vector by a scalar \c s
	vec4& operator/=(const float s)
	{
		x /= s;
		y /= s;
		z /= s;
		w /= s;
		return *this;
	}

	/// component-wise multiplication of this vector with vector \c v
	vec4& operator*=(const vec4& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}

	/// subtract vector \c v from this vector
	vec4& operator-=(const vec4& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	/// add vector \c v to this vector
	vec4& operator+=(const vec4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
};


//-----------------------------------------------------------------------------


/// unary minus: turn v into -v
inline const vec4 operator-(const vec4& v)
{
	return vec4(-v.x, -v.y, -v.z, -v.w);
}

/// multiply vector \c v by scalar \c s
inline const vec4 operator*(const float s, const vec4& v )
{
	return vec4(s * v.x,
				s * v.y,
				s * v.z,
				s * v.w);
}

/// multiply vector \c v by scalar \c s
inline const vec4 operator*(const vec4& v, const float s)
{
	return vec4(s * v.x,
				s * v.y,
				s * v.z,
				s * v.w);
}

/// component-wise multiplication of vectors \c v0 and \c v1
inline const vec4 operator*(const vec4& v0, const vec4& v1)
{
	return vec4(v0.x * v1.x,
				v0.y * v1.y,
				v0.z * v1.z,
				v0.w * v1.w);
}

/// divide vector \c v by scalar \c s
inline const vec4 operator/(const vec4& v, const float s)
{
	return vec4(v.x / s,
				v.y / s,
				v.z / s,
				v.w / s);
}

/// add two vectors \c v0 and \c v1
inline const vec4 operator+(const vec4& v0, const vec4& v1)
{
	return vec4(v0.x + v1.x,
				v0.y + v1.y,
				v0.z + v1.z,
				v0.w + v1.w);
}

/// subtract vector \c v1 from vector \c v0
inline const vec4 operator-(const vec4& v0, const vec4& v1)
{
	return vec4(v0.x - v1.x,
				v0.y - v1.y,
				v0.z - v1.z,
				v0.w - v1.w);
}

/// compute the Euclidean norm (length) of a vector \c v
inline const float norm(const vec4& v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
}

/// compute the Euclidean dot product of \c v0 and \c v1
inline const float dot(const vec4& v0, const vec4& v1)
{
	return (v0.x*v1.x + v0.y*v1.y + v0.z*v1.z + v0.w*v1.w);
}

/// read the space-separated components of a vector from a stream
inline std::istream& operator>>(std::istream& is, vec4& v)
{
	is >> v.x >> v.y >> v.z >> v.w;
	return is;
}

/// output a vector by printing its comma-separated compontens
inline std::ostream& operator<<(std::ostream& os, const vec4& v)
{
	os << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')';
	return os;
}

//=============================================================================


/// \class mat2 glmath.h
/// This class implements a simple 3x3 matrix.
/// \sa glmath.h
class mat2
{
private:

	/// the data is stored as an array of 9 values
	float data_[4];

public:

	/// default constructor
	mat2() {}

	mat2(float m00, float m01, float m10, float m11);

	/// construct matrix where all values are s
	explicit mat2(float s);

	/// read/write access to element (i,j)
	float& operator()(const int i, const int j) { return data_[i+j*2]; }

	/// read access to element (i,j)
	float operator()(const int i, const int j) const { return data_[i+j*2]; }

	/// pointer to data (for passing it to OpenGL)
	const float* data() const { return data_; }
};


//-----------------------------------------------------------------------------


/// return matrix product m0*m1
mat2 operator*(const mat2& m0, const mat2& m1);

vec2 operator*(const mat2& m, const vec2& v);

/// return transposed matrix
mat2 transpose(const mat2& m);

/// return inverse of 3x3 matrix
mat2 inverse(const mat2& m);

/// print matrix to output stream
std::ostream& operator<<(std::ostream& os, const mat2& m);

//=============================================================================


/// \class mat3 glmath.h
/// This class implements a simple 3x3 matrix.
/// \sa glmath.h
class mat3
{
private:

	/// the data is stored as an array of 9 values
	float data_[9];

public:

	/// default constructor
	mat3() {}

	mat3(
		float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22);

	/// construct matrix where all values are s
	explicit mat3(float s);

	/// construct 3x3 matrix from the upper-left 3x3 part of a 4x4 matrix
	mat3(const mat4& m4);

	/// read/write access to element (i,j)
	float& operator()(const int i, const int j) { return data_[i+j*3]; }

	/// read access to element (i,j)
	float operator()(const int i, const int j) const { return data_[i+j*3]; }

	/// pointer to data (for passing it to OpenGL)
	const float* data() const { return data_; }
};


//-----------------------------------------------------------------------------


/// return matrix product m0*m1
mat3 operator*(const mat3& m0, const mat3& m1);

/// return transposed matrix
mat3 transpose(const mat3& m);

/// return inverse of 3x3 matrix
mat3 inverse(const mat3& m);

/// print matrix to output stream
std::ostream& operator<<(std::ostream& os, const mat3& m);


//=============================================================================


/// \class mat4 glmath.h
/// This class implements a simple 4x4 matrix.
/// \sa glmath.h
class mat4
{
private:

	/// the data is stored as an array of 16 values
	float data_[16];

public:
	/// default constructor
	mat4() {}

	mat4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	/// construct matrix where all values are s
	explicit mat4(float s);

	/// read/write access to element (i,j)
	float& operator()(const int i, const int j) { return data_[i+(j<<2)]; }
	/// read access to element (i,j)
	float operator()(const int i, const int j) const { return data_[i+(j<<2)]; }

	/// pointer to data (for passing it to OpenGL)
	const float* data() const { return data_; }

	/// return identity matrix
	static mat4 identity();
	/// return frustum matrix
	static mat4 frustum(float left, float right, float bottom, float top, float near, float far);
	/// return matrix for perspective projection (special case of frustum matrix)
	static mat4 perspective(float fovy, float aspect, float near, float far);
	/// return look-at camera matrix
	static mat4 look_at(const vec3& eye, const vec3& center, const vec3& up);
	/// return translation matrix
	static mat4 translate(const vec3& t);
	// return scaling matrix
	static mat4 scale(float factor);
	/// return matrix for rotation around x-axis
	static mat4 rotate_x(float angle);
	/// return matrix for rotation around y-axis
	static mat4 rotate_y(float angle);
	/// return matrix for rotation around z-axis
	static mat4 rotate_z(float angle);
};


//-----------------------------------------------------------------------------


/// return matrix product m0*m1
mat4 operator*(const mat4& m0, const mat4& m1);

/// return matrix-vector product m*v
vec4 operator*(const mat4& m, const vec4& v0);

/// print matrix to output stream
std::ostream& operator<<(std::ostream& os, const mat4& m);


//=============================================================================
#endif
//=============================================================================

