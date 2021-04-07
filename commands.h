/* 
 * commands.h
 *
 * Author: 316383298 Yarin Dado
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}
};

class SocketIO:public DefaultIO{
private:
    static const char DELIMITER = '\n';
    static const size_t BUF_SIZE = 1024;
    int socket;
    size_t recv_size = 0;
    char recv_buf[BUF_SIZE];
    char send_buf[BUF_SIZE];
public:
    SocketIO(int socket):socket(socket) {}
    // helper function for read to figure out when we've received a full message
    size_t findDelimiter(char *buf, size_t size) {
        for (size_t i = 0; i < size; i++) {
            if (buf[i] == DELIMITER) {
                return i;
            }
        }
        // return size (invalid index) if we didn't find the delimiter
        return size;
    }
    
    virtual string read() {
        int len;
        do {
            // break if we already have a full message in the buffer
            len = findDelimiter(recv_buf, recv_size);
            if (len < recv_size) {
                break;
            }
            // read more bytes into our buffer
            size_t to_read = BUF_SIZE - recv_size;
            int bytes_read = recv(socket, &recv_buf[recv_size], to_read, 0);
            if (bytes_read == -1) {
                throw "Error: recv()";
            }
            recv_size += bytes_read;
        } while (1);
        // extract the first message in our buffer
        const size_t str_size = len + 1;
        recv_size -= str_size;
        string ret(recv_buf, len);
        memmove(&recv_buf[0], &recv_buf[str_size], BUF_SIZE - str_size);
        return ret;
    }
    
    virtual void write(string text) {
        strcpy(send_buf, text.c_str());
        send(socket, &send_buf, strlen(send_buf), 0);
    }

    virtual void write(float f) {
        sprintf(send_buf, "%f", f);
        send(socket, &send_buf, sizeof(float), 0);
    }

    virtual void read(float *f) {
        char buf[sizeof(float)] = { 0 };
        if (recv(socket, buf, BUF_SIZE, 0) != -1) {
            throw "Error: recv()";
        }
        sscanf(buf, "%f", f);
    }

    virtual ~SocketIO(){}
};

// data aggregator to aid our command classes
class DataManager {
    HybridAnomalyDetector detector;
    vector<AnomalyReport> anomalies;
    bool run;
public:
    DataManager():run(true){}
    ~DataManager(){}
    vector<AnomalyReport>& getAnomalies() {
        return anomalies;
    }
    HybridAnomalyDetector& getDetector() {
        return detector;
    }
    void setAnomalies(vector<AnomalyReport> new_anomalies) {
        anomalies.swap(new_anomalies);
    }
    bool running() {
        return run;
    }
    void stop() {
        run = false;
    }
};

class Command{
    string description;
protected:
    DefaultIO *dio;
    DataManager *dataManager;
public:
	Command(string& description, DefaultIO *dio, DataManager *dataManager)
        :description(description),dio(dio),dataManager(dataManager){}
	virtual void execute()=0;
    string getDescription() {
        return description;
    }

    // helper function to write a local file
    // with data read from default io
    void writeFile(string dst_file_path) {
        ofstream dst_file;
        // make sure we successfully opened the file
        dst_file.open(dst_file_path, ios::out);
        if (dst_file.is_open() == false) {
            throw "Failed to open " + dst_file_path;
        }
        
        // read line by line (until "done") and write it into the file
        string line;
        do {
            line = dio->read();
            if (line.compare("done") == 0) {
                break;
            }
            dst_file << line << endl;
        } while (true);

        dst_file.close();        
    }

    // helper function to read a local file
    // and send its data using dio->write
    void readFile(string src_file_path) {
        ifstream src_file;
        src_file.open(src_file_path);
        if (src_file.is_open() == false) {
            throw "Failed to open " + src_file_path;
        }
        // read line by line (until "done") and write using default io
        string line;
        do {
            src_file >> line;
            dio->write(line);
            dio->write("\n");
        } while (line.compare("done") != 0);
    }
	virtual ~Command(){}
};

// upload a time series csv file
class CommandUpload : public Command {
public:
    CommandUpload(string& description, DefaultIO *dio, DataManager *dataManager)
        :Command(description, dio, dataManager){}
	virtual ~CommandUpload(){}
    void execute() {
        dio->write("Please upload your local train CSV file.\n");
        writeFile("anomalyTrain.csv");
        dio->write("Upload complete.\n");
        
        dio->write("Please upload your local test CSV file.\n");
        writeFile("anomalyTest.csv");
        dio->write("Upload complete.\n");
    }
};
// algorithm settings
class CommandSettings : public Command {
public:
    CommandSettings(string& description, DefaultIO *dio, DataManager *dataManager)
        :Command(description, dio, dataManager){}
	virtual ~CommandSettings(){}
    void execute() {
        float correlation = dataManager->getDetector().getThreshold();
        dio->write("The current correlation threshold is ");
        dio->write(correlation);
        dio->write("\n");
        dio->write("Type a new threshold\n");
        float new_correlation;
        dio->read(&new_correlation);
        while (new_correlation < 0 || new_correlation > 1) {
            dio->write("please choose a value between 0 and 1.\n");
            dio->read(&new_correlation);
        }
        dataManager->getDetector().setThreshold(new_correlation);
    }
};
// detect anomalies
class CommandDetect : public Command {
public:
    CommandDetect(string& description, DefaultIO *dio, DataManager *dataManager)
        :Command(description, dio, dataManager){}
	virtual ~CommandDetect(){}
    void execute() {
        auto& detector = dataManager->getDetector();
        TimeSeries trainData("anomalyTrain.csv");
        TimeSeries testData ("anomalyTest.csv");
        detector.learnNormal(trainData);
        auto anomalies = detector.detect(testData);
        dataManager->setAnomalies(anomalies);
        dio->write("anomaly detection complete.\n");
    }
};
// display results
class CommandResults : public Command {
public:
    CommandResults(string& description, DefaultIO *dio, DataManager *dataManager)
        :Command(description, dio, dataManager){}
	virtual ~CommandResults(){}
    void execute() {
        auto& anomalies = dataManager->getAnomalies();
        for (AnomalyReport report : anomalies) {
            float step = (float)report.timeStep;
            dio->write(step);
            dio->write("\t");
            dio->write(report.description);
            dio->write("\n");
        }
        dio->write("Done.\n");
    }
};
// upload anomalies and analyze results
class CommandAnalyze : public Command {
public:
    CommandAnalyze(string& description, DefaultIO *dio, DataManager *dataManager)
        :Command(description, dio, dataManager){}
	virtual ~CommandAnalyze(){}
    bool conjunction(int a, int b, int c, int d) {
        return ((a >= c && a <= d) || (b >= c && b <= d))
            || ((c >= a && c <= b) || (d >= a && d <= b));
    }
    void execute() {
        auto& anomalies = dataManager->getAnomalies();
        TimeSeries testData("anomalyTest.csv");
        int N = testData.rows();
        float FP, FN, TP, TN, P;
        FP = FN = TP = TN = P = 0;

        dio->write("Please upload your local anomalies file.\n");
        ifstream file;
        writeFile("analysis.txt");
        file.open("analysis.txt");
        if (file.is_open() == false) {
            throw "Failed to open analysis.txt";
        }
        dio->write("Upload complete.\n");

        string line;
        int x, y;
        vector<int> input_timeSteps;
        // read analysis' data into a input_timeSteps
        while (getline(file, line)) {
            stringstream ss(line);
            ss >> x;
            ss.ignore(1);
            ss >> y;
            // compute N and P
            N -= (y - x + 1);
            input_timeSteps.push_back(x);
            input_timeSteps.push_back(y);
            P++;
        }

        // read anomalies into anomaly_timeSteps
        string desc;
        vector<int> anomaly_timeSteps;
        for (size_t i = 0; i < anomalies.size(); i++) {
            AnomalyReport& report = anomalies[i];
            anomaly_timeSteps.push_back(report.timeStep);
            int end = report.timeStep;
            for (; i < anomalies.size(); i++) {
                AnomalyReport& other = anomalies[i + 1];
                if (other.timeStep != end + 1 
                    || report.description.compare(other.description)) {
                    break;
                }
                end = other.timeStep;
            }
            anomaly_timeSteps.push_back(end);
        }

        // compute FP
        for (size_t i = 0; i < anomaly_timeSteps.size(); i+=2) {
            int counter = 0;
            int a = anomaly_timeSteps[i];
            int b = anomaly_timeSteps[i + 1];
            for (size_t j = 0; j < input_timeSteps.size(); j+=2) {
                int c = input_timeSteps[j];
                int d = input_timeSteps[j + 1];
                if (!conjunction(a, b, c, d)) {
                    counter++;
                }
            }
            if (counter == P) {
                FP++;
            }
        }

        // compute TP
        for (size_t j = 0; j < input_timeSteps.size(); j+=2) {
            int a = input_timeSteps[j];
            int b = input_timeSteps[j + 1];
            for (size_t i = 0; i < anomaly_timeSteps.size(); i+=2) {
                int c = anomaly_timeSteps[i];
                int d = anomaly_timeSteps[i + 1];
                if (conjunction(a, b, c, d)) {
                    TP++;
                    break;
                }
            }
        }

        // compute TP/P and FP/N 
        // round to 3 decimal points and get rid of trailing zeros
        const float TP_P = floorf((TP / P)*1000) / 1000;
        const float FP_N = floorf((FP / N)*1000) / 1000;
        stringstream ss_tp;
        ss_tp << TP_P;
        stringstream ss_fp;
        ss_fp << FP_N;
        string tp_str = ss_tp.str();
        string fp_str = ss_fp.str();

        // write TP/P and FP/N using default io
        dio->write("True Positive Rate: ");
        dio->write(tp_str);
        dio->write("\n");
        dio->write("False Positive Rate: ");
        dio->write(fp_str);
        dio->write("\n");
    }
};
// exit
class CommandExit : public Command {
public:
	CommandExit(string& description, DefaultIO *dio, DataManager *dataManager)
        :Command(description, dio, dataManager){}
	virtual ~CommandExit(){}
    void execute() {
        dataManager->stop();
    }
};

#endif /* COMMANDS_H_ */
