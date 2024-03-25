#include "gameobj.h"

b2BodyId create_floor_obj(b2WorldId world)
{
	b2BodyDef body = b2DefaultBodyDef();
	return b2CreateBody(world, &body);
}

b2BodyId create_square_box_obj(b2WorldId world)
{
	b2Polygon box = b2MakeBox(25.0f, 25.0f);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.friction = 0.2;

	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.x = 200.0f;
	b2BodyId id = b2CreateBody(world, &bodyDef);
	b2CreatePolygonShape(id, &shapeDef, &box);
	return id;
}


