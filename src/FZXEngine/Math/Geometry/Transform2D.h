#pragma once

#include "../Algebra/Vector2.h"
#include "../Algebra/Matrix3.h"
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
    Vector2 apply(const Vector2& point) const;
    std::vector<Vector2> apply(const std::vector<Vector2>& points) const;

    Vector2 applyTranslation(const Vector2& point) const;
    std::vector<Vector2> applyTranslation(const std::vector<Vector2>& points) const;

    Vector2 applyRotation(const Vector2& point) const;
    std::vector<Vector2> applyRotation(const std::vector<Vector2>& points) const;

    Vector2 applyScale(const Vector2& point) const;
    std::vector<Vector2> applyScale(const std::vector<Vector2>& points) const;

    Vector2 applyInverse(const Vector2& point) const;
    std::vector<Vector2> applyInverse(const std::vector<Vector2>& points) const;

    Matrix3 matrix() const;
    void setMatrix(const Matrix3& matrix);

    // Relational operators
    bool operator==(const Transform2D& other) const;
    bool operator!=(const Transform2D& other) const;
};

