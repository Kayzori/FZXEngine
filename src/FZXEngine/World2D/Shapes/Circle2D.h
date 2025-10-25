#include "Shape2D.h"

class Circle2D : public Shape2D
{
public:
    // Constructors and Destructors
    Circle2D(const float radius, int precision = 3);
    Circle2D(Transform2D* tranform, const float radius, int precision = 3);
    ~Circle2D() = default;
};
