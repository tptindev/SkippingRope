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
	int16_t order_in_layer;
	bool flip;
	const char* source;
	void* bitmap;
	void* _ptr; // sprite pointer
} Sprite;

typedef struct {
	int16_t order_in_layer;
	int frame_x; // x of first frame
	int frame_y; // y of first frame
	int frame_width;
	int frame_height;
	int frame_count;
	const char* source;
	void* bitmap;
	void* _ptr; // sprite pointer
} AnimatedSprite;

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
	Sprite* sprite;
	AnimatedSprite* animated_sprite;
} Components;

#endif // __COMPONENT_H__
