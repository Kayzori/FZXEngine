#pragma once

#include "../Node.h"

#include "Math/Geometry2D/Core/Transform2D.h"

// Base class for all 2D components/entities/systems
class Node2D : public Node
{
private:
    // Flags
    bool transformOwned;
public:
    // Properties
    Transform2D* transform;

    // Constructors and Destructors
    Node2D() :
    transform(new Transform2D()),
    transformOwned(true)
    {}

    Node2D(Transform2D* transform) :
    transform(new Transform2D()),
    transformOwned(true)
    {}
    
    virtual ~Node2D() {
        if (transformOwned) delete transform;
    }

    // Methods
    virtual void setTransform(Transform2D* _transform) {
        transformOwned = false;
        transform = _transform;
    }
};
