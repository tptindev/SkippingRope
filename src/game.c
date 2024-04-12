#include "game.h"
#include "gameobj.h"
#include "draw.h"

float world_scale = 80.0f;

static const float FPS = 30.0;
static bool initialized = false;
static PlaydateAPI* api = NULL;
static b2WorldId worldId;

static GameObject floor_obj = { B2_ZERO_INIT, 1.0f, 2.5f, 1.5f, 0.05f }; // id, x, y, hw, hh
static GameObject box_obj = { B2_ZERO_INIT, 2.5f, 0.0f, 0.1, 0.1 }; // id, x, y, hw, hh


b2WorldId register_world(b2Vec2 gravity);
void register_bodies(b2WorldId world_id);
void unregister_body(b2BodyId bodyId);

static bool PreSolveCb(b2ShapeId shapeIdA, b2ShapeId shapeIdB, b2Manifold* manifold, void* context);
static int ButtonEventCb(PDButtons button, int down, uint32_t when, void* userdata);

void game_initialize(void* userdata)
{
	initialized = true;
	api = userdata;
	api->display->setRefreshRate(FPS);
	api->system->setButtonCallback(ButtonEventCb, NULL, 5);

	b2Vec2 gravity = { 0.0f, 9.81f };
	worldId = register_world(gravity);
	if (b2World_IsValid(worldId))
	{
		register_bodies(worldId);
		b2World_SetPreSolveCallback(worldId, PreSolveCb, NULL);
	}
}

void game_update(float deltatime)
{
	if (b2World_IsValid(worldId))
	{
		float timeStep = 1.0f / FPS;
		b2World_Step(worldId, timeStep, 25);

		b2Vec2 pos = { 0.0f, 0.0f };
		b2Vec2 local_point = { 0.0f, 0.0f };
		b2Vec2 world_point = { 0.0f, 0.0f };
		{
		}

		{
			pos = b2Body_GetPosition(box_obj.id);
			box_obj.x = pos.x;
			box_obj.y = pos.y;
		}
	}
}

void game_draw()
{
	api->graphics->clear(kColorWhite);
	api->graphics->setBackgroundColor(kColorBlack);
	{
		drawRect(api, floor_obj.x, floor_obj.y, floor_obj.half_width * 2, floor_obj.half_height * 2, kColorBlack);
		drawRect(api, box_obj.x - box_obj.half_width, box_obj.y - box_obj.half_height, box_obj.half_width * 2, box_obj.half_height * 2, kColorBlack);
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
	b2BodyDef bodyDef;
	b2ShapeDef shapeDef;
	b2Polygon polygon; 

	{
		bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_staticBody;
		bodyDef.position.x = floor_obj.x + floor_obj.half_width;
		bodyDef.position.y = floor_obj.y + floor_obj.half_height;
		bodyDef.enableSleep = true;
		floor_obj.id = b2CreateBody(world, &bodyDef);

		polygon = b2MakeBox(floor_obj.half_width, floor_obj.half_height);
		shapeDef = b2DefaultShapeDef();
		shapeDef.density = 1.0f;
		shapeDef.friction = 0.3;
		shapeDef.enablePreSolveEvents = true;
		b2CreatePolygonShape(floor_obj.id, &shapeDef, &polygon);
	}

	{
		bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.x = box_obj.x;
		bodyDef.position.y = box_obj.y;
		bodyDef.enableSleep = true;
		box_obj.id = b2CreateBody(world, &bodyDef);

		polygon = b2MakeBox(box_obj.half_width, box_obj.half_height);
		shapeDef = b2DefaultShapeDef();
		shapeDef.density = 1.0f;
		shapeDef.friction = 0.3;
		shapeDef.enablePreSolveEvents = true;
		b2CreatePolygonShape(box_obj.id, &shapeDef, &polygon);
	}
}

void unregister_body(b2BodyId bodyId)
{
}

static bool PreSolveCb(b2ShapeId shapeIdA, b2ShapeId shapeIdB, b2Manifold* manifold, void* context)
{
	return true;
}

static int ButtonEventCb(PDButtons button, int down, uint32_t when, void* userdata)
{
	if (down == 0) return 0;
	switch (button)
	{
	case kButtonUp:
	{
		b2Vec2 force = { 0.0f, -10.0f };
		b2Body_ApplyForce(box_obj.id, force, (b2Vec2){ box_obj.x, box_obj.y }, true);
		break;
	}
	case kButtonDown:
	{
		b2Vec2 force = { 0.0f, 10.0f };
		b2Body_ApplyForce(box_obj.id, force, (b2Vec2) { box_obj.x, box_obj.y }, true);
		break;
	}
	case kButtonLeft:
	{
		b2Vec2 force = { -1.0f, 0.0f };
		b2Body_ApplyForce(box_obj.id, force, (b2Vec2) { box_obj.x, box_obj.y }, true);
		break;
	}
	case kButtonRight:
	{
		b2Vec2 force = { 1.0f, 0.0f };
		b2Body_ApplyForce(box_obj.id, force, (b2Vec2) { box_obj.x, box_obj.y }, true);
		break;
	}
	default:
		break;
	}
	
	return 0;
}