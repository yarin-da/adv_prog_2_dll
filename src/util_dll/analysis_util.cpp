#include "pch.h"
#include "analysis_util.h"

// simple function that copies 'str' into 'buffer'
static void copyStringToBuffer(std::string str, char* buffer) {
	std::copy(str.begin(), str.end(), buffer);
	// null-terminate buffer
	buffer[str.size()] = '\0';
}

// initialize timeseries obj with csv data
void* Analysis_CreateTimeSeries(const char* csvFileName, const char* titles[], int numOfTitles) {
	// convert the array to a vector
	std::vector<std::string> titlesVec;
	for (int i = 0; i < numOfTitles; i++) {
		titlesVec.push_back(titles[i]);
	}
	return (void*)new TimeSeries(csvFileName, titlesVec);
}

// deallocate ts
void Analysis_DestroyTimeSeries(TimeSeries* ts) {
	delete ts;
}

// get most correlated column (title is enough)
void Analysis_GetCorrelatedColumn(char* buffer, TimeSeries* ts, const char* title) {
	SimpleAnomalyDetector detector;
	string correlated = detector.getMaxCorrelatedFeature(*ts, title);
	// write 'correlated' into 'buffer' 
	copyStringToBuffer(correlated, buffer);
}

// simple getter of a value in column 'title' at row 'index'
float Analysis_GetValue(TimeSeries* ts, const char* title, int index) {
	return ts->getColumn(title).at(index);
}

// return the minimum value of 'feature'
float Analysis_GetMin(TimeSeries* ts, const char* feature) {
	std::vector<float> vec = ts->getColumn(feature);
	auto it = std::min_element(vec.begin(), vec.end());
	return *it;
}

// return the maximum value of 'feature'
float Analysis_GetMax(TimeSeries* ts, const char* feature) {
	std::vector<float> vec = ts->getColumn(feature);
	auto it = std::max_element(vec.begin(), vec.end());
	return *it;
}
