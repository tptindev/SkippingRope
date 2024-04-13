#include "gameobj.h"
#include <float.h>

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


