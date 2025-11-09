#pragma once

#include "API/Renderer/Renderer.h"

struct GLFWwindow;

// Base class of all engine's components and entities
class Node
{
public:
    Node() = default;
    ~Node() = default;

    virtual void onUpdate(float delta_time) {};
    virtual void onRender(Renderer* renderer) {};
    virtual void onInputEvent(GLFWwindow* win, int key, int scan_code, int action, int mods) {};
};
