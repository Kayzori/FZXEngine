#include "Collision2D.h"

#include "Collision2DSystem.h"

#include "Math/Geometry2D/Utils.h"

// Constructors and Destructors

Collision2D::Collision2D(Collision2DSystem* col_sys, std::vector<Vector2> verts, PrimitiveShape2D p_shape):
    Node2D(),
    p_shape(p_shape),
    o_verts(verts),
    o_centroid(Math::Geometry::polygonCentroid(verts)),
    radius(Math::Geometry::polygonRadius(o_verts, o_centroid))
{
    col_sys->addObject(this);
    sys = col_sys;
}

Collision2D::Collision2D(Transform2D* transform, Collision2DSystem* col_sys, std::vector<Vector2> verts, PrimitiveShape2D p_shape):
    Node2D(transform),
    p_shape(p_shape),
    o_verts(verts),
    o_centroid(Math::Geometry::polygonCentroid(verts)),
    radius(Math::Geometry::polygonRadius(o_verts, o_centroid))
{
    col_sys->addObject(this);
    sys = col_sys;
}

Collision2D::Collision2D(Collision2DSystem* col_sys, Collision2D* other):
    Node2D(other->transform),
    p_shape(other->p_shape),
    o_verts(other->o_verts),
    o_centroid(other->o_centroid),
    radius(Math::Geometry::polygonRadius(o_verts, o_centroid))
{
    col_sys->addObject(this);
    sys = col_sys;
}

Collision2D::Collision2D(Transform2D* transform, Collision2DSystem* col_sys, Collision2D* other):
    Node2D(transform),
    p_shape(other->p_shape),
    o_verts(other->o_verts),
    o_centroid(other->o_centroid),
    radius(Math::Geometry::polygonRadius(o_verts, o_centroid))
{
    col_sys->addObject(this);
    sys = col_sys;
}

Collision2D::~Collision2D()
{
    sys->removeObject(this);
}

// Methods

float Collision2D::getRadius() const {
    return radius;
}

std::vector<Vector2> Collision2D::getOVertices() const {
    return o_verts;
}

Vector2 Collision2D::getOCentroid() const {
    return o_centroid;
}

std::vector<Vector2> Collision2D::getVertices() const {
    return transform->apply(o_verts);
}

Vector2 Collision2D::getCentroid() const {
    return transform->apply(o_centroid);
}

Collision2DSystem* Collision2D::getSys() const {
    return sys;
}

void Collision2D::onRender(Renderer* renderer) {
    renderer->drawPolygonSegments(getVertices(), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
}
