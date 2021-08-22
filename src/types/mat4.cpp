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

Vec4f operator*(const Mat4f &m, const Vec4f &v)
{
    /*
    std::cout << Vec4f::Dot(m.r1, v) << "\n";
    std::cout << Vec4f::Dot(m.r2, v) << "\n";
    std::cout << Vec4f::Dot(m.r3, v) << "\n";
    std::cout << Vec4f::Dot(m.r4, v) << "\n\n";
    */
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