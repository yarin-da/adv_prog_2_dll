/* 
 * Server.h
 *
 * Author: 316383298 Yarin Dado
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "CLI.h"
#include "commands.h"

using namespace std;

class ClientHandler
{
public:
    virtual void handle(int clientID) = 0;
    virtual ~ClientHandler() {}
};

class AnomalyDetectionHandler:public ClientHandler
{
public:
    virtual void handle(int clientID);
    virtual ~AnomalyDetectionHandler() {}
};

class Server {
    volatile bool accept_clients = true;
    // the thread that accepts clients
    thread *accept_thread;
    // the server's socket that accepts clients
    int server_socket;
public:
    Server(int port);
    ~Server();
    void start(ClientHandler& ch);
    void stop();
};

#endif /* SERVER_H_ */