

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "headers/Socket.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::cerr;
using std::vector;
using std::thread;
using std::mutex;



    // Socket class constructor
    Socket::Socket(string type){
        this->type = type;
    };

    // Socket class destructor
    Socket::~Socket(){
        // close all opened sockets
        for(int socket : socket_connection){
            close(socket);
        }
    };

    // this method opens a socket and listens to port "port_num"
    // this method is called by the stubs and controller 
    void Socket::listenTo(int port_num, int conn_num){
        int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

        if(socket_fd < 0) {
            cerr << "socket_fd creation failed" << endl;
            exit(1);
        } 
        int opt = 1;

        if( setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) ){
            cerr << "setsocketopt failed" << endl;
            exit(1);
        }

        struct sockaddr_in address;
        int addrlen = sizeof(address);

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port_num);

        if (bind(socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
            cerr << "bind failed" << endl;
            exit(1);
        }

        // sets the max number of connections in the
        // connection queue that can be created by socket_fd
        listen(socket_fd, conn_num);

        // creates multiple socket connections
        // depending on the specified "conn_num" variable
        for(int i=0; i< conn_num; i++){
            // create a thread to listen on port and store messages
            thread thr = thread(&Socket::listenThread, this, socket_fd, &address, addrlen);
            thr.detach();
        }
        
        // add socket_fd to list of socket that needs to be closed at end of program
        socket_connection.push_back(socket_fd);
    };

    // this method connects to a pre-exsisting socket
    // and sends data into the socket via a message queue
    void Socket::sendThread(int port_num){
        int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

        if(socket_fd < 0) {
            cerr << "socket_fd creation failed" << endl;
            exit(1);
        } 
        int opt = 1;

        if( setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) ){
            cerr << "setsocketopt failed" << endl;
            exit(1);
        }

        struct sockaddr_in address;

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port_num);

        int status = connect(socket_fd, (struct sockaddr*)&address, sizeof(address));
        
        if (status < 0) {
            cerr<< "socket connection failed" << endl;
            exit(1);
        }

        // add socket_fd to list of socket that needs to be closed at end of program
        socket_connection.push_back(socket_fd);
        
        string message;
        while(1){
            std::unique_lock<mutex> ul(locker);
            cv.wait(ul, [this]() {return !messageQueue.empty();});
        
            message = messageQueue.front();
            cout << "Sending:    " << message << endl;    
            send(socket_fd, message.c_str(), message.size(), 0);
            messageQueue.erase(messageQueue.begin());
        }
    }

    // this method connects to a pre-exsiting socket
    // this method is called by the controller and threads
    void Socket::connectTo(int port_num){
        thread thr(&Socket::sendThread, this, port_num);
        thr.detach();
    }

    // adds message to message queue and notifies sendThread
    void Socket::sendMessage(string message){
        locker.lock();
        messageQueue.push_back(message);
        cv.notify_one();
        locker.unlock();
    }


    // this is a private method cannot be called directly
    // please create the listener functionality within this method in while loop
    void Socket::listenThread(int socket_fd, sockaddr_in *address, int addrlen){

        int new_socket_fd = accept(socket_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        socket_connection.push_back(new_socket_fd);
        
        if (new_socket_fd < 0) {
            cerr<< "accept socket connection failed" << endl;
            exit(1);
        }
        
        // create a list of activation message
        string controller_msg1 = "done map";
        string controller_msg2 = "done reduce";
        string stub_msg = "start stub";
        string stub_msg2 = "stop stub";
        
        // listens for message, then execute functionality based on message receieved
        while(1){
            
            char buffer[1024];
            int valread = read(new_socket_fd, buffer, sizeof(buffer));
            string str_buf = string(buffer);
            if(this->type == "stub"){
                if(str_buf.find(stub_msg) != std::string::npos){
                    cout << "Starting stub!" << endl;
                }
                else if(str_buf.find(stub_msg2) != std::string::npos){
                    cout << "Stopping stub!" << endl;
                }
            }
            else if(this->type == "controller"){
                if(str_buf.find(controller_msg1) != std::string::npos){
                    cout << "Do something 1" << endl;
                }
                else if(str_buf.find(controller_msg2) != std::string::npos){
                    cout << "Do something 2" << endl;
                }
            }
        }
    }
    
