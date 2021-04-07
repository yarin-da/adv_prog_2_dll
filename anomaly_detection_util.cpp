/* 
 * anomaly_detection_util.cpp
 *
 * Author: 316383298 Yarin Dado
 */

#include "pch.h"
#include <memory>
#include "anomaly_detection_util.h"
#include <cmath>

float avg(const float *x, const int size) {
	float sum = 0;
	for (int i = 0; i < size; i++) {
		sum += x[i];
	}
	return sum / size;
}

float var(const float *x, const int size) {
	// holds sum of x values
 	float sum_normal = 0;
 	// holds sum of x^2 values
 	float sum_squared = 0;
 	// calc sums
 	for (int i = 0; i < size; i++) {
 		sum_normal += x[i];
 		sum_squared += x[i] * x[i];
 	}

 	// average of normal sum
 	float avg = sum_normal / size;

	return sum_squared / size - avg * avg;
}

float cov(const float *x, const float *y, const int size) {
	float avg_x = avg(x, size);
	float avg_y = avg(y, size);

	float sum = 0;
	for (int i = 0; i < size; i++) {
		float diff_x = x[i] - avg_x;
		float diff_y = y[i] - avg_y;
		sum += diff_x * diff_y;
	}

	return sum / size;
}

float pearson(const float *x, const float *y, const int size) {
	// standard deviation for x
	float dev_x = std::sqrt(var(x, size));

	// standard deviation for y
	float dev_y = std::sqrt(var(y, size));

	float covariance = cov(x, y, size);

	if (covariance == 0) {
		return 0;
	}

	return covariance / (dev_x * dev_y);
}

Line linear_reg(const Point **points, const int size) {	
	float *x = new float[size];
	float *y = new float[size];

	// initialize x,y arrays
	for (int i = 0; i < size; i++) {
		const Point *pt = points[i];
		x[i] = pt->x;
		y[i] = pt->y;
	}

	Line ret = linear_reg_arr(x, y, size);

	delete[] x;
	delete[] y;

	return ret;
}

Line linear_reg_arr(const float *x, const float *y, const int size) {
	float slope = cov(x, y, size) / var(x, size);

	float avg_x = avg(x, size);
	float avg_y = avg(y, size);

	float b = avg_y - slope * avg_x;

	return Line(slope, b);
}

float dev(const Point p, const Point **points, const int size) {
	Line line = linear_reg(points, size);
	return dev(p, line);
}

float dev(const Point p, const Line l) {
	float x = p.x;
	float y = p.y;
	float fx = l.f(x);
	return std::abs(fx - y);
}
