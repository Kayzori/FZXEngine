#pragma once

#include "../Algebra/Vector/Vector2.h"
#include <vector>

namespace Math {
    namespace Geometry
    {
        Vector2 polygonCentroid(const std::vector<Vector2>& verts);
    }
}