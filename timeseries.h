/* 
 * timeseries.h
 *
 * Author: 316383298 Yarin Dado
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <map>

class TimeSeries {
private:
	std::map<std::string, std::vector<float>> data_table;
	int m_col_count;
	int m_row_count;
	void loadValues(std::ifstream& csvFile, std::vector<std::string>& titles);
	void loadFromFile(std::ifstream& csvFile);
public:
    TimeSeries():m_col_count(0),m_row_count(0) {}
	TimeSeries(const char *CSVfileName);
	TimeSeries(const char* CSVfileName, std::vector<std::string> titles);

	std::vector<std::string> loadTitles(std::string titleLine);
	int rows() const;
	int cols() const;
	
	void tokenize(std::stringstream& stream, 
		std::vector<std::string>& vec);

	const std::vector<float>& getColumn(const std::string& title) const {
		return data_table.at(title);
	}

	const std::map<std::string, std::vector<float>>& getData() const {
		return data_table;
	}
};

#endif
