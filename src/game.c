#include "game.h"
#include "struct_defines.h"
#include "draw.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "tiny_math.h"

#define EARTH_ON 1
#define MOON_ON 0
#define TESTING_ON 0
#define METEORITES_ON 1

typedef enum e_direction
{
	TOP		= 1,
	RIGHT	= 1 << 1,	// 1 * (2 ^ 1)
	BOTTOM	= 1 << 2,	// 1 * (2 ^ 2)
	LEFT	= 1 << 3	// 1 * (2 ^ 3)
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
static unsigned int current_level = 0; // 0: index
static float last_crank_angle = 0.0f;
static unsigned int user_point = 0;
static bool flag_auto_gen = true;
LevelObject levels[3] =
{
	{"2021", NULL, 500, 3}, // title, tilemap, speed, interval
	{"2022", NULL, 400, 2}, // title, tilemap, speed, interval
	{"2023", NULL, 250, 1}, // title, tilemap, speed, interval
};

static GameObject* meteorites = NULL;
static GameObject earth_obj = { B2_ZERO_INIT, 2.5f, 1.5f, 0.0f, 0.0f, NULL, 1 }; // id, xcenter, ycenter, hw, hh, bitmap, num of bitmap
static GameObject moon_obj = { B2_ZERO_INIT, 2.5f, 0.0f, 0.0f, 0.0f, NULL, 1 }; // id, xcenter, ycenter, hw, hh, bitmap, num of bitmap
static GameObject box_obj = { B2_ZERO_INIT, 2.8f, 0.0f, 0.1f, 0.1f, NULL, 0 };

b2WorldId register_world(b2Vec2 gravity);
void register_bodies(b2WorldId world_id);
void unregister_body(b2BodyId bodyId);

static bool pre_solve_cb(b2ShapeId shapeIdA, b2ShapeId shapeIdB, b2Manifold* manifold, void* context);
static int button_event_cb(PDButtons button, int down, uint32_t when, void* userdata);
static clock_t before;

void game_initialize(void* userdata)
{
	initialized = true;
	api = userdata;
	api->display->setRefreshRate(FPS);
	api->system->setButtonCallback(button_event_cb, NULL, 5);

	before = clock();
	b2Vec2 gravity = { 0.0f, 9.81f };
	worldId = register_world(gravity);
	if (b2World_IsValid(worldId))
	{
		register_bodies(worldId);
		b2World_SetPreSolveCallback(worldId, pre_solve_cb, NULL);
	}

	// register blocks stores 32 GameObjects
	meteorites = api->system->realloc(NULL, sizeof(GameObject) * MAX_METEORITES);
	for (int i = 0; i < MAX_METEORITES; i++)
	{
		meteorites[i].id = b2_nullBodyId;
		meteorites[i].x = 0.0f;
		meteorites[i].y = 0.0f;
		meteorites[i].half_width = 0.0f;
		meteorites[i].half_height = 0.0f;
		meteorites[i].sprites = NULL;
		meteorites[i].sprite_size = 0;
	}
}

void game_update(float deltatime)
{
	clock_t diff = clock() - before;
	int sec = diff / CLOCKS_PER_SEC;
	if (b2World_IsValid(worldId))
	{
		float timeStep = 1.0f / FPS;
		b2World_Step(worldId, timeStep, 10);
		LevelObject level = levels[current_level];
		b2Vec2 pos = { 0.0f, 0.0f };
		b2Vec2 local_point = { 0.0f, 0.0f };
		b2Vec2 world_point = { 0.0f, 0.0f };
		b2Vec2 earth_center = { 0.0f, 0.0f };

#if EARTH_ON
		{ // earth
			earth_center = b2Body_GetPosition(earth_obj.id);
			b2Body_SetTransform(
				earth_obj.id,
				(b2Vec2) { earth_obj.x, earth_obj.y },
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


#if METEORITES_ON
		{ // meteorites: Entry every 'levels[current_level].interval' miliseconds 
			if (sec == 0 && flag_auto_gen)
			{
				flag_auto_gen = false;
				int edge_index = rand() % (int)(sizeof(edge) / sizeof(Edge));
				DIRECTION direction = edge[edge_index].direction;
				b2Vec2 from = edge[edge_index].from;
				b2Vec2 to = edge[edge_index].to;
				b2Vec2 vec = normalize((b2Vec2) { to.x - from.x, to.y - from.y });
				float val = (float)rand() / (float)(RAND_MAX / distance(from, to));
				vec.x = (int)vec.x * val;
				vec.y = (int)vec.y * val;
				api->system->logToConsole("Direction %d x: %f y: %f", (int)direction, vec.x, vec.y);

				// create meteorite
				{
					meteorites[0].half_width = 0.15f;
					meteorites[0].half_height = 0.15f;
					meteorites[0].x = vec.x;
					meteorites[0].y = vec.y;

					b2BodyDef bodyDef = b2DefaultBodyDef();
					bodyDef.type = b2_staticBody;
					bodyDef.position.x = meteorites[0].x;
					bodyDef.position.y = meteorites[0].y;
					bodyDef.enableSleep = true;
					meteorites[0].id = b2CreateBody(worldId, &bodyDef);

					b2Circle circle = { (b2Vec2) { 0.0f, 0.0f }, meteorites[0].half_width };

					b2ShapeDef shapeDef = b2DefaultShapeDef();
					shapeDef.density = 1.0f;
					shapeDef.friction = 0.3f;
					b2CreateCircleShape(meteorites[0].id, &shapeDef, &circle);
				}
			}

			{ // update meteorite logic
				for (int i = 0; i < MAX_METEORITES; i++)
				{
					if (b2Body_IsValid(meteorites[i].id))
					{
						b2Vec2 obj_pos = b2Body_GetPosition(meteorites[i].id);
						meteorites[i].x += (earth_center.x - meteorites[i].x) * levels[current_level].speed * deltatime;
						meteorites[i].y += (earth_center.y - meteorites[i].y) * levels[current_level].speed * deltatime;

						b2Body_SetTransform(
							meteorites[i].id,
							(b2Vec2) {
								meteorites[i].x, meteorites[i].y
							},
							0
						);
					}
				}
			}

		}
#endif // METEORITES_ON

		if (api->system->getCrankAngle() != last_crank_angle)
		{

#if MOON_ON
			api->system->logToConsole("Moon position: %f %f", moon_obj.x, moon_obj.y);
#endif

#if TESTING_ON
			api->system->logToConsole("Box position: %f %f", box_obj.x, box_obj.y);
#endif

		}

		last_crank_angle = api->system->getCrankAngle();
		if (sec > levels[current_level].interval)
		{
			before = clock();
			flag_auto_gen = true;
		}
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


#if METEORITES_ON
	{ // meteorites
		for (int i = 0; i < MAX_METEORITES; i++)
		{
			if (b2Body_IsValid(meteorites[i].id))
			{
				b2Transform meteorite_trans = b2Body_GetTransform(meteorites[i].id);
				drawEllipse(api, meteorite_trans.p.x, meteorite_trans.p.y, meteorites[i].half_width * 2.0f, meteorites[i].half_height * 2.0f, 0, 0, kColorBlack);
			}
		}
	}
#endif // METEORITES_ON
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
