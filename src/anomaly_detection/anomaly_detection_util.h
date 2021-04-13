/* 
 * anomaly_detection_util.cpp
 *
 * Author: 316383298 Yarin Dado
 */

#pragma once

class Line {
public:
	float a;
	float b;

	Line() : a(0), b(0) {}
	Line (float a, float b): a(a), b(b) {}

	float f (float x) const {
		return a * x + b;
	}
};

class Point {
public:
	float x;
    float y;

	Point (float x, float y): x(x), y(y) {}
};

// performs a linear regression and returns the line equation
// doesn't require the caller to make a Point * array
Line linear_reg_arr(const float *x, const float *y, int size);

// performs a linear regression and returns the line equation
Line linear_reg(const Point **points, const int size);

// returns the deviation between point p and the line equation of the points
float dev(const Point p, const Point **points, const int size);

// returns the deviation between point p and the line
float dev(const Point p, const Line l);

// returns the variance of X and Y
float var(const float *x, const int size);

// returns the covariance of X and Y
float cov(const float *x, const float *y, const int size);

// returns the Pearson correlation coefficient of X and Y
float pearson(const float *x, const float *y, const int size);
