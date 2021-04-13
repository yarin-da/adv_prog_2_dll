#ifndef CIRCLEANOMALYDETECTOR_H_
#define CIRCLEANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class CircleAnomalyDetector : public SimpleAnomalyDetector {
public:
	CircleAnomalyDetector() {}
	virtual ~CircleAnomalyDetector() {}
	bool isAnomaly(const correlatedFeatures* c, const Point& point);
	void fillDataStructure(correlatedFeatures& c_features,
		const Point** points, int size);

};

#endif