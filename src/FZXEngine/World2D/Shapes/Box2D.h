#include "Shape2D.h"

class Box2D : public Shape2D
{
public:
    // Constructors and Destructors
    Box2D(const Vector2& size) :
        Shape2D()
    {
        Vector2 hsize = size / 2;
        o_verts = {
            {-hsize.x, -hsize.y},
            {hsize.x, -hsize.y},
            {hsize.x, hsize.y},
            {-hsize.x, hsize.y}
        };
        o_centroid = {0, 0};
    }
    Box2D(Transform2D* transform, const Vector2& size) :
        Shape2D(transform)
    {
        Vector2 hsize = size / 2;
        o_verts = {
            {-hsize.x, -hsize.y},
            {hsize.x, -hsize.y},
            {hsize.x, hsize.y},
            {-hsize.x, hsize.y}
        };
        o_centroid = {0, 0};
    }
    ~Box2D() {}
};