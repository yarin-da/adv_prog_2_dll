#include "pch.h"
#include <string>
#include <algorithm>
#include "CircleAnomalyDetector.h"
/*
// allocate a timeseries object
extern "C" __declspec(dllexport)
void* Analysis_CreateTimeSeries(
	const char* filename,
	const char* titles[],
	int titlesAmount);

// deallocate a timeseries object
extern "C" __declspec(dllexport)
void Analysis_DestroyTimeSeries(TimeSeries * ts);

// allocate a detector object
extern "C" __declspec(dllexport)
void* Analysis_CreateDetector(TimeSeries * learnTs);

// deallocate a detector object
extern "C" __declspec(dllexport)
void Analysis_DestroyDetector(CircleAnomalyDetector * detector);

// get anomaly timesteps of 'feature'
extern "C" __declspec(dllexport)
void* Analysis_GetTimeSteps(
	CircleAnomalyDetector * detector,
	const TimeSeries * detectTs,
	const char* feature,
	int* size);

// get the timestep at 'index'
extern "C" __declspec(dllexport)
int Analysis_GetTimeStepAt(
	std::vector<int> *timeSteps,
	int index);

// deallocate the timesteps vector
extern "C" __declspec(dllexport)
void Analysis_DestroyTimeSteps(std::vector<int>*timesteps);

// get a vector with points that would define 
// the minimal enclosing circle on the graph
extern "C" __declspec(dllexport)
void* Analysis_GetFuncData(
	CircleAnomalyDetector* detector,
	TimeSeries* detectTs,
	const char* feature,
	int* size);

// get the value at 'index'
extern "C" __declspec(dllexport)
float Analysis_GetFuncDataAt(
	std::vector<float>*funcData,
	int index);

// deallocate the funcData vector
extern "C" __declspec(dllexport)
void Analysis_DestroyFuncData(std::vector<float> *funcData);
*/