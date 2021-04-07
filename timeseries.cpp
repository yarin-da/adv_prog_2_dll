/* 
 * timeseries.cpp
 *
 * Author: 316383298 Yarin Dado
 */

#include "pch.h"
#include "timeseries.h"
#include <iostream>

const char delimiter = ',';

TimeSeries::TimeSeries(const char* CSVfileName, std::vector<std::string> titles) {
	m_col_count = m_row_count = 0;

	std::ifstream csvFile;
	csvFile.open(CSVfileName);

	if (!csvFile.fail()) {
		m_col_count = titles.size();
		TimeSeries::loadValues(csvFile, titles);
	}
	else {
		std::cerr << "Failed to load file " << CSVfileName << std::endl;
	}

	csvFile.close();
}

TimeSeries::TimeSeries(const char *CSVfileName) {
	m_col_count = m_row_count = 0;

	std::ifstream csvFile;
	csvFile.open(CSVfileName);
	
	if (!csvFile.fail()) {
		TimeSeries::loadFromFile(csvFile);	
	} else {
		std::cerr << "Failed to load file " << CSVfileName << std::endl;
	}
	
	csvFile.close();
}

std::vector<std::string> TimeSeries::loadTitles(std::string titleLine) {
	std::vector<std::string> titles;
	// add column headers to a vector
	std::stringstream ss(titleLine);
	tokenize(ss, titles);
	return titles;
}

void TimeSeries::loadValues(std::ifstream& csvFile, std::vector<std::string>& titles) {
	// initialize the column vector
	for (auto i = 0; i < m_col_count; i++) {
		std::string& columnTitle = titles.at(i);
		data_table.insert({ columnTitle, std::vector<float>() });
	}

	// run through the table row by row 
	std::string row;
	while (std::getline(csvFile, row)) {
		// parse row data
		++m_row_count;
		std::stringstream ss(row);
		std::vector<std::string> values;
		tokenize(ss, values);

		// run through each column and add a new row
		for (auto i = 0; i < m_col_count; i++) {
			std::string& title = titles.at(i);
			const auto& str = values.at(i);
			const auto num = std::stof(str, nullptr);

			data_table.at(title).push_back(num);
		}
	}
}

void TimeSeries::loadFromFile(std::ifstream& csvFile) {
	// get the column headers/titles
	std::string titleLine;
	std::vector<std::string> titles;
	if (std::getline(csvFile, titleLine)) {
		titles = loadTitles(titleLine);
	} else {
		std::cerr << "Input file is empty" << std::endl;
		return;
	}

	// number of columns = number of headers
	m_col_count = titles.size();

	// initialize the map
	loadValues(csvFile, titles);
}

void TimeSeries::tokenize(std::stringstream& stream, 
	std::vector<std::string>& vec) {

	std::string line;
	while (getline(stream, line, delimiter)) {
		vec.push_back(line);
	}
}

int TimeSeries::rows() const {
	return m_row_count;
}
int TimeSeries::cols() const {
	return m_col_count;
}