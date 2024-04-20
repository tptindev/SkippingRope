#include "game.h"
#include "struct_defines.h"
#include "draw.h"
#include <math.h>
#include <stdlib.h>

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
	unsigned int size;
} Edge;

const float FPS = 30.0;
float world_scale = 80.0f;
const unsigned int MAX_METEORITES = 32;
const double orbit_radius = 0.5f;
const Edge edge[4] =
{
	{TOP, 5},
	{RIGHT, 3},
	{BOTTOM, 5},
	{LEFT, 3}
};

static bool initialized = false;
static PlaydateAPI* api = NULL;
static b2WorldId worldId;
static unsigned int current_level = 0; // index
LevelObject levels[5] =
{
	{"2021", NULL, 500},
	{"2022", NULL, 400},
	{"2023", NULL, 250},
	{"2024", NULL, 200},
	{"2025", NULL, 100},
};


static GameObject* meteorites = NULL;
static GameObject earth_obj = { B2_ZERO_INIT, 2.5f, 1.5f, 0.35f, 0.35f, NULL, 1 }; // id, xcenter, ycenter, hw, hh, bitmap, num of bitmap
static GameObject moon_obj = { B2_ZERO_INIT, 2.5f, 0.0f, 0.15f, 0.15f, NULL, 1 }; // id, xcenter, ycenter, hw, hh, bitmap, num of bitmap

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

		{ // earth
			earth_center = b2Body_GetPosition(earth_obj.id);
		}

		{ // moon
			double angle_rad = api->system->getCrankAngle() * (3.14159265358979323846f / 180.0f);
			moon_obj.x = (float)(orbit_radius * cos(angle_rad) + earth_center.x);
			moon_obj.y = (float)(orbit_radius * sin(angle_rad) + earth_center.y);
			b2Body_SetTransform(
				moon_obj.id,
				(b2Vec2) { moon_obj.x, moon_obj.y},
				api->system->getCrankAngle()
			);
		}

		{ // meteorites
		}
	
	}
}

void game_draw()
{
	api->graphics->clear(kColorWhite);
	api->graphics->setBackgroundColor(kColorBlack);

	{ // earth
		drawRotationFrame(
			api,
			earth_obj.sprites,
			earth_obj.x,
			earth_obj.y,
			true,
			(float)((api->system->getCurrentTimeMilliseconds() / 3) % 360)
		);
	}

	{ // moon
		drawRotationFrame(
			api,
			moon_obj.sprites,
			moon_obj.x,
			moon_obj.y,
			true,
			api->system->getCrankAngle()
		);
	}

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

	{ // earth
		int earth_width = 0; 
		int earth_height = 0;

		const char* outerr = NULL;
		earth_obj.sprites = api->graphics->loadBitmap("images/earth.png", &outerr);
		if (outerr)
		{
			api->system->error("[EARTH] bitmap %s", outerr);
		}

		api->graphics->getBitmapData(earth_obj.sprites, &earth_width, &earth_height, 0, NULL, NULL);

		earth_obj.half_height = (float)(earth_height / 2) / 80.0f;
		earth_obj.half_width = (float)(earth_width / 2) / 80.0f;

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

	{ // moon
		int earth_width = 0;
		int earth_height = 0;

		const char* outerr = NULL;
		moon_obj.sprites = api->graphics->loadBitmap("images/moon.png", &outerr);
		if (outerr)
		{
			api->system->error("[MOON] bitmap %s", outerr);
		}

		api->graphics->getBitmapData(moon_obj.sprites, &earth_width, &earth_height, 0, NULL, NULL);

		moon_obj.half_height = (float)(earth_height / 2) / 80.0f;
		moon_obj.half_width = (float)(earth_width / 2) / 80.0f;

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
		b2Vec2 force = { 0.0f, -10.0f };
		b2Body_ApplyForce(moon_obj.id, force, (b2Vec2){ moon_obj.x, moon_obj.y }, true);
		break;
	}
	case kButtonDown:
	{
		b2Vec2 force = { 0.0f, 10.0f };
		b2Body_ApplyForce(moon_obj.id, force, (b2Vec2) { moon_obj.x, moon_obj.y }, true);
		break;
	}
	case kButtonLeft:
	{
		b2Vec2 force = { -1.0f, 0.0f };
		b2Body_ApplyForce(moon_obj.id, force, (b2Vec2) { moon_obj.x, moon_obj.y }, true);
		break;
	}
	case kButtonRight:
	{
		b2Vec2 force = { 1.0f, 0.0f };
		b2Body_ApplyForce(moon_obj.id, force, (b2Vec2) { moon_obj.x, moon_obj.y }, true);
		break;
	}
	default:
		break;
	}
	
	return 0;
}

void generate_meteorite()
{
}
