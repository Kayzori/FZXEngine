#pragma once

#include "../../Algebra/Vector/Vector2.h"
#include "../../Algebra/Matrix/Matrix3.h"
#include <vector>

class Transform2D
{
public:
    // Properties
    Vector2 position;
    float rotation; // in degrees
    Vector2 scale;

    // Constructors
    Transform2D();
    Transform2D(const Vector2& position, float rotation, const Vector2& scale);
    Transform2D(const Matrix3& matrix);
    Transform2D(const Transform2D& other);

    // Methods
    // n: number of points
                                                                                        // Time Complexity
    Vector2 apply(const Vector2& point) const;                                          // Best O(1), Average O(1), Worst O(1)
    std::vector<Vector2> apply(const std::vector<Vector2>& points) const;               // Best O(n), Average O(n), Worst O(n)

    Vector2 applyTranslation(const Vector2& point) const;                               // Best O(1), Average O(1), Worst O(1)
    std::vector<Vector2> applyTranslation(const std::vector<Vector2>& points) const;    // Best O(n), Average O(n), Worst O(n)

    Vector2 applyRotation(const Vector2& point) const;                                  // Best O(1), Average O(1), Worst O(1)
    std::vector<Vector2> applyRotation(const std::vector<Vector2>& points) const;       // Best O(n), Average O(n), Worst O(n)

    Vector2 applyScale(const Vector2& point) const;                                     // Best O(1), Average O(1), Worst O(1)
    std::vector<Vector2> applyScale(const std::vector<Vector2>& points) const;          // Best O(n), Average O(n), Worst O(n)

    Vector2 applyInverse(const Vector2& point) const;                                   // Best O(1), Average O(1), Worst O(1)
    std::vector<Vector2> applyInverse(const std::vector<Vector2>& points) const;        // Best O(n), Average O(n), Worst O(n)

    Matrix3 matrix() const;                                                             // Best O(1), Average O(1), Worst O(1)
    void setMatrix(const Matrix3& matrix);                                              // Best O(1), Average O(1), Worst O(1)

    // Relational operators
    bool operator==(const Transform2D& other) const;                                    // Best O(1), Average O(1), Worst O(1)
    bool operator!=(const Transform2D& other) const;                                    // Best O(1), Average O(1), Worst O(1)
};

