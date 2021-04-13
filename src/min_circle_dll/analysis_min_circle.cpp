#include "pch.h"
#include "analysis_min_circle.h"
#include <cmath>

// take a csv file, and array of column titles
// and create a data table using a map<string, vector<float>>
void* Analysis_CreateTimeSeries(
	const char* filename,
	const char* titles[],
	int titlesAmount) {
	// convert the array to a vector (to comply with TimeSeries constructor)
	std::vector<std::string> titlesVec;
	for (int i = 0; i < titlesAmount; i++) {
		titlesVec.push_back(titles[i]);
	}
	// return a new TimeSeries object (as void*)
	return (void*)new TimeSeries(filename, titlesVec);
}

// deallocate the data table
void Analysis_DestroyTimeSeries(TimeSeries * ts) {
	delete ts;
}

// create a detector
void* Analysis_CreateDetector(TimeSeries * learnTs) {
	auto* detector = new CircleAnomalyDetector();
	detector->learnNormal(*learnTs);
	return (void*)detector;
}

// deallocate detector
void Analysis_DestroyDetector(CircleAnomalyDetector * detector) {
	delete detector;
}

// return a timesteps vector
void* Analysis_GetTimeSteps(
	CircleAnomalyDetector * detector,
	const TimeSeries * detectTs,
	const char* feature,
	int* size) {
	// get the anomaly reports
	auto vec = detector->detectFeature(*detectTs, feature);
	*size = vec.size();
	// extract the timesteps into a new vector
	auto* timesteps = new std::vector<int>();
	for (auto& report : vec) {
		timesteps->push_back(report.timeStep);
	}
	return (void*)timesteps;
}

// helper function so that the user won't have to work with vectors directly
int Analysis_GetTimeStepAt(
	std::vector<int> *timeSteps,
	int index) {
	if (index >= timeSteps->size()) {
		return 0;
	}
	// convert the timeStep to an index (i.e. line number)
	return timeSteps->at(index) - 1;
}

// deallocate a timesteps vector
void Analysis_DestroyTimeSteps(std::vector<int>*timesteps) {
	delete timesteps;
}

void* Analysis_GetFuncData(
	CircleAnomalyDetector * detector,
	TimeSeries* detectTs,
	const char* feature,
	int* size) {
	float centerX = 0;
	float centerY = 0;
	float radius = 0;
	std::vector<correlatedFeatures> cf = detector->getNormalModel();
	for (correlatedFeatures c : cf) {
		// find the minimal enclosing circle of 'feature'
		if (c.feature1 == feature || c.feature2 == feature) {
			radius = c.threshold;
			centerX = c.x;
			centerY = c.y;
			break;
		}
	}

	std::cout << feature << " " << centerX << " " << centerY << " " << radius << std::endl;

	const double PI = 3.14159265359;
	const double MAX_ANGLE = 2 * PI;
	// we want to sample POINTS_AMOUNT points
	const double POINTS_AMOUNT = 100;
	const double INC = MAX_ANGLE / POINTS_AMOUNT;
	float angle = 0;
	auto* points = new std::vector<float>();
	// sample a few points around the min circle
	// push every point into 'points' and return it
	for (float angle = 0; angle <= MAX_ANGLE; angle += INC) {
		float x = centerX + std::cos(angle) * radius;
		float y = centerY + std::sin(angle) * radius; 
		points->push_back(x);
		points->push_back(y);
	}
	// insert the point at angle 2PI again to close the shape
	points->push_back(centerX + radius);
	points->push_back(centerY);

	*size = points->size();
	return (void*)points;
}

// let the user work with vectors without knowing what a vector is
float Analysis_GetFuncDataAt(
	std::vector<float>*funcData,
	int index) {
	if (index >= funcData->size()) {
		return 0;
	}
	return funcData->at(index);
}

// deallocate the funcData vector
void Analysis_DestroyFuncData(std::vector<float> *funcData) {
	delete funcData;
}
