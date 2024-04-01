#include "gameobj.h"

b2BodyId create_floor_obj(b2WorldId world)
{
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_staticBody;
	bodyDef.position.x = -10.0f;
	bodyDef.position.y = 2.75f;
	bodyDef.enableSleep = true;
	b2BodyId id = b2CreateBody(world, &bodyDef);

	b2Polygon box = b2MakeBox(15.0f, 0.05f);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 0.0f;
	shapeDef.friction = 0.3;
	shapeDef.enablePreSolveEvents = true;
	b2CreatePolygonShape(id, &shapeDef, &box);
	return id;
}

b2BodyId create_square_box_obj(b2WorldId world)
{
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.x = 2.25f;
	bodyDef.position.y = 1.0f;
	bodyDef.enableSleep = true;
	bodyDef.isAwake = true;
	b2BodyId id = b2CreateBody(world, &bodyDef);

	b2Polygon box = b2MakeBox(0.1, 0.1);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.friction = 0.3;
	shapeDef.enablePreSolveEvents = true;
	b2CreatePolygonShape(id, &shapeDef, &box);
	return id;
}

b2BodyId create_column_obj(b2WorldId world, float x, float y, float haft_width, float haft_height)
{
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_staticBody;
	bodyDef.position.x = x;
	bodyDef.position.y = y;
	bodyDef.enableSleep = true;
	b2BodyId id = b2CreateBody(world, &bodyDef);

	b2Polygon box = b2MakeBox(haft_width, haft_height);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 0.0f;
	shapeDef.friction = 0.3;
	shapeDef.enablePreSolveEvents = true;
	b2CreatePolygonShape(id, &shapeDef, &box);
	return id;
}

b2BodyId* create_rope_obj(b2WorldId world)
{
	return NULL;
}


