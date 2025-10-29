#pragma once

#include "../Object2D.h"
#include "Math/Algebra/Vector/Vector2.h"
#include "API/Renderer/Renderer.h"

#include <vector>

// Base Class

class Collision2D : public Object2D
{
private:
    // Properties
    std::vector<Vector2> o_verts;
    Vector2 o_centroid;

public:
    // Constructors and Destructors
    Collision2D(std::vector<Vector2> verts);
    Collision2D(Transform2D* transform, std::vector<Vector2> verts);

    Collision2D(Collision2D* other);
    Collision2D(Transform2D* transform, Collision2D* other);

    ~Collision2D() = default;

    // Methods
                                                    // Time Complexity
    std::vector<Vector2> getVertices() const;       // Best O(n), Average O(n), Worst O(n)
    Vector2 getCentroid() const;                    // Best O(1), Average O(1), Worst O(1)

    void onRender(Renderer* renderer) const;
};
