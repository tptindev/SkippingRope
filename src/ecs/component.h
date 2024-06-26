#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <stddef.h>
#include "../Physics2D/Math/Vector2D.h"
#include "../Physics2D/Math/Shape2D.h"

typedef enum
{
	e_static,
	e_dynamic,
	e_kinematic
} BodyType;

typedef struct {
	Vec2 position;
	Vec2 rotation;
	Vec2 scale;
} Transform;

typedef struct {
	bool left;
	bool right;
	bool up;
	bool down;
	bool a;
	bool b;
	bool crank;
} KeyInput;

typedef struct {
	BodyType type;
	float mass;
	float gravity_scale;
} Regidbody;

typedef struct {
	Shape2D shape;
} BoxCollider;

typedef struct {
	Transform* transform;
	KeyInput* input;
	BoxCollider* collider;
} Components;

#endif // __COMPONENT_H__
