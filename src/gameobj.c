#include "gameobj.h"
#include <float.h>

b2BodyId create_floor_obj(b2WorldId world)
{
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_staticBody;
	bodyDef.position.x = 1.0f;
	bodyDef.position.y = 2.75f;
	bodyDef.enableSleep = true;
	b2BodyId id = b2CreateBody(world, &bodyDef);

	b2Polygon box = b2MakeBox(1.5f, 0.1f);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1.0f;
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
	bodyDef.position.y = 0.0f;
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

void get_shape_size(b2ShapeId shapeId, float* width, float* height)
{
	float min_width = FLT_MAX;
	float max_width = -FLT_MAX;
	float min_height = FLT_MAX;
	float max_height = -FLT_MAX;

	b2Polygon polygon = b2Shape_GetPolygon(shapeId);
	b2BodyId body = b2Shape_GetBody(shapeId);

	for (int index = 0; index < polygon.count; index++)
	{
		b2Vec2 vertex = b2Body_GetWorldPoint(body, polygon.vertices[index]);
		float x = vertex.x;
		float y = vertex.y;

		if (x < min_width) min_width = x;
		if (x > max_height) max_width = x;
		if (y < min_height) min_height = y;
		if (y > max_height) max_height = y;
	}
	(*width) = (max_width - min_width);
	(*height) = (max_height - min_height);
}


