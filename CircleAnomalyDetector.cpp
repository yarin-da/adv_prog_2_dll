
#include "pch.h"
#include "CircleAnomalyDetector.h"

bool CircleAnomalyDetector::isAnomaly(
    const correlatedFeatures* c, const Point& point) {
    float x_diff = c->x - point.x;
    float y_diff = c->y - point.y;
    float dist = std::sqrt(x_diff * x_diff + y_diff * y_diff);
    return dist > c->threshold * 1.1f;
}

void CircleAnomalyDetector::fillDataStructure(
    correlatedFeatures& c_features, const Point** points, int size) {
    Circle circle = findMinCircle(points, size);
    c_features.x = circle.center.x;
    c_features.y = circle.center.y;
    c_features.threshold = circle.radius;
}
