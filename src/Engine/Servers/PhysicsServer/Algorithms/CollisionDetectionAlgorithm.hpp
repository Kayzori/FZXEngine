#pragma once
#include "Engine/Object/2D/Collision2D.hpp"

namespace CDA
{
    Collision2DInfos Detect(Collision2D* A, Collision2D* B);
    Collision2DInfos CCCD(Collision2D* A, Collision2D* B);
    Collision2DInfos CPCD(Collision2D* circle, Collision2D* poly, Collision2D* ref);
    Collision2DInfos PPCD(Collision2D* A, Collision2D* B);
}
