/* 
 * SimpleAnomalyDetector.h
 *
 * Author: 316383298 Yarin Dado
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "minCircle.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <string>

struct correlatedFeatures {
	std::string feature1;
	std::string feature2;
	float corrlation;
	Line lin_reg;
    float x;
    float y;
    float threshold;
};

class SimpleAnomalyDetector: public TimeSeriesAnomalyDetector {
	std::vector<correlatedFeatures> cf;
    
	const correlatedFeatures *getCorrelatedFeature(const std::string& title) const;
	float getCorrelation(const TimeSeries& ts, 
		const std::string &first_title, const std::string &second_title);
	void addCorrelatedFeatures(
		const TimeSeries& ts, const std::string &first_feature, 
		const std::string &second_feature, float correlation);
	Point CreateDataPoint(
		const TimeSeries& ts, int index, const std::string& title);
protected:
    float correlation_threshold;
public:
	SimpleAnomalyDetector():correlation_threshold(0.9f) {}
	virtual ~SimpleAnomalyDetector() {}
    float getThreshold() {
        return correlation_threshold;
    }
    void setThreshold(float threshold) {
        correlation_threshold = threshold;
    }
	const std::string getMaxCorrelatedFeature(
		const TimeSeries& ts, const std::string& first_title);
    virtual bool isValidCorrelation(const float correlation);
    virtual bool isAnomaly(const correlatedFeatures *c, const Point& point);
    virtual void fillDataStructure(correlatedFeatures& c_features,
                                   const Point **points, int size);

	virtual void learnNormal(const TimeSeries& ts);
	virtual std::vector<AnomalyReport> detect(const TimeSeries& ts);
	std::vector<AnomalyReport> detectFeature(
		const TimeSeries& ts, const char* feature);
	std::vector<correlatedFeatures> getNormalModel(){
		return cf;
	}
};

#endif
