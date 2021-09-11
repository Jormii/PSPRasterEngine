#include "mat3f.hpp"

Mat3f::Mat3f() : r1{Vec3f{}}, r2{Vec3f{}}, r3{Vec3f{}}
{
}

Mat3f::Mat3f(const Vec3f &r1, const Vec3f &r2, const Vec3f &r3) : r1{r1}, r2{r2}, r3{r3}
{
}

Mat3f::Mat3f(const Mat3f &m) : r1{m.r1}, r2{m.r2}, r3{m.r3}
{
}

float_psp Mat3f::Determinant() const
{
    const Mat3f &m{*this};
    float_psp pos{m(0, 0) * m(1, 1) * m(2, 2) + m(0, 1) * m(1, 2) * m(2, 0) + m(0, 2) * m(1, 0) * m(2, 1)};
    float_psp neg{m(2, 0) * m(1, 1) * m(0, 2) + m(2, 1) * m(1, 2) * m(0, 0) + m(2, 2) * m(1, 0) * m(0, 1)};
    return pos - neg;
}

Vec3f &Mat3f::operator()(size_t i)
{
    switch (i)
    {
    case 0:
        return r1;
    case 1:
        return r2;
    case 2:
        return r3;
    default:
        exit(-1);
    }
}

Vec3f Mat3f::operator()(size_t i) const
{
    switch (i)
    {
    case 0:
        return r1;
    case 1:
        return r2;
    case 2:
        return r3;
    default:
        exit(-1);
    }
}

float_psp &Mat3f::operator()(size_t i, size_t j)
{
    Vec3f &row{(*this)(i)};
    return row(j);
}

float_psp Mat3f::operator()(size_t i, size_t j) const
{
    const Vec3f &row{(*this)(i)};
    return row(j);
}

std::ostream &operator<<(std::ostream &out, const Mat3f &m)
{
    out << "[" << m.r1 << "\n"
        << m.r2 << "\n"
        << m.r3 << "]";

    return out;
}