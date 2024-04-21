#ifndef __TINY_MATH_H__
#define __TINY_MATH_H__

#include <box2d.h>
#include <math.h>

// [BEGIN] vector
double dot_product(b2Vec2 A, b2Vec2 B)
{
	return (A.x * B.x + A.y + B.y);
}
double magnitude(b2Vec2 vec)
{
	return sqrt((double)(vec.x * vec.x + vec.y + vec.y));
}

double distance(b2Vec2 A, b2Vec2 B)
{
	double x = (double)(B.x - A.x);
	double y = (double)(B.y - A.y);
	return sqrt(x * x + y * y);
}

b2Vec2 normalize(b2Vec2 vec)
{
	float len = (float)magnitude(vec);
	return (b2Vec2) { vec.x / len, vec.y / len };
}
// [END] vector

#endif // __TINY_MATH_H__