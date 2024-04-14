#include "game.h"
#include "gameobj.h"
#include "draw.h"
#include <math.h>
#include <stdlib.h>

float world_scale = 80.0f;

static const float FPS = 30.0;
static bool initialized = false;
static PlaydateAPI* api = NULL;
static b2WorldId worldId;
static int current_time = 0;
static int number_of_meteorites = 3;
static int time_step = 0;
static GameObject* meteorite_list = NULL;

static GameObject earth_obj = { B2_ZERO_INIT, 2.5f, 1.5f, 0.35f, 0.35f, NULL, 1 }; // id, xcenter, ycenter, hw, hh
static GameObject moon_obj = { B2_ZERO_INIT, 1.75f, 0.8f, 0.15f, 0.15f, NULL, 1 }; // id, xcenter, ycenter, hw, hh
const double orbit_radius = 1.0f;

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
		b2Vec2 earth_center = { 0.0f, 0.0f };

		{ // earth
			earth_center = b2Body_GetPosition(earth_obj.id);

		}

		{ // moon
			double angle_rad = api->system->getCrankAngle() * (3.14159265358979323846f / 180.0f);
			moon_obj.x = (float)(orbit_radius * cos(angle_rad) + earth_center.x);
			moon_obj.y = (float)(orbit_radius * sin(angle_rad) + earth_center.y);
		}

		{ // meteorites
			current_time = api->system->getCurrentTimeMilliseconds();
			time_step = floor((current_time/1000) % 3);
			if (time_step == 0) {
				meteorite_list = api->system->realloc(meteorite_list, sizeof(GameObject) * number_of_meteorites);
			}

			if (meteorite_list != NULL)
			{
				for (int i = 0; i < number_of_meteorites; i++)
				{

					meteorite_list[i] = (GameObject){ B2_ZERO_INIT, 0.0f, 0.0f, 0.0f, 0.0f };
				}
			}
		}
	}
}

void game_draw()
{
	api->graphics->clear(kColorWhite);
	api->graphics->setBackgroundColor(kColorBlack);

	{ // earth
		drawFrame(api, earth_obj.sprites, earth_obj.x - earth_obj.half_width, earth_obj.y - earth_obj.half_height);
		//drawEllipse(api, earth_obj.x - earth_obj.half_width, earth_obj.y - earth_obj.half_height, earth_obj.half_width * 2, earth_obj.half_height * 2, 0, 0, kColorBlack);
	}

	{ // moon
		drawFrame(api, moon_obj.sprites, moon_obj.x - moon_obj.half_width, moon_obj.y - moon_obj.half_height);
		//drawEllipse(api, moon_obj.x - moon_obj.half_width, moon_obj.y - moon_obj.half_height, moon_obj.half_width * 2, moon_obj.half_height * 2, 0, 0, kColorBlack);
	}

	//{ // meteorites
	//	if (time_step == 3) {
	//		for (int i = 0; i < number_of_meteorites; i++) {
	//			int start_edge = rand() % 4;
	//			b2Vec2 meteorite_pos = { 0.0f, 0.0f };
	//			switch (start_edge)
	//			{
	//			case 0:
	//				meteorite_pos.x = rand() % 6;
	//				break;
	//			case 1:
	//				meteorite_pos.y = rand() % 3;
	//				break;
	//			case 2:
	//				meteorite_pos.x = rand() % 5;
	//				meteorite_pos.y = 3.0f;
	//				break;
	//			case 3:
	//				meteorite_pos.x = 5.0f;
	//				meteorite_pos.y = rand() % 3;
	//				break;
	//			default:
	//				break;
	//			}
	//			drawEllipse(api, meteorite_pos.x, meteorite_pos.y, moon_obj.half_width, moon_obj.half_height, 0, 0, kColorBlack);
	//		}
	//	}
	//	
	//}

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

	{ // earth
		int earth_width = 0; 
		int earth_height = 0;

		const char* outerr = NULL;
		earth_obj.sprites = api->system->realloc(NULL, sizeof(LCDBitmap*));
		earth_obj.sprites = api->graphics->loadBitmap("images/earth.png", &outerr);
		api->graphics->getBitmapData(earth_obj.sprites, &earth_width, &earth_height, 0, NULL, NULL);

		earth_obj.half_height = (float)(earth_height / 2) / 80.0f;
		earth_obj.half_width = (float)(earth_width / 2) / 80.0f;

		bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_staticBody;
		bodyDef.position.x = earth_obj.x;
		bodyDef.position.y = earth_obj.y;
		bodyDef.enableSleep = true;
		earth_obj.id = b2CreateBody(world, &bodyDef);

		polygon = b2MakeRoundedBox(earth_obj.half_width, earth_obj.half_height, earth_obj.half_width); // h-w, h-h, r
		shapeDef = b2DefaultShapeDef();
		shapeDef.density = 1.0f;
		shapeDef.friction = 0.3;
		shapeDef.enablePreSolveEvents = true;
		shapeId = b2CreatePolygonShape(earth_obj.id, &shapeDef, &polygon);

	}

	{ // moon
		int earth_width = 0;
		int earth_height = 0;

		const char* outerr = NULL;
		moon_obj.sprites = api->system->realloc(NULL, sizeof(LCDBitmap*));
		moon_obj.sprites = api->graphics->loadBitmap("images/moon.png", &outerr);
		api->graphics->getBitmapData(moon_obj.sprites, &earth_width, &earth_height, 0, NULL, NULL);

		moon_obj.half_height = (float)(earth_height / 2) / 80.0f;
		moon_obj.half_width = (float)(earth_width / 2) / 80.0f;

		bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_kinematicBody;
		bodyDef.position.x = moon_obj.x;
		bodyDef.position.y = moon_obj.y;
		bodyDef.enableSleep = true;
		moon_obj.id = b2CreateBody(world, &bodyDef);

		polygon = b2MakeRoundedBox(moon_obj.half_width, moon_obj.half_height, moon_obj.half_width); // h-w, h-h, r
		shapeDef = b2DefaultShapeDef();
		shapeDef.density = 1.0f;
		shapeDef.friction = 0.3;
		shapeDef.enablePreSolveEvents = true;
		shapeId = b2CreatePolygonShape(moon_obj.id, &shapeDef, &polygon);
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
	//switch (button)
	//{
	//case kButtonUp:
	//{
	//	b2Vec2 force = { 0.0f, -10.0f };
	//	b2Body_ApplyForce(box_obj.id, force, (b2Vec2){ box_obj.x, box_obj.y }, true);
	//	break;
	//}
	//case kButtonDown:
	//{
	//	b2Vec2 force = { 0.0f, 10.0f };
	//	b2Body_ApplyForce(box_obj.id, force, (b2Vec2) { box_obj.x, box_obj.y }, true);
	//	break;
	//}
	//case kButtonLeft:
	//{
	//	b2Vec2 force = { -1.0f, 0.0f };
	//	b2Body_ApplyForce(box_obj.id, force, (b2Vec2) { box_obj.x, box_obj.y }, true);
	//	break;
	//}
	//case kButtonRight:
	//{
	//	b2Vec2 force = { 1.0f, 0.0f };
	//	b2Body_ApplyForce(box_obj.id, force, (b2Vec2) { box_obj.x, box_obj.y }, true);
	//	break;
	//}
	//default:
	//	break;
	//}
	//
	return 0;
}