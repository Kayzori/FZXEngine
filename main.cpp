// Windowed entrypoint - runs Engine with a GLFW window and the Collision2DSystem
#include <iostream>
#include <vector>

#include "Engine/Engine.h"
#include "Engine/2D/CollisionSystem/Collision2DSystem.h"
#include "Engine/2D/CollisionSystem/Collision2D.h"
#include "Engine/Math/Geometry2D/Primitives/Circle2D.h"
#include "Engine/Math/Geometry2D/Primitives/Rect2D.h"

int main() {
    // Create and init the engine (window + renderer)
    Engine engine(1024, 768, "FZXEngine - Window");
    if (!engine.init()) {
        std::cerr << "Failed to initialize engine\n";
        return -1;
    }

    // Create collision system and register it with the engine so its onUpdate is called
    Collision2DSystem* colsys = new Collision2DSystem();
    engine.addNode(colsys);

    // Create collision objects and register them with both the collision system (done in constructor)
    // and with the engine so they will be rendered
    std::vector<Collision2D*> objs;

    for (int i = 0; i < 10; ++i) {
        Circle2D circ(30.0f);
        auto verts = circ.getVertices(16);
        Collision2D* c = new Collision2D(colsys, verts, PrimitiveShape2D::CIRCLE2D);
        c->transform->position = {100.0f + i * 40.0f, 300.0f};
        engine.addNode(c);
        objs.push_back(c);
    }

    for (int i = 0; i < 10; ++i) {
        Rect2D r({60.0f, 60.0f});
        auto verts = r.getVertices(false, {0,0});
        Collision2D* b = new Collision2D(colsys, verts, PrimitiveShape2D::CONVEX_POLYGON2D);
        b->transform->position = {120.0f + i * 40.0f, 320.0f};
        engine.addNode(b);
        objs.push_back(b);
    }

    // A couple of isolated shapes
    Rect2D isoRect({50.0f, 50.0f});
    auto isoVerts = isoRect.getVertices(false, {0,0});
    Collision2D* iso = new Collision2D(colsys, isoVerts, PrimitiveShape2D::CONVEX_POLYGON2D);
    iso->transform->position = {800.0f, 100.0f};
    engine.addNode(iso);
    objs.push_back(iso);

    Circle2D isoCirc(20.0f);
    auto isoCV = isoCirc.getVertices(12);
    Collision2D* iso2 = new Collision2D(colsys, isoCV, PrimitiveShape2D::CIRCLE2D);
    iso2->transform->position = {900.0f, 500.0f};
    engine.addNode(iso2);
    objs.push_back(iso2);

    // Run the engine (this will call onUpdate/onRender for Collision2DSystem and Collision2D nodes)
    engine.run();

    // Cleanup
    for (auto* o : objs) delete o;
    delete colsys;

    return 0;
}

