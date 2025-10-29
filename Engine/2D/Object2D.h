#pragma once

struct GLFWwindow;

#include "Math/Geometry2D/Core/Transform2D.h"

// Base Class

class Object2D
{
private:
    // Flags
    bool transformOwned;
public:
    // Properties
    Transform2D* transform;

    // Constructors and Destructors
    Object2D() :
    transform(new Transform2D()),
    transformOwned(true)
    {}

    Object2D(Transform2D* transform) :
    transform(new Transform2D()),
    transformOwned(true)
    {}
    
    virtual ~Object2D() {
        if (transformOwned) delete transform;
    }

    // Methods
    virtual void onUpdate(double delta_time) {};
    // Use forward-declared GLFWwindow to avoid requiring GLFW in this header
    virtual void onInputEvent(GLFWwindow* win, int key, int scan_code, int action, int mods) {};
};
