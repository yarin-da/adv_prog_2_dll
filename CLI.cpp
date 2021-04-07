/* 
 * CLI.cpp
 *
 * Author: 316383298 Yarin Dado
 */

#include "CLI.h"
#include <string>

CLI::CLI(DefaultIO *dio):dio(dio) {}

void initCommands(map<string, Command*>& commands, DefaultIO *stdio, DataManager *dataManager) {
    string desc;
    desc = "upload a time series csv file";
    commands["1"] = new CommandUpload(desc, stdio, dataManager);
    desc = "algorithm settings";
    commands["2"] = new CommandSettings(desc, stdio, dataManager);
    desc = "detect anomalies";
    commands["3"] = new CommandDetect(desc, stdio, dataManager);
    desc = "display results";
    commands["4"] = new CommandResults(desc, stdio, dataManager);
    desc = "upload anomalies and analyze results";
    commands["5"] = new CommandAnalyze(desc, stdio, dataManager);
    desc = "exit";
    commands["6"] = new CommandExit(desc, stdio, dataManager);    
}

void printMenu(map<string, Command*>& commands, DefaultIO *dio) {
    dio->write("Welcome to the Anomaly Detection Server.\n");
    dio->write("Please choose an option:\n");
    for (auto& pair : commands) {
        string key = pair.first;
        Command *val = pair.second;
        dio->write(key);
        dio->write(".");
        dio->write(val->getDescription());
        dio->write("\n");
    }
}

void CLI::start(){
    DataManager dataManager;
    map<string, Command*> commands;
    initCommands(commands, dio, &dataManager);
    
    while (dataManager.running()) {
        printMenu(commands, dio);
        string input = dio->read();
        if (commands.count(input)) {
            commands.at(input)->execute();
        }
    }

    for (auto& pair : commands) {
        delete pair.second;
    }
}

CLI::~CLI() {
}

