#pragma once
#include <Math/Math.hpp>
#include <Engine/Renderer/2D/Renderer2D.hpp>
#include <Engine/Component/Component.h>

// BASE CLASS
class Object
{
public:
    // Con/De structor
    std::string name;

    Object() = default;
    virtual ~Object() = default;

    // GameLoop
    virtual void OnReady() {};
    virtual void OnUpdate(float delta) {};
    virtual void OnPhysicsUpdate(float delta) {};
    virtual void OnDraw() {};
};
