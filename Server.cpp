/* 
 * Server.cpp
 *
 * Author: 316383298 Yarin Dado
 */

#include "Server.h"
#include <signal.h>
#include <cerrno>

Server::Server(int port)
{
    server_socket = socket(AF_INET,SOCK_STREAM,0);
    if (server_socket < 0) {
        throw "Error: socket()";
    }
    sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_socket, (struct sockaddr*)&serv, sizeof(serv)) != 0) {
        throw "Error: bind()";
    }
    const int max_clients = 5;
    if (listen(server_socket, max_clients) != 0) {
        throw "Error: listen()";
    }
}

void AnomalyDetectionHandler::handle(int clientID) {
	SocketIO socketIO(clientID);
	CLI cli(&socketIO);
	cli.start();
}

void handle_alarm(int signum) {}

void Server::start(ClientHandler& ch) {
    // set a handler function for SIGALRM
    struct sigaction sig_alarm; 
    sig_alarm.sa_handler = &handle_alarm;
    sigemptyset(&sig_alarm.sa_mask);
    sig_alarm.sa_flags = 0; 
    if (sigaction(SIGALRM, &sig_alarm, nullptr)) {
        throw "Error: sigaction()";
    }

    // initialize a sigset with SIGALRM
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    // block SIGALRM in the main thread
	if(pthread_sigmask(SIG_BLOCK, &mask, nullptr)) {
        throw "Error: pthread_sigmask()";
	}
    
    // start accepting clients in a new thread
    accept_thread = new thread([&]() {
        // initialize a sigset with SIGLARM
		sigset_t mask;
		sigemptyset(&mask);
		sigaddset(&mask, SIGALRM);
        // unblock SIGALRM in this thread
        // this is because we want this thread to handle all SIGALRMS
        // and escape accept's blocking state
		if(pthread_sigmask(SIG_UNBLOCK, &mask, nullptr) != 0) {
	        throw "Error: pthread_sigmask()";
        }
        sockaddr_in client_address;
		socklen_t client_size = sizeof(client_address); 
        // start accepting clients
		while(accept_clients) {
            // call for a SIGALRM in the next second 
            // in order to make sure we'll not be stuck in blocking state 
            alarm(1);
            int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_size);
            // cancel the alarm call
            alarm(0);

            if (client_socket < 0) {
                if (errno != EINTR) {
                    throw "Error: accept()";
                }
                // we received EINTR, which means SIGLARM stopped accept
                // we can break out of the loop because we ran out of clients
                break;
            }

            // start a CLI session with the new client
            ch.handle(client_socket);
            // close the connection with client
            close(client_socket);
		}
    });

}

void Server::stop(){
    // make sure we exit the client accept loop
    accept_clients = false;
    // wait for the thread to finish
    accept_thread->join();
    close(server_socket);
}

Server::~Server() {}
