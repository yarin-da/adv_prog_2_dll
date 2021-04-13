/* 
 * SimpleAnomalyDetector.cpp
 *
 * Author: 316383298 Yarin Dado
 */

#include "pch.h"
#include "SimpleAnomalyDetector.h"
#include <limits>

void SimpleAnomalyDetector::fillDataStructure(
    correlatedFeatures& c_features, const Point **points, int size) {

	const Line line = linear_reg(points, size);
    
	// calculate the threshold field in struct correlatedFeatures
	float max_offset = 0;
	for (auto k = 0; k < size; k++) {		
		// get the highest deviation and store it in max_offset
		const float deviation = abs(dev(*points[k], line));
		if (max_offset < deviation) {
			max_offset = deviation;
		}
	}

    c_features.lin_reg = line;
    c_features.threshold = max_offset;
}

void SimpleAnomalyDetector::addCorrelatedFeatures(
	const TimeSeries& ts, const std::string &first_feature, 
	const std::string &second_feature, float correlation) {
	
    correlatedFeatures c_features;
    c_features.feature1 = first_feature;
    c_features.feature2 = second_feature;    
    c_features.corrlation = correlation;

    int rows = ts.rows();
	const auto &first_vec = ts.getData().at(first_feature);
	const auto &second_vec = ts.getData().at(second_feature);
    const Point **points = new const Point*[rows];
    for (int i = 0; i < rows; i++) {
        points[i] = new const Point(first_vec[i], second_vec[i]);
    }

    fillDataStructure(c_features, (const Point**)points, rows);

    for (int i = 0; i < rows; i++) {
        delete points[i];
    }
    delete[] points;
        
	// finally add a new correlatedFeatures object into cf
    cf.push_back(c_features);
    //cf.push_back({first_feature,second_feature,correlation,line.a,line.b,max_offset});
}

float SimpleAnomalyDetector::getCorrelation(
	const TimeSeries& ts, const std::string &first_title, 
	const std::string &second_title) {

	const auto &first_vec = ts.getData().at(first_title);
	const auto &second_vec = ts.getData().at(second_title);
	const int rows = ts.rows();

	return pearson(first_vec.data(), second_vec.data(), rows);
}

const std::string SimpleAnomalyDetector::getMaxCorrelatedFeature(
	const TimeSeries& ts, const std::string &first_title) {
	
	std::string max_feature;
	float max_correlation = -1;

	// find the feature with the maximum correlation
	for (const auto &column : ts.getData()) {
		const auto &second_title = column.first;
		// make sure not to test the column with itself
		if (second_title.compare(first_title) != 0) {
			const float correlation = getCorrelation(ts, first_title, second_title);
			const float absCorr = abs(correlation);
			// update the maximum correlation
			if(max_correlation < absCorr) {
				max_correlation = absCorr;
				max_feature = second_title;
			}
		}
	}
	return max_feature;
}

bool SimpleAnomalyDetector::isValidCorrelation(const float correlation) {
    return abs(correlation) > correlation_threshold;
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
	// run through all features
	for (const auto &column : ts.getData()) {
		const auto &title = column.first;
		if (getCorrelatedFeature(title) == nullptr) {
			// get the feature with the highest correlation
			const auto &max_feature = getMaxCorrelatedFeature(ts, title);
			// make sure the correlation is valid
			const float correlation = getCorrelation(ts, title, max_feature);
			if (isValidCorrelation(correlation)) {
				addCorrelatedFeatures(ts, title, max_feature, correlation);
			}
		}
	}
}

bool SimpleAnomalyDetector::isAnomaly(const correlatedFeatures *c, const Point& point) {
    const float deviation = dev(point, c->lin_reg);
    return deviation > 1.15f * c->threshold;
}

Point SimpleAnomalyDetector::CreateDataPoint(
	const TimeSeries& ts, int index, const std::string& title) {
	return Point(0, 0);
}

std::vector<AnomalyReport> SimpleAnomalyDetector::detectFeature(
	const TimeSeries& ts, const char* feature) {
	
	std::vector<AnomalyReport> anomalies;
	const int row_count = ts.rows();

	const auto& title = feature;
	const correlatedFeatures* c = getCorrelatedFeature(title);
	for (auto i = 0; i < row_count; i++) {
		// make sure our current column has a most correlated feature
		// and make sure it's the first feature in the struct
		if (c == nullptr || c->feature1.compare(title) != 0) {
			continue;
		}
		// create a point with the values in row i
		const float a = ts.getData().at(c->feature1).at(i);
		const float b = ts.getData().at(c->feature2).at(i);
		const Point point(a, b);

		// test if it the deviation is big enough for an anomaly
		if (isAnomaly(c, point)) {
			const auto& description = c->feature1 + "-" + c->feature2;
			// we start the line count from 1
			const long time_step = i + 1;
			// create a report about the anomaly, and add it
			const AnomalyReport report(std::move(description), time_step);
			anomalies.push_back(std::move(report));
		}
	}
	return anomalies;
}

std::vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
	std::vector<AnomalyReport> anomalies;
	const int row_count = ts.rows();

	for (auto i = 0; i < row_count; i++) {
		for (const auto &column : ts.getData()) {
			const auto &title = column.first;
			const correlatedFeatures *c = getCorrelatedFeature(title);
			// make sure our current column has a most correlated feature
			// and make sure it's the first feature in the struct
			if (c == nullptr || c->feature1.compare(title) != 0) {
				continue;
			}
			// create a point with the values in row i
			const float a = ts.getData().at(c->feature1).at(i);
			const float b = ts.getData().at(c->feature2).at(i);
			const Point point(a, b);

			// test if it the deviation is big enough for an anomaly
			if (isAnomaly(c, point)) {
				const auto &description = c->feature1 + "-" + c->feature2;
				// we start the line count from 1
				const long time_step = i + 1;
				// create a report about the anomaly, and add it
				const AnomalyReport report(std::move(description), time_step);
				anomalies.push_back(std::move(report));
			}
		}
	}

	return anomalies;
}

const correlatedFeatures *SimpleAnomalyDetector::getCorrelatedFeature(
	const std::string& title) const {

	const auto size = cf.size();
	for (auto i = 0u; i < size; i++) {
		// get &cf[i]
		const correlatedFeatures *f = cf.data() + i;
		// check if the feature is inside this correlatedFeatures structure
		if (f->feature1.compare(title) == 0 || f->feature2.compare(title) == 0) {
			return f;
		}
	}
	// the feature doesn't have a correlatedFeatures structure
	return nullptr;
}
