#include "game.h"
#include "gameobj.h"
#include "draw.h"

static const float FPS = 30.0;
static bool initialized = false;
static PlaydateAPI* api = NULL;
static b2WorldId worldId;
static b2BodyId floorId;
static b2BodyId columns[2];
static b2BodyId boxId;

float world_scale = 80.0f;

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
	}
}

void game_draw()
{
	api->graphics->clear(kColorWhite);
	api->graphics->setBackgroundColor(kColorBlack);

	b2Vec2 pos = {0.0f, 0.0f};
	b2ShapeId shapeId = b2_nullShapeId;
	b2Segment segment;
	{
		pos = b2Body_GetPosition(floorId);
		shapeId = b2Body_GetFirstShape(floorId);
		drawRect(api, pos, 30.0f, 0.1f, kColorBlack);

		for (size_t i = 0; i < sizeof(columns) / sizeof(b2BodyId); i++)
		{
			pos = b2Body_GetPosition(columns[i]);
			drawRect(api, pos, 0.04f, 2.9f, kColorBlack);
		}

        pos = b2Body_GetPosition(boxId);
		shapeId = b2Body_GetFirstShape(boxId);
		drawRect(api, pos, 0.2f, 0.2f, kColorBlack);
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
	floorId = create_floor_obj(world);
	columns[0] = create_column_obj(world, 1.0f, 1.0f, 0.05, 2.0f);
	columns[1] = create_column_obj(world, 4.0f, 1.0f, 0.05, 2.0f);
	boxId = create_square_box_obj(world);
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
		b2Vec2 pos = b2Body_GetPosition(boxId);
		b2Body_ApplyForce(boxId, force, pos, true);
		break;
	}
	case kButtonDown:
	{
		b2Vec2 force = { 0.0f, 10.0f };
		b2Vec2 pos = b2Body_GetPosition(boxId);
		b2Body_ApplyForce(boxId, force, pos, true);
		break;
	}
	case kButtonLeft:
	{
		b2Vec2 force = { -1.0f, 0.0f };
		b2Vec2 pos = b2Body_GetPosition(boxId);
		b2Body_ApplyForce(boxId, force, pos, true);
		break;
	}
	case kButtonRight:
	{
		b2Vec2 force = { 1.0f, 0.0f };
		b2Vec2 pos = b2Body_GetPosition(boxId);
		b2Body_ApplyForce(boxId, force, pos, true);
		break;
	}
	default:
		break;
	}
	return 0;
}