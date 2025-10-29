#pragma once

#include "Collision2D.h"

#include "Math/Geometry2D/Utils.h"

// Constructors

Collision2D::Collision2D(std::vector<Vector2> verts):
    o_verts(verts),
    o_centroid(Math::Geometry::polygonCentroid(verts)),
    Object2D()
{}

Collision2D::Collision2D(Transform2D* transform, std::vector<Vector2> verts):
    o_verts(verts),
    o_centroid(Math::Geometry::polygonCentroid(verts)),
    Object2D(transform)
{}

Collision2D::Collision2D(Collision2D* other):
    o_verts(other->o_verts),
    o_centroid(other->o_centroid),
    Object2D(other->transform)
{}

Collision2D::Collision2D(Transform2D* transform, Collision2D* other):
    o_verts(other->o_verts),
    o_centroid(other->o_centroid),
    Object2D(transform)
{}

// Methods

std::vector<Vector2> Collision2D::getVertices() const {
    return transform->apply(o_verts);
}

Vector2 Collision2D::getCentroid() const {
    return transform->apply(o_centroid);
}

void Collision2D::onRender(Renderer* renderer) const {
    // DEBUG
    renderer->DrawPolygonLines(getVertices(), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
}
