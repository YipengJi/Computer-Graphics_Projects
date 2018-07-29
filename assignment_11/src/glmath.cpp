//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#include "glmath.h"

//=============================================================================


mat3::mat3(const float s)
{
    for (int i=0; i<9; ++i)
        data_[i] = s;
}


//-----------------------------------------------------------------------------


mat3::mat3(const mat4& m4)
{
    for (int i=0; i<3; ++i)
        for (int j=0; j<3; ++j)
            (*this)(i,j) = m4(i,j);
}


//-----------------------------------------------------------------------------


mat3 operator*(const mat3& m0, const mat3& m1)
{
    mat3 m;

    for (int i=0; i<3; ++i)
    {
        for (int j=0; j<3; ++j)
        {
            m(i,j) = 0.0f;
            for (int k=0; k<3; ++k)
                m(i,j) += m0(i,k) * m1(k,j);
        }
    }

    return m;
}

//-----------------------------------------------------------------------------

vec3 operator*(const mat3& m, const vec3& v)
{
    return vec3(
        m(0, 0) * v[0] + m(0, 1) * v[1] + m(0, 2) * v[2],
        m(1, 0) * v[0] + m(1, 1) * v[1] + m(1, 2) * v[2],
        m(2, 0) * v[0] + m(2, 1) * v[1] + m(2, 2) * v[2]
    );
}

//-----------------------------------------------------------------------------


mat3 transpose(const mat3& m)
{
    mat3 mt;
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            mt(i,j) = m(j,i);
    return mt;
}


//-----------------------------------------------------------------------------


mat3 inverse(const mat3& m)
{
    float det = (- m(0,0)*m(1,1)*m(2,2)
                 + m(0,0)*m(1,2)*m(2,1)
                 + m(1,0)*m(0,1)*m(2,2)
                 - m(1,0)*m(0,2)*m(2,1)
                 - m(2,0)*m(0,1)*m(1,2)
                 + m(2,0)*m(0,2)*m(1,1));

    mat3 inv;
    inv(0,0) = (m(1,2)*m(2,1) - m(1,1)*m(2,2)) / det;
    inv(0,1) = (m(0,1)*m(2,2) - m(0,2)*m(2,1)) / det;
    inv(0,2) = (m(0,2)*m(1,1) - m(0,1)*m(1,2)) / det;
    inv(1,0) = (m(1,0)*m(2,2) - m(1,2)*m(2,0)) / det;
    inv(1,1) = (m(0,2)*m(2,0) - m(0,0)*m(2,2)) / det;
    inv(1,2) = (m(0,0)*m(1,2) - m(0,2)*m(1,0)) / det;
    inv(2,0) = (m(1,1)*m(2,0) - m(1,0)*m(2,1)) / det;
    inv(2,1) = (m(0,0)*m(2,1) - m(0,1)*m(2,0)) / det;
    inv(2,2) = (m(0,1)*m(1,0) - m(0,0)*m(1,1)) / det;

    return inv;
}


//-----------------------------------------------------------------------------


std::ostream& operator<<(std::ostream& os, const mat3& m)
{
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
            os << m(i,j) << ' ';
        os << std::endl;
    }
    return os;
}


//=============================================================================


mat4::mat4(const float s)
{
    for (int i=0; i<16; ++i)
        data_[i] = s;
}

//-----------------------------------------------------------------------------

vec4 operator*(const mat4& m, const vec4& v0)
{
    vec4 v;

    for (int i=0; i<4; ++i)
    {
        v[i] =0.0;
        for (int j=0; j<4; ++j)
        {
            v[i] += m(i,j) * v0[j];
        }
    }

    return v;
}

//-----------------------------------------------------------------------------


mat4 operator*(const mat4& m0, const mat4& m1)
{
    mat4 m;

    for (int i=0; i<4; ++i)
    {
        for (int j=0; j<4; ++j)
        {
            m(i,j) = 0.0f;
            for (int k=0; k<4; ++k)
                m(i,j) += m0(i,k) * m1(k,j);
        }
    }

    return m;
}


//-----------------------------------------------------------------------------



