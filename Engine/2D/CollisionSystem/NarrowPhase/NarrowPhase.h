#pragma once

// Forward declaration
class Collision2D;
class Collision2DManifold;

namespace NarrowPhase
{
    // SAT Collision Detection Algorithm
                                                                                // Time Complexity
    Collision2DManifold SAT(Collision2D* A, Collision2D* B);                    // Best O(n + m), Average O(n + m), Worst O((n + m)²)

    // Circle vs Circle
    Collision2DManifold CircleXCircle(Collision2D* A, Collision2D* B);          // Best O(), Average O(), Worst O()

    // Circle vs Polygon
    Collision2DManifold CircleXPolygon(Collision2D* A, Collision2D* B);         // Best O(), Average O(), Worst O()
};
