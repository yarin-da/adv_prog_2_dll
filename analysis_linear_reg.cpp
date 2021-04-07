#include "pch.h"
#include "analysis_linear_reg.h"
/*
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
void Analysis_DestroyTimeSeries(TimeSeries* ts) {
	delete ts;
}

// create a detector
void* Analysis_CreateDetector(TimeSeries* learnTs) {
	auto* detector = new SimpleAnomalyDetector();
	// call learnNormal immediately on the new detector
	detector->learnNormal(*learnTs);
	return (void*)detector;
}

// deallocate detector
void Analysis_DestroyDetector(SimpleAnomalyDetector* detector) {
	delete detector;
}

// return a timesteps vector
void* Analysis_GetTimeSteps(
	SimpleAnomalyDetector *detector,
	const TimeSeries *detectTs,
	const char *feature,
	int *size) {
	// get the anomaly reports
	auto vec = detector->detectFeature(*detectTs, feature);
	*size = vec.size();
	// extract the timesteps into a new vector
	auto *timesteps = new std::vector<int>();
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
void Analysis_DestroyTimeSteps(std::vector<int>* timesteps) {
	delete timesteps;
}

// let the user work with vectors without knowing what a vector is
float Analysis_GetFuncDataAt(
	std::vector<float>* funcData,
	int index) {
	if (index >= funcData->size()) {
		return 0;
	}
	return funcData->at(index);
}

// helper function for GetFuncData
// get the minimum value of a feature in the data table
static float GetMin(TimeSeries* ts, const char* feature) {
	std::vector<float> vec = ts->getColumn(feature);
	auto it = std::min_element(vec.begin(), vec.end());
	return *it;
}

// helper function for GetFuncData
// get the maximum value of a feature in the data table
static float GetMax(TimeSeries* ts, const char* feature) {
	std::vector<float> vec = ts->getColumn(feature);
	auto it = std::max_element(vec.begin(), vec.end());
	return *it;
}

// return the data (points) required to draw the algorithm
void* Analysis_GetFuncData(
	SimpleAnomalyDetector* detector,
	TimeSeries* detectTs,
	const char* feature,
	int* size) {
	float a = 0;
	float b = 0;
	std::vector<correlatedFeatures> cf = detector->getNormalModel();
	for (correlatedFeatures c : cf) {
		// find the linear regression of 'feature'
		if (c.feature1 == feature || c.feature2 == feature) {
			Line& line = c.lin_reg;
			a = line.a;
			b = line.b;
			break;
		}
	}

	// get min/max x values
	float x1 = GetMin(detectTs, feature);
	float x2 = GetMax(detectTs, feature);
	// make sure x1 and x2 are different
	// because we don't want a zero-length line segment
	if (x1 == x2) { x2 = x1 + 1; }
	// calculate the y-values
	float y1 = x1 * a + b;
	float y2 = x2 * a + b;
	// return a vector with the points that make the linear regression
	auto* vec = new std::vector<float>{ x1, y1, x2, y2 };

	*size = vec->size();
	return (void*)vec;
}

// deallocate the funcData vector
void Analysis_DestroyFuncData(std::vector<float> *funcData) {
	delete funcData;
}
*/