mat4 mat4::identity()
{
    mat4 m(0.0f);
    m(0,0) = m(1,1) = m(2,2) = m(3,3) = 1.0f;

    return m;
}


//-----------------------------------------------------------------------------


mat4 mat4::frustum(float l, float r, float b, float t, float n, float f)
{
    mat4 m(0.0f);
    m(0,0) = 2.0f * n / (r-l);
    m(1,1) = 2.0f * n / (t-b);
    m(0,2) = (r+l) / (r-l);
    m(1,2) = (t+b) / (t-b);
    m(2,2) = (n-f) / (f-n);
    m(3,2) = -1.0f;
    m(2,3) = -2.0f * f * n / (f-n);

    return m;
}


//-----------------------------------------------------------------------------


mat4 mat4::perspective(float fovy, float aspect, float near, float far)
{
    float t = near * tan( fovy * (float)M_PI / 360.0f );
    float b = -t;
    float l = b * aspect;
    float r = t * aspect;

    return mat4::frustum(l, r, b, t, near, far);
}


//-----------------------------------------------------------------------------


mat4 mat4::look_at(const vec3& eye, const vec3& center, const vec3& up)
{
    vec3 z = normalize(eye-center);
    vec3 x = normalize(cross(up, z));
    vec3 y = normalize(cross(z, x));

    mat4 m;
    m(0,0)=x[0]; m(0,1)=x[1]; m(0,2)=x[2]; m(0,3)=-dot(x,eye);
    m(1,0)=y[0]; m(1,1)=y[1]; m(1,2)=y[2]; m(1,3)=-dot(y,eye);
    m(2,0)=z[0]; m(2,1)=z[1]; m(2,2)=z[2]; m(2,3)=-dot(z,eye);
    m(3,0)=0.0;  m(3,1)=0.0;  m(3,2)=0.0;  m(3,3)=1.0;

    return m;
}


//-----------------------------------------------------------------------------


mat4 mat4::translate(const vec3& t)
{
    mat4 m(0.0f);
    m(0,0) = m(1,1) = m(2,2) = m(3,3) = 1.0f;
    m(0,3) = t[0];
    m(1,3) = t[1];
    m(2,3) = t[2];

    return m;
}


//-----------------------------------------------------------------------------


mat4 mat4::rotate_x(float angle)
{
    float ca = cosf(angle * ((float)M_PI/180.0f));
    float sa = sinf(angle * ((float)M_PI/180.0f));

    mat4 m(0.0f);
    m(0,0) = 1.0f;
    m(1,1) = ca;
    m(1,2) = -sa;
    m(2,2) = ca;
    m(2,1) = sa;
    m(3,3) = 1.0f;

    return m;
}


//-----------------------------------------------------------------------------


mat4 mat4::rotate_y(float angle)
{
    float ca = cosf(angle * ((float)M_PI/180.0f));
    float sa = sinf(angle * ((float)M_PI/180.0f));

    mat4 m(0.0f);
    m(0,0) = ca;
    m(0,2) = sa;
    m(1,1) = 1.0f;
    m(2,0) = -sa;
    m(2,2) = ca;
    m(3,3) = 1.0f;

    return m;
}


//-----------------------------------------------------------------------------


mat4 mat4::rotate_z(float angle)
{
    float ca = cosf(angle * ((float)M_PI/180.0f));
    float sa = sinf(angle * ((float)M_PI/180.0f));

    mat4 m(0.0f);
    m(0,0) = ca;
    m(0,1) = -sa;
    m(1,0) = sa;
    m(1,1) = ca;
    m(2,2) = 1.0f;
    m(3,3) = 1.0f;

    return m;
}

//-----------------------------------------------------------------------------


mat4 mat4::scale(float factor)
{
    mat4 m(0.0f);
    m(0,0) = factor;
    m(1,1) = factor;
    m(2,2) = factor;
    m(3,3) = 1.0f;

    return m;
}


//-----------------------------------------------------------------------------


std::ostream& operator<<(std::ostream& os, const mat4& m)
{
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
            os << m(i,j) << ' ';
        os << std::endl;
    }
    return os;
}


//=============================================================================
