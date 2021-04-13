# DLL source code example

All the code is used by [Flight Inspection Client](https://github.com/yarin-da/adv_prog_2)

## Customizable API for your needs:

### These DLL's can be loaded dynamically at runtime via user request

[Linear Regression API interface](src/linear_reg_dll)

[Minimum Enclosing Circle API interface](src/min_circle_dll)

### Utility DLL is loaded at the startup of the program

[General Utility API interface](src/util_dll)

### All the code is based on this library

[Anomaly Detection Library](src/anomaly_detection)

# API Documentation

### **Note: Your code should compile to x86 platform.**

## API specific terms:

* TimeSeries: an object that holds the data table.
  The API will use this object to receive values about a certain feature.
* Detector: an object that is able to detect anomalies. The API will feed it a data table with regular values.

***

## Functions

* Each function starts with the prefix `Analysis_`.
* For every function that allocates memory (e.g. `Analysis_CreateTimeSeries`), there's a function that deallocates this memory to prevent memory leaks (e.g. `Analysis_DestroyTimeSeries`).

***

## Basic generic constructors and destructors
```cpp
// return a new object that holds all your data (e.g. a data table)
void *Analysis_CreateTimeSeries(
  // the file that holds all your data
  const char *fileName, 
  // an array of titles for the columns in your data table
  const char *titles[], 
  // the size of the array
  int numOfTitles
);

// deallocate 'timeSeriesObject'
void Analysis_DestroyTimeSeries(
  void *timeSeriesObject
);

// return a new Detector object that was initialized with the learning data
// in 'learnTimeSeriesObject
void *Analysis_CreateDetector(
  void *learnTimeSeriesObject
);

// deallocate 'detectorObject'
void Analysis_DestroyDetector(
  void *detectorObject
);
```

***

## Wrapper functions for data structures

You'd probably want to send the program a data structure with anomalies.

This can be done easily and with any data structure using these wrapper functions:

```cpp
// return a data structure that contains the timesteps at which an anomaly occurred
void *Analysis_GetTimeSteps(
  // a detector that could detect and retrieve all of the anomalies
	void *detector,
  // the data to look for anomalies
	const void *detectTs,
  // we are looking specifically for anomalies that are relevant to 'feature'
	const char *feature,
  // the size of the data structure (i.e. the amount of anomalies we returned)
	int *size
);

// simple wrapper function to get the timestep at 'index'
int Analysis_GetTimeStepAt(
  // the timesteps data structure
  void *timeSteps,
  // the index of the timestep
  int index
);

// deallocate 'timesteps'
void Analysis_DestroyTimeSteps(
  void *timesteps
);
```

***

## Generic drawing of the algorithm

You may want to simulate your anomaly detection algorithm better.

These next functions will help you do that.

```cpp
// return a data structure that contains the points you want to draw
// the data structure must be linear (accessed by indices)
// x-values are immidiately followed by y-values
// for example:
//    let's say I want to draw a line segment
//    a line segment is defined by its two edge points
//    if the line is defined by (x1, y1) and (x2, y2)
//    we'll return a data structure that contains (x1, y1, x2, y2)
void* Analysis_GetFuncData(
  // a detector that can has the values required for the drawing
	void *detector,
  // a timeseries object that contains the data 
  // can be used for determining min/max values
	void *detectTs,
  // we want to draw according to 'feature's data
	const char *feature,
  // the size of the data structure the function returns
	int *size
);

// simple wrapper for the data structure to get the value at 'index'
float Analysis_GetFuncDataAt(
  // a data structure that contains all of the points
	void* funcData,
  // the index of the value we want
	int index
);

// deallocate 'funcData'
void Analysis_DestroyFuncData(
  void *funcData
);
```