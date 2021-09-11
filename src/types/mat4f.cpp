#include "mat4f.hpp"

Mat4f::Mat4f() : r1{Vec4f{}}, r2{Vec4f{}}, r3{Vec4f{}}, r4{Vec4f{}}
{
}

Mat4f::Mat4f(const Vec4f &r1, const Vec4f &r2, const Vec4f &r3, const Vec4f &r4)
    : r1{r1}, r2{r2}, r3{r3}, r4{r4}
{
}

Mat4f::Mat4f(const Mat4f &m) : r1{m.r1}, r2{m.r2}, r3{m.r3}, r4{m.r4}
{
}

Mat4f Mat4f::Transpose() const
{
    return Mat4f{
        Vec4f{r1.x, r2.x, r3.x, r4.x},
        Vec4f{r1.y, r2.y, r3.y, r4.y},
        Vec4f{r1.z, r2.z, r3.z, r4.z},
        Vec4f{r1.w, r2.w, r3.w, r4.w}};
}

Mat4f Mat4f::Cofactor() const
{
    Mat4f adjoint;
    for (size_t i{0}; i < 4; ++i)
    {
        for (size_t j{0}; j < 4; ++j)
        {
            float_psp sign{(i + j) % 2 ? -1.0f : 1.0f};
            adjoint(i, j) = sign * Minor(i, j).Determinant();
        }
    }

    return adjoint;
}

float_psp Mat4f::Determinant() const
{
    const Mat4f &m{*this};
    float_psp d00{m(0, 0) * Minor(0, 0).Determinant()};
    float_psp d10{-m(1, 0) * Minor(1, 0).Determinant()};
    float_psp d20{m(2, 0) * Minor(2, 0).Determinant()};
    float_psp d30{-m(3, 0) * Minor(3, 0).Determinant()};
    return d00 + d10 + d20 + d30;
}

Mat3f Mat4f::Minor(size_t i, size_t j) const
{
    Mat3f minor;
    size_t iIndex{0};
    for (size_t _i{0}; _i < 4; ++_i)
    {
        if (_i == i)
        {
            continue;
        }

        size_t jIndex{0};
        for (size_t _j{0}; _j < 4; ++_j)
        {
            if (_j == j)
            {
                continue;
            }

            minor(iIndex, jIndex) = (*this)(_i, _j);
            ++jIndex;
        }
        ++iIndex;
    }

    return minor;
}

bool Mat4f::Inverse(Mat4f &out) const
{
    float_psp determinant{Determinant()};
    if (determinant == 0.0f)
    {
        return false;
    }

    out = (1.0f / determinant) * Cofactor().Transpose();
    return true;
}

Vec4f &Mat4f::operator()(size_t i)
{
    switch (i)
    {
    case 0:
        return r1;
    case 1:
        return r2;
    case 2:
        return r3;
    case 3:
        return r4;
    default:
        exit(-1);
    }
}

Vec4f Mat4f::operator()(size_t i) const
{
    switch (i)
    {
    case 0:
        return r1;
    case 1:
        return r2;
    case 2:
        return r3;
    case 3:
        return r4;
    default:
        exit(-1);
    }
}

float_psp &Mat4f::operator()(size_t i, size_t j)
{
    Vec4f &row{(*this)(i)};
    return row(j);
}

float_psp Mat4f::operator()(size_t i, size_t j) const
{
    const Vec4f &row{(*this)(i)};
    return row(j);
}

Mat4f operator*(float_psp scalar, const Mat4f &m)
{
    return Mat4f{
        scalar * m.r1,
        scalar * m.r2,
        scalar * m.r3,
        scalar * m.r4};
}

Vec4f operator*(const Mat4f &m, const Vec4f &v)
{
    return Vec4f{
        Vec4f::Dot(m.r1, v),
        Vec4f::Dot(m.r2, v),
        Vec4f::Dot(m.r3, v),
        Vec4f::Dot(m.r4, v)};
}

Mat4f operator*(const Mat4f &a, const Mat4f &b)
{
    Mat4f bT{b.Transpose()};
    Mat4f abT{Mat4f{
        Vec4f{a * bT.r1},
        Vec4f{a * bT.r2},
        Vec4f{a * bT.r3},
        Vec4f{a * bT.r4}}};

    return abT.Transpose();
}

std::ostream &operator<<(std::ostream &out, const Mat4f &m)
{
    out << "[" << m.r1 << "\n"
        << m.r2 << "\n"
        << m.r3 << "\n"
        << m.r4 << "]";

    return out;
}

const Mat4f Mat4f::IDENTITY{
    Vec4f{1.0f, 0.0f, 0.0f, 0.0f},
    Vec4f{0.0f, 1.0f, 0.0f, 0.0f},
    Vec4f{0.0f, 0.0f, 1.0f, 0.0f},
    Vec4f{0.0f, 0.0f, 0.0f, 1.0f}};