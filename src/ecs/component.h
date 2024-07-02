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
	Vec2 last_position;
	Vec2 direction;
	Vec2 acceleration;
} Motion;

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
	const char* sprite;
} SpriteRenderer;

typedef struct {
	BodyType type;
	float mass;
	float gravity_scale;
} Regidbody;

typedef struct {
	Shape2D shape;
	Vec2 offset; // distance from collider center to other
} Collider;

typedef struct {
	Transform* transform;
	KeyInput* input;
	Collider* collider;
	Motion* motion;
} Components;

#endif // __COMPONENT_H__
