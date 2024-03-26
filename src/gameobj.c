#include "gameobj.h"

b2BodyId create_floor_obj(b2WorldId world)
{
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_staticBody;
	bodyDef.enableSleep = true;
	b2BodyId id = b2CreateBody(world, &bodyDef);

	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 0.0f;
	shapeDef.friction = 0.3;
	b2Segment segment = { {-400.0f, 230.0f}, {400.0f, 230.0f} };
	b2CreateSegmentShape(id, &shapeDef, &segment);
	return id;
}

b2BodyId create_square_box_obj(b2WorldId world)
{
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.x = 200.0f;
	bodyDef.enableSleep = true;
	b2BodyId id = b2CreateBody(world, &bodyDef);

	b2Polygon box = b2MakeBox(25.0f, 50.0f);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.friction = 0.3;

	b2CreatePolygonShape(id, &shapeDef, &box);
	b2Body_SetGravityScale(id, 80);
	return id;
}


