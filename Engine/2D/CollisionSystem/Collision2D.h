#pragma once

#include "../Node2D.h"
#include "Collision2DManifold.h"

#include "Math/Geometry2D/Primitives/Primitives.h"

#include "API/Renderer/Renderer.h"

#include <vector>

// Forward declaration
class Collision2DSystem;

// Base collision class for 2d physics
class Collision2D : public Node2D
{
private:
    // Properties
    Collision2DSystem* sys;
    std::vector<Vector2> o_verts;
    Vector2 o_centroid;
    float radius;

public:
    // Properties
    PrimitiveShape2D p_shape;

    // Constructors and Destructors
    Collision2D(Collision2DSystem* col_sys, std::vector<Vector2> verts, PrimitiveShape2D p_shape = PrimitiveShape2D::CONVEX_POLYGON2D);
    Collision2D( Transform2D* transform, Collision2DSystem* col_sys, std::vector<Vector2> verts, PrimitiveShape2D p_shape = PrimitiveShape2D::CONVEX_POLYGON2D);

    Collision2D(Collision2DSystem* col_sys, Collision2D* other);
    Collision2D(Transform2D* transform, Collision2DSystem* col_sys, Collision2D* other);

    ~Collision2D();

    // Methods
                                                    // Time Complexity
    float getRadius() const;                        // Best O(1), Average O(1), Worst O(1) 
    std::vector<Vector2> getOVertices() const;      // Best O(1), Average O(1), Worst O(1)
    Vector2 getOCentroid() const;                   // Best O(1), Average O(1), Worst O(1)                                          
    std::vector<Vector2> getVertices() const;       // Best O(n), Average O(n), Worst O(n)
    Vector2 getCentroid() const;                    // Best O(1), Average O(1), Worst O(1)

    Collision2DSystem* getSys() const;          // Best O(1), Average O(1), Worst O(1)

    void onRender(Renderer* renderer) override;     // Best O(n), Average O(n), Worst O(n)
};
