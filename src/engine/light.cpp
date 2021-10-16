#include "light.hpp"

PointLight::PointLight(const Vec3f &position, float_psp r, const Vec4f &color) : position{position}, r{r}, color{color}
{
}