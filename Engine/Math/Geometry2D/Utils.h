#pragma once

#include "../Algebra/Vector/Vector2.h"
#include <vector>

namespace Math {
    namespace Geometry
    {
        float polygonArea(const std::vector<Vector2>& verts);
        Vector2 polygonCentroid(const std::vector<Vector2>& verts);
        float polygonRadius(const std::vector<Vector2>& verts, const Vector2& from);
    }
}