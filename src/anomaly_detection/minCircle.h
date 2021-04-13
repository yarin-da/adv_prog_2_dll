/* 
 * minCircle.h
 *
 * Author: 316383298 Yarin Dado
 */

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "anomaly_detection_util.h"

using namespace std;

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};

/*
// use pythagorean theorem
float distSquared(const Point *a, const Point *b);
// return TRUE if 'p' is inside 'c' 
float insideCircle(const Point *p, Circle c);
// create a circle so that the points are on the edge of its diameter
Circle circleTwoPoints(const Point *a, const Point *b);
// create a circle using three points
Circle circleThreePoints(const Point *a, const Point *b, const Point *c);
// make the smallest circle that encloses 'points' 
Circle makeCircle(vector<const Point *> points);
Circle minCircle(
    Point **points, size_t size, vector<const Point *> circumference);
*/
// this function will be called from main
Circle findMinCircle(const Point **points, size_t size);

#endif /* MINCIRCLE_H_ */
