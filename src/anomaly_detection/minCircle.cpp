#include "pch.h"
#include "minCircle.h"

float dist(Point a, Point b) {
	float x2 = (a.x - b.x) * (a.x - b.x);
	float y2 = (a.y - b.y) * (a.y - b.y);
	return sqrt(x2 + y2);
}

Circle from2points(Point a, Point b) {
	float x = (a.x + b.x) / 2;
	float y = (a.y + b.y) / 2;
	float r = dist(a, b) / 2;
	return Circle(Point(x, y), r);
}



Circle from3Points(Point a, Point b, Point c) {
	// find the circumcenter of the triangle a,b,c
	Point mAB((a.x + b.x) / 2, (a.y + b.y) / 2); // mid point of line AB
	float pSlopAB = (a.x - b.x) / (b.y - a.y); // the perpendicular slop of AB
	Point mBC((b.x + c.x) / 2, (b.y + c.y) / 2); // mid point of line BC
	float pSlopBC = (b.x - c.x) / (c.y - b.y);
	
	// if slopes are equal (or both are NaN, i.e. vertical)
	if ((pSlopAB == pSlopBC) || (pSlopAB != pSlopAB && pSlopBC != pSlopBC)) {
		float minX = min(a.x, min(b.x, c.x));
		float maxX = max(a.x, max(b.x, c.x));
		float x = (minX + maxX) / 2;
		float minY = min(a.y, min(b.y, c.y));
		float maxY = max(a.y, max(b.y, c.y));
		float y = (minY + maxY) / 2;
		Point center(x, y);
		float radius = dist(a, center);
		return Circle(center, radius);
	}
	// if only AB is vertical
	else if (a.y == b.y) {
		float x = mAB.x;
		float y = pSlopBC * (x - mBC.x) + mBC.y;
		Point center(x, y);
		float radius = dist(a, center);
		return Circle(center, radius);
	}
	// if only BC is vertical
	else if (b.y == c.y) {
		float x = mBC.x;
		float y = pSlopAB * (x - mAB.x) + mAB.y;
		Point center(x, y);
		float radius = dist(a, center);
		return Circle(center, radius);
	}

	// otherwise, the center can be calculated normally
	float x = (-pSlopBC * mBC.x + mBC.y + pSlopAB * mAB.x - mAB.y) / (pSlopAB - pSlopBC);
	float y = pSlopAB * (x - mAB.x) + mAB.y;
	Point center(x, y);
	float R = dist(center, a);

	return Circle(center, R);
}

Circle trivial(vector<Point>& P) {
	if (P.size() == 0)
		return Circle(Point(0, 0), 0);
	else if (P.size() == 1)
		return Circle(P[0], 0);
	else if (P.size() == 2)
		return from2points(P[0], P[1]);

	// maybe 2 of the points define a small circle that contains the 3ed point
	Circle c = from2points(P[0], P[1]);
	if (dist(P[2], c.center) <= c.radius)
		return c;
	c = from2points(P[0], P[2]);
	if (dist(P[1], c.center) <= c.radius)
		return c;
	c = from2points(P[1], P[2]);
	if (dist(P[0], c.center) <= c.radius)
		return c;
	// else find the unique circle from 3 points
	return from3Points(P[0], P[1], P[2]);
}


/*
algorithm welzl
	input: Finite sets P and R of points in the plane |R|<= 3.
	output: Minimal disk enclosing P with R on the boundary.

	if P is empty or |R| = 3 then
		return trivial(R)
	choose p in P (randomly and uniformly)
	D := welzl(P - { p }, R)
	if p is in D then
		return D

	return welzl(P - { p }, R U { p })
 */


Circle welzl(const Point** P, vector<Point> R, size_t n) {
	if (n == 0 || R.size() == 3) {
		return trivial(R);
	}

	// remove random point p
	// swap is more efficient than remove
	//srand (time(NULL));
	int i = rand() % n;
	Point p(P[i]->x, P[i]->y);
	swap(P[i], P[n - 1]);

	Circle c = welzl(P, R, n - 1);

	if (dist(p, c.center) <= c.radius)
		return c;

	R.push_back(p);

	return welzl(P, R, n - 1);
}

Circle findMinCircle(const Point** points, size_t size) {
	return welzl(points, {}, size);
}
