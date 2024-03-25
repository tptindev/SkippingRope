#include "game.h"
#include "gameobj.h"

static const float FPS = 30.0;
static bool initialized = false;
static PlaydateAPI* api = NULL;
static b2WorldId worldId;
static b2BodyId floorId;
static b2BodyId boxId;

b2WorldId register_world(b2Vec2 gravity);
void register_bodies(b2WorldId world_id);

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
	}
}

void game_update(float deltatime)
{
	if (b2World_IsValid(worldId))
	{
		float timeStep = 1.0f / FPS;
		b2World_Step(worldId, timeStep, 1);
	}
}

void game_draw()
{
	api->graphics->clear(kColorWhite);
	api->graphics->setBackgroundColor(kColorBlack);

	{
        b2Vec2 pos = b2Body_GetPosition(boxId);
        api->graphics->fillRect(pos.x, pos.y, 50, 50, kColorBlack);
	}
    api->system->drawFPS(0,0);
}

b2WorldId register_world(b2Vec2 gravity)
{
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = gravity;
	return b2CreateWorld(&worldDef);
}

void register_bodies(b2WorldId world)
{
	api->system->logToConsole("Entry: %s", __FUNCTION__);
	//floorId = create_floor_obj(world);
	boxId = create_square_box_obj(world);
}

