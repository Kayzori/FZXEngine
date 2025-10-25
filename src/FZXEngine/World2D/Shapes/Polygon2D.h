#include "Shape2D.h"

class Polygon2D : public Shape2D
{
private:
    // Help Methods
    Vector2 CalculateCentroid(const std::vector<Vector2>& verts) const;
public:
    // Constructors and Destructors
    Polygon2D(const std::vector<Vector2>& verts);
    Polygon2D(Transform2D* transform, const std::vector<Vector2>& verts);
    ~Polygon2D() = default;
};