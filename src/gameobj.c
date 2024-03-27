#include "gameobj.h"

b2BodyId create_floor_obj(b2WorldId world)
{
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_staticBody;
	bodyDef.enableSleep = true;
	b2BodyId id = b2CreateBody(world, &bodyDef);

	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 0.0f;
	shapeDef.friction = 1.0;
	b2Segment segment = { {0.0f, 2.7f}, {5.0f,2.7f} };
	b2CreateSegmentShape(id, &shapeDef, &segment);
	return id;
}

b2BodyId create_square_box_obj(b2WorldId world)
{
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.x = 2.5f;
	bodyDef.enableSleep = true;
	bodyDef.isAwake = true;
	b2BodyId id = b2CreateBody(world, &bodyDef);

	b2Polygon box = b2MakeBox(0.5, 0.5);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.friction = 1.0;
	shapeDef.enablePreSolveEvents = true;
	b2CreatePolygonShape(id, &shapeDef, &box);
	return id;
}


