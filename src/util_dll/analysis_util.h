#pragma once

#include "pch.h"
#include <vector>
#include <string>
#include <iostream>
#include "SimpleAnomalyDetector.h"

// initialize timeseries obj with csv data
extern "C" __declspec(dllexport)
void* Analysis_CreateTimeSeries(
	const char* csvFileName,
	const char* titles[],
	int numOfTitles);

extern "C" __declspec(dllexport)
void Analysis_DestroyTimeSeries(TimeSeries * ts);

// get most correlated column (title is enough)
extern "C" __declspec(dllexport)
void Analysis_GetCorrelatedColumn(
	char* buffer,
	TimeSeries * ts,
	const char* title);

// simple getter of a value in column 'title' at row 'index'
extern "C" __declspec(dllexport)
float Analysis_GetValue(TimeSeries * ts,
	const char* title,
	int index);

// get the minimum value of 'feature'
extern "C" __declspec(dllexport)
float Analysis_GetMin(TimeSeries * ts, const char* feature);

// get the maximum value of 'feature'
extern "C" __declspec(dllexport)
float Analysis_GetMax(TimeSeries * ts, const char* feature);
