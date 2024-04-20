#include "game.h"
#include "struct_defines.h"
#include "draw.h"
#include <math.h>
#include <stdlib.h>

#define EARTH_ON 1
#define MOON_ON 0
#define TESTING_ON 1

typedef enum e_direction
{
	TOP,
	RIGHT,
	BOTTOM,
	LEFT
} DIRECTION;

typedef struct edge
{
	DIRECTION direction;
	b2Vec2 from;
	b2Vec2 to;
} Edge;

const float FPS = 30.0;
float world_scale = 80.0f;
const unsigned int MAX_METEORITES = 32;
const double orbit_radius = 0.5f;
const Edge edge[4] =
{
	{TOP, {0,0}, {5,0}},
	{RIGHT, {5, 0}, {5, 3}},
	{BOTTOM, {0, 3}, {5, 3}},
	{LEFT, {0, 0}, {0, 3}}
};

static bool initialized = false;
static PlaydateAPI* api = NULL;
static b2WorldId worldId;
static unsigned int current_level = 0; // 0: nothing
static float last_crank_angle = 0.0f;
static unsigned int user_point = 0;
LevelObject levels[5] =
{
	{"2021", NULL, 500},
	{"2022", NULL, 400},
	{"2023", NULL, 250},
	{"2024", NULL, 200},
	{"2025", NULL, 100},
};


static GameObject* meteorites = NULL;
static GameObject earth_obj = { B2_ZERO_INIT, 2.5f, 1.5f, 0.0f, 0.0f, NULL, 1 }; // id, xcenter, ycenter, hw, hh, bitmap, num of bitmap
static GameObject moon_obj = { B2_ZERO_INIT, 2.5f, 0.0f, 0.0f, 0.0f, NULL, 1 }; // id, xcenter, ycenter, hw, hh, bitmap, num of bitmap
static GameObject box_obj = { B2_ZERO_INIT, 2.8f, 0.0f, 0.1f, 0.1f, NULL, 1 };

b2WorldId register_world(b2Vec2 gravity);
void register_bodies(b2WorldId world_id);
void unregister_body(b2BodyId bodyId);

static bool pre_solve_cb(b2ShapeId shapeIdA, b2ShapeId shapeIdB, b2Manifold* manifold, void* context);
static int button_event_cb(PDButtons button, int down, uint32_t when, void* userdata);
void generate_meteorite();



void game_initialize(void* userdata)
{
	initialized = true;
	api = userdata;
	api->display->setRefreshRate(FPS);
	api->system->setButtonCallback(button_event_cb, NULL, 5);

	b2Vec2 gravity = { 0.0f, 9.81f };
	worldId = register_world(gravity);
	if (b2World_IsValid(worldId))
	{
		register_bodies(worldId);
		b2World_SetPreSolveCallback(worldId, pre_solve_cb, NULL);
	}

	// register blocks stores 32 GameObjects
	meteorites = api->system->realloc(NULL, sizeof(GameObject) * MAX_METEORITES);
}

void game_update(float deltatime)
{
	if (b2World_IsValid(worldId))
	{
		float timeStep = 1.0f / FPS;
		b2World_Step(worldId, timeStep, 10);

		b2Vec2 pos = { 0.0f, 0.0f };
		b2Vec2 local_point = { 0.0f, 0.0f };
		b2Vec2 world_point = { 0.0f, 0.0f };
		b2Vec2 earth_center = { 0.0f, 0.0f };

#if EARTH_ON
		{ // earth
			earth_center = b2Body_GetPosition(earth_obj.id);
			b2Body_SetTransform(
				earth_obj.id,
				(b2Vec2) {
				earth_obj.x, earth_obj.y
			},
				(float)((api->system->getCurrentTimeMilliseconds() / 3) % 360)
			);
		}
#endif // EARTH_ON


#if MOON_ON
		{ // moon
			double angle_rad = api->system->getCrankAngle() * (3.14159265358979323846f / 180.0f);
			moon_obj.x = (float)(orbit_radius * cos(angle_rad) + earth_center.x);
			moon_obj.y = (float)(orbit_radius * sin(angle_rad) + earth_center.y);
			b2Body_SetTransform(
				moon_obj.id,
				(b2Vec2) {
				moon_obj.x, moon_obj.y
			},
				//api->system->getCrankAngle()
				0
			);
		}
#endif // MOON_ON


#if TESTING_ON
		{ // box for testing
			pos = b2Body_GetPosition(box_obj.id);
			box_obj.x = pos.x;
			box_obj.y = pos.y;
		}
#endif // TESTING_ON



		{ // meteorites
		}
	
		if (api->system->getCrankAngle() != last_crank_angle)
		{
			api->system->logToConsole("Moon position: %f %f", moon_obj.x, moon_obj.y);
			api->system->logToConsole("Box position: %f %f", box_obj.x, box_obj.y);
		}

		last_crank_angle = api->system->getCrankAngle();
	}
}

void game_draw()
{
	api->graphics->clear(kColorWhite);
	api->graphics->setBackgroundColor(kColorBlack);

#if EARTH_ON
	{ // earth
		b2Transform earth_trans = b2Body_GetTransform(earth_obj.id);
		drawRotationFrame(
			api,
			earth_obj.sprites,
			earth_trans.p.x,
			earth_trans.p.y,
			true,
			(float)((api->system->getCurrentTimeMilliseconds() / 3) % 360)
		);
	}
#endif // EARTH_ON

#if MOON_ON
	{ // moon
		b2Transform box_trans = b2Body_GetTransform(box_obj.id);
		drawRotationFrame(
			api,
			moon_obj.sprites,
			moon_trans.p.x,
			moon_trans.p.y,
			true,
			api->system->getCrankAngle()
		);
	}
#endif // MOON_ON


#if TESTING_ON
	{ // box for testing
		b2Transform box_trans = b2Body_GetTransform(box_obj.id);
		drawEllipse(api, box_trans.p.x, box_trans.p.y, box_obj.half_width * 2.0f, box_obj.half_height * 2.0f, 0, 0, kColorBlack);
	}
#endif // TESTING_ON



	{ // meteorites
	
	}

}

