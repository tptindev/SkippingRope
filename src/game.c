#include "game.h"
#include "gameobj.h"
#include "draw.h"
#include "callbacks.h"

static const float FPS = 30.0;
static bool initialized = false;
static PlaydateAPI* api = NULL;
static b2WorldId worldId;
static b2BodyId floorId;
static b2BodyId boxId;

float world_scale = 80.0f;

b2WorldId register_world(b2Vec2 gravity);
void register_bodies(b2WorldId world_id);
void unregister_body(b2BodyId bodyId);

void game_initialize(void* userdata)
{
	initialized = true;
	api = userdata;
	api->display->setRefreshRate(FPS);

	b2Vec2 gravity = { 0.0f, 9.81f };
	worldId = register_world(gravity);
	if (b2World_IsValid(worldId))
	{
		register_bodies(worldId);
		b2World_SetPreSolveCallback(worldId, PreSolveFcn, NULL);
	}
}

void game_update(float deltatime)
{
	if (b2World_IsValid(worldId))
	{
		float timeStep = 1.0f / FPS;
		b2World_Step(worldId, timeStep, 10);
	}
}

void game_draw()
{
	api->graphics->clear(kColorWhite);
	api->graphics->setBackgroundColor(kColorBlack);

	b2Vec2 pos = {0.0f, 0.0f};
	b2ShapeId shapeId = b2_nullShapeId;
	{
		pos = b2Body_GetPosition(floorId);
		shapeId = b2Body_GetFirstShape(floorId);
		b2Segment segment = b2Shape_GetSegment(shapeId);
		drawLine(api, segment, 0.1, kColorBlack);

        pos = b2Body_GetPosition(boxId);
		drawRect(api, pos, 0.5, 0.5, kColorBlack);
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
	boxId = create_square_box_obj(world);
}

void unregister_body(b2BodyId bodyId)
{
}

