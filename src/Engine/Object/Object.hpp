#pragma once

class Object
{
public:
    inline virtual void OnReady() {};
    inline virtual void OnUpdate(float delta) {};
    inline virtual void OnDraw() {};
    Object() = default;
    virtual ~Object() = default;
};