b2WorldId register_world(b2Vec2 gravity)
{
	api->system->logToConsole("Entry: %s", __FUNCTION__);
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = gravity;
	worldDef.enableSleep = true;
	worldDef.enableContinous = true;
	return b2CreateWorld(&worldDef);
}

void register_bodies(b2WorldId world)
{
	api->system->logToConsole("Entry: %s", __FUNCTION__);

	b2Vec2 pos = { 0.0f, 0.0f };
	b2ShapeId shapeId = b2_nullShapeId;
	b2BodyDef bodyDef;
	b2ShapeDef shapeDef;
	b2Polygon polygon; 
	b2Circle circle;
	int obj_width = 0;
	int obj_height = 0;

	(void)polygon;
	(void)circle;

#if EARTH_ON
	{ // earth
		const char* outerr = NULL;
		earth_obj.sprites = api->graphics->loadBitmap("images/earth.png", &outerr);
		if (outerr)
		{
			api->system->error("[EARTH] bitmap %s", outerr);
		}

		api->graphics->getBitmapData(earth_obj.sprites, &obj_width, &obj_height, 0, NULL, NULL);

		earth_obj.half_height = (float)(4 + obj_height / 2) / 80.0f;
		earth_obj.half_width = (float)(4 + obj_width / 2) / 80.0f;

		api->system->logToConsole("Earth pos: %f, %f  size: %f, %f", earth_obj.x, earth_obj.y, earth_obj.half_height, earth_obj.half_width);

		bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_staticBody;
		bodyDef.position.x = earth_obj.x;
		bodyDef.position.y = earth_obj.y;
		bodyDef.enableSleep = true;
		earth_obj.id = b2CreateBody(world, &bodyDef);

		circle.point = (b2Vec2){ 0.0f, 0.0f };
		circle.radius = earth_obj.half_width;

		shapeDef = b2DefaultShapeDef();
		shapeDef.density = 1.0f;
		shapeDef.friction = 0.3f;
		shapeId = b2CreateCircleShape(earth_obj.id, &shapeDef, &circle);

}
#endif // 1

#if MOON_ON
	{ // moon
		const char* outerr = NULL;
		moon_obj.sprites = api->graphics->loadBitmap("images/moon.png", &outerr);
		if (outerr)
		{
			api->system->error("[MOON] bitmap %s", outerr);
		}

		api->graphics->getBitmapData(moon_obj.sprites, &obj_width, &obj_height, 0, NULL, NULL);

		moon_obj.half_height = (float)(8 + obj_height / 2) / 80.0f;
		moon_obj.half_width = (float)(8 + obj_width / 2) / 80.0f;

		api->system->logToConsole("Moon pos: %f, %f  size: %f, %f", moon_obj.x, moon_obj.y, moon_obj.half_height, moon_obj.half_width);


		bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.x = moon_obj.x;
		bodyDef.position.y = moon_obj.y;
		bodyDef.enableSleep = true;
		moon_obj.id = b2CreateBody(world, &bodyDef);

		circle.point = (b2Vec2){ 0.0f, 0.0f };
		circle.radius = moon_obj.half_width;

		shapeDef = b2DefaultShapeDef();
		shapeDef.density = 1.0f;
		shapeDef.friction = 0.3f;
		shapeId = b2CreateCircleShape(moon_obj.id, &shapeDef, &circle);
}
#endif // 0
	

#if TESTING_ON
	{ // box for testing
		bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.x = box_obj.x;
		bodyDef.position.y = box_obj.y;
		bodyDef.enableSleep = true;
		box_obj.id = b2CreateBody(world, &bodyDef);

		circle.point = (b2Vec2){ 0.0f, 0.0f };
		circle.radius = box_obj.half_width;

		shapeDef = b2DefaultShapeDef();
		shapeDef.density = 1.0f;
		shapeDef.friction = 0.3f;
		shapeDef.restitution = 0.3f;
		shapeId = b2CreateCircleShape(box_obj.id, &shapeDef, &circle);
	}
#endif // 1
}

void unregister_body(b2BodyId bodyId)
{
}

static bool pre_solve_cb(b2ShapeId shapeIdA, b2ShapeId shapeIdB, b2Manifold* manifold, void* context)
{
	return true;
}

static int button_event_cb(PDButtons button, int down, uint32_t when, void* userdata)
{
	if (down == 0) return 0;
	switch (button)
	{
	case kButtonUp:
	{
		break;
	}
	case kButtonDown:
	{
		break;
	}
	case kButtonLeft:
	{
		break;
	}
	case kButtonRight:
	{
		break;
	}
	default:
		break;
	}
	
	return 0;
}

void generate_meteorite()
{
	int edge_index = rand() % (int)(sizeof(edge) / sizeof(Edge));
	DIRECTION direction = edge[edge_index].direction;
	b2Vec2 from = edge[edge_index].from;
	b2Vec2 to = edge[edge_index].to;
	(void)direction;
	(void)from;
	(void)to;
}
