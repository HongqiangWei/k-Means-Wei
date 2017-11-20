
#ifndef Point_H_DEF
#define  Point_H_DEF
#include "stdafx.h"
#include <math.h>
#include <vector>


struct Point
{
	float x;
	float y;
	float z;
	size_t label;
};

struct PointRgb
{
	float x;
	float y;
	float z;
	size_t r;
	size_t g;
	size_t b;
};



float distance(const Point point1, const Point point2) {
	return sqrt((point1.x - point2.x)*(point1.x - point2.x) + 
				(point1.y - point2.y)*(point1.y - point2.y) + 
				(point1.z - point2.z)*(point1.z - point2.z));
}

#endif  //Point_H_DEF
