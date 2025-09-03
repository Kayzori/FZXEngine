#pragma once
#include <Scripting/Signal.hpp>

// BASE CLASS
class Object
{
public:
    // Con/De structor
    Object() = default;
    virtual ~Object() = default;

    // GameLoop
    virtual void OnReady() {};
    virtual void OnUpdate(float delta) {};
    virtual void OnPhysicsUpdate(float delta) {};
    virtual void OnDraw() {};
};
