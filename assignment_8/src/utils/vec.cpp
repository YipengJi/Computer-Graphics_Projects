//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#include "vec.h"

//=============================================================================


mat2::mat2(const double s)
{
	for (int i=0; i<4; ++i)
		data_[i] = s;
}

mat2::mat2(double m00, double m01, double m10, double m11)
{
	auto& t = *this;
	t(0, 0) = m00;
	t(0, 1) = m01;
	t(1, 0) = m10;
	t(1, 1) = m11;
}


//-----------------------------------------------------------------------------


mat2 operator*(const mat2& m0, const mat2& m1)
{
	mat2 m;

	for (int i=0; i<2; ++i)
	{
		for (int j=0; j<2; ++j)
		{
			m(i,j) = 0.0f;
			for (int k=0; k<2; ++k)
				m(i,j) += m0(i,k) * m1(k,j);
		}
	}

	return m;
}

vec2 operator*(const mat2& m, const vec2& v)
{
	vec2 result;
	for (int i=0; i<2; ++i)
	{
		result[i] = 0.;
		for (int k=0; k<2; ++k)
			result[i] += m(i,k) * v[k];
	}

	return result;
}


//-----------------------------------------------------------------------------

mat2 transpose(const mat2& m)
{
	mat2 mt;
	for(int i=0; i<2; i++)
		for(int j=0; j<2; j++)
			mt(i,j) = m(j,i);
	return mt;
}

mat2 inverse(const mat2& m)
{
	double const det = m(0, 0) * m(1, 1) - m(0, 1)*m(1, 0);

	return {
		m(1, 1) / det, -m(0, 1) / det,
		-m(1, 0) / det, m(0, 0) / det
	};
}


std::ostream& operator<<(std::ostream& os, const mat2& m)
{
	for(int i=0; i<2; i++)
	{
		for(int j=0; j<2; j++)
			os << m(i,j) << ' ';
		os << std::endl;
	}
	return os;
}

//=============================================================================

//=============================================================================
