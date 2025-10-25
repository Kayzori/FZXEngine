#include "Circle2D.h"

#define MATH_TRIGONOMETRY_UTILS
#include "Math/Math.h"

using namespace Math;

Circle2D::Circle2D(const float radius, int precision = 3) :
    Shape2D()
{
    precision = (precision < 3) ? 3 : precision;
    const double rad_angle = TWO_PI / precision;
    o_verts.clear();
    for (int i = 0; i < precision; i++) {
        o_verts.push_back(rotate({0, radius}, rad_angle * i));
    }
    o_centroid = {0, 0};
}

Circle2D::Circle2D(Transform2D* _transform, const float radius, int precision = 3) :
    Shape2D(_transform)
{
    precision = (precision < 3) ? 3 : precision;
    const double rad_angle = TWO_PI / precision;
    o_verts.clear();
    for (int i = 0; i < precision; i++) {
        o_verts.push_back(rotate({0, radius}, rad_angle * i));
    }
    o_centroid = {0, 0};
}