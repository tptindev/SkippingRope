#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <stddef.h>
#include "../Physics2D/Math/Vector2D.h"
#include "../Physics2D/Math/Shape2D.h"
#include "../Physics2D/DataStructs/Array1D.h"

typedef enum
{
	e_static,
	e_dynamic,
	e_kinematic
} BodyType;

typedef enum
{
	NORMAL,
    ACTIVE,
    PUSHED,
    RELEASE,
    DISABLE,
} BtnState;

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
	int frame_index;
	int frame_width;
	int frame_height;
	int frame_count;
	int16_t order_in_layer;
	const char* source;
	void* (*bitmaps);
	void* _ptr; // sprite pointer
    bool running;
} AnimatedSprite;

typedef struct {
	BodyType type;
	float mass;
	float gravity_scale;
} Regidbody;

typedef struct {
    int event_id;
	Shape2D shape;
	Vec2 offset; // distance from collider center to other
    bool collided;
} Collider;

typedef struct {
    int event_id;
	float current;
    float max;
} Health;

typedef struct {
    float damage;
} Strength;

typedef struct {
    int event_id; // Event { id, fn }
	int16_t order_in_layer;
    BtnState state;
    const char* imgdir;
    void* (*bitmaps);
    char bitmaps_cnt;
    void* sprite;
} ButtonImage;

typedef struct {
    int event_id;
    Array1D* others;
} Binding;

typedef struct {
    int current;
    int high;
} ScoreBoard;

typedef struct {
    int length;
    int16_t order_in_layer;
    void* bitmaps[10];
    void* sprites[6];
} ScoreBoardVisual;

typedef struct {
	Transform* transform;
	KeyInput* input;
	Collider* collider;
	Motion* motion;
	Sprite* sprite;
	AnimatedSprite* animated_sprite;
	Health* health;
    ButtonImage* button_img;
    Strength* strength;
    Binding* binding;
    ScoreBoard* score_board;
    ScoreBoardVisual* score_board_visual;
} Components;

#endif // __COMPONENT_H__
