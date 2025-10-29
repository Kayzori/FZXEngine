#pragma once

// Engine.h

// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLAD_INCLUDED

// Math
#define MATH_INCLUDE_ALGEBRA
#define MATH_INCLUDE_VECTOR_UTILS
#define MATH_INCLUDE_MATRIX_UTILS
#define MATH_INCLUDE_GEOMETRY2D
#define MATH_INCLUDE_GEOMETRY2D_UTILS
#define MATH_INCLUDE_TRIGONOMETRY_UTILS
#include "Math/Math.h"

// Custom API
#include "API/Renderer/Renderer.h"

// SYS
#include "2D/CollisionSystem/Collision2D.h"
