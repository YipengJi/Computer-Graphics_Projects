//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================
#ifndef VEC_H
#define VEC_H
//=============================================================================

#include <iostream>
#include <cassert>
#include <math.h>
#include <algorithm>


#ifdef _WIN32
#define M_PI 3.14159265
#include <algorithm>
#endif


//=============================================================================


/// \file vec.h Implements the vector and matrix classes for 2D


//=============================================================================

//=============================================================================


inline double deg2rad(const double deg) { return deg/180.0f*(double)M_PI; }
inline double rad2deg(const double rad) { return rad*180.0f/(double)M_PI; }

//=============================================================================

/// \class vec2
/// This class implements a simple 2D vector, that we use to represent
/// 2D points and 3D color. You can access the individual components either by
/// x,y,z or by r,g,b. The vec2 class provides all commonly used mathematical
/// operations.
class vec2
{
public:

	/// this union allows us to access vector elements by either x,y,z or r,g,b,
	/// but we have to store only three doubles.
	union
	{
		/// XYZ coordinates
		struct
		{
			double x; ///< x coordinate
			double y; ///< y coordinate
		};
	};


public:

	/// default constructor
	vec2() {}

	/// construct with scalar value that is assigned to x, y, and z
	vec2(double _s) : x(_s), y(_s) {}

	/// construct with x,y,z values
	vec2(double _x, double _y) : x(_x), y(_y) {}

	/// return pointer to array/vector (for passing it to OpenGL)
	const double* data() const { return &x; }

	/// read/write the _i'th vector component (_i from 0 to 2)
	double& operator[](unsigned int _i)
	{
		assert(_i < 2);
		return (&x)[_i];
	}

	/// read the _i'th vector component (_i from 0 to 2)
	const double operator[](unsigned int _i) const
	{
		assert(_i < 2);
		return (&x)[_i];
	}


	/// multiply this vector by a scalar \c s
	vec2& operator*=(const double s)
	{
		x *= s;
		y *= s;
		return *this;
	}

	/// divide this vector by a scalar \c s
	vec2& operator/=(const double s)
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
};

using Segment = std::pair< vec2, vec2 >;

//-----------------------------------------------------------------------------


/// unary minus: turn v into -v
inline const vec2 operator-(const vec2& v)
{
	return vec2(-v.x, -v.y);
}

/// multiply vector \c v by scalar \c s
inline const vec2 operator*(const double s, const vec2& v )
{
	return vec2(s * v.x,
				s * v.y);
}

/// multiply vector \c v by scalar \c s
inline const vec2 operator*(const vec2& v, const double s)
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
inline const vec2 operator/(const vec2& v, const double s)
{
	return vec2(v.x / s,
				v.y / s);
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

/// compute the squared Euclidean norm (length) of a vector \c v
inline const double norm2(const vec2& v)
{
	return v.x*v.x + v.y*v.y;
}

/// compute the Euclidean norm (length) of a vector \c v
inline const double norm(const vec2& v)
{
	return sqrt(norm2(v));
}

/// normalize vector \c v by dividing it by its norm
inline const vec2 normalize(const vec2& v)
{
	const double n = norm(v);
	if (n != 0.0)
	{
		return vec2(v.x / n,
					v.y / n);
	}
	return v;
}

/// compute the distance between vectors \c v0 and \c v1
inline const double distance(const vec2& v0, const vec2& v1)
{
	return norm(v0-v1);
}

/// compute the Euclidean dot product of \c v0 and \c v1
inline const double dot(const vec2& v0, const vec2& v1)
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

/// \class mat2
/// This class implements a simple 2x2 matrix.
class mat2
{
private:

	/// the data is stored as an array of 9 values
	double data_[4];

public:

	/// default constructor
	mat2() {}

	mat2(double m00, double m01, double m10, double m11);

	/// construct matrix where all values are s
	explicit mat2(double s);

	/// read/write access to element (i,j)
	double& operator()(const int i, const int j) { return data_[i+j*2]; }

	/// read access to element (i,j)
	double operator()(const int i, const int j) const { return data_[i+j*2]; }

	/// pointer to data (for passing it to OpenGL)
	const double* data() const { return data_; }
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
#endif
//=============================================================================

