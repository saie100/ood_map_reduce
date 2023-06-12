

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
#include <sstream>
#include <condition_variable>
#include "headers/Socket.h"
#include "headers/Executive.h"
#include "headers/FileManager.h"
 
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "headers/FileManager.h"
#include "headers/Sort.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::cerr;
using std::vector;
using std::thread;
using std::mutex;
using std::to_string;

std::vector<int> convertStringToVector(const std::string& inputString) {
    std::vector<int> outputVector;
    // Find the position of the colon ':' in the input string
    size_t colonPosition = inputString.find(':');
    // Extract the substring after the colon
    std::string numbersString = inputString.substr(colonPosition + 1);
    // Create a stringstream from the numbers substring
    std::stringstream ss(numbersString);
    // Loop through each number separated by commas and add it to the output vector
    std::string numberString;
    while (std::getline(ss, numberString, ',')) {
        // convert the number string to an integer
        try {
            int number = std::stoi(numberString);
            outputVector.push_back(number);
        } catch (const std::exception& e) {
            std::cerr << "Invalid number format: " << numberString << std::endl;
        }
    }
    return outputVector;
}

                    

void mapProcess(int threadId, string mapDllPath, string inputDir, string outputFilePath) {

#ifdef _WIN32
  // create DLL handles
  HINSTANCE mapDLL = LoadLibraryA(mapDllPath.c_str());
#else
  void* MaplibraryHandle = dlopen(mapDllPath.c_str(), RTLD_LAZY);

  if (!MaplibraryHandle) {
    printf("Error: %s\n", dlerror());
    exit(1);
  }

  typedef void (*Map)(const string&, const string&, const string&);

  Map map = (Map) dlsym(MaplibraryHandle, "map");

  if (!map) {
    printf("Error: %s\n", dlerror());
    exit(1);
  }
#endif

  vector<string> inputFilePaths = FileManager::getFilesFromDir(inputDir);
  int filesMapped = 0;
  for (string inputFilePath : inputFilePaths) {
    auto fileName = FileManager::getFilename(inputFilePath);
    auto firstUnderscore = fileName.find("_");
    auto secondUnderscore = fileName.find("_", firstUnderscore + 1);
    auto threadNum = fileName.substr(firstUnderscore + 1, secondUnderscore - firstUnderscore - 1);
    if (threadNum == to_string(threadId)) {
      auto inputFile = FileManager::readFile(inputFilePath);
      auto threadOutputPath = outputFilePath;
      size_t period = threadOutputPath.find(".");
      threadOutputPath = threadOutputPath.substr(0, period);
      threadOutputPath = threadOutputPath + "_" + to_string(threadId) + "_" + ".txt";
      auto inputFileName = inputFile[0];
      auto inputContent = inputFile[1];
#ifdef _WIN32
      // use map dll to map
      if (mapDLL != NULL) {
        funcMap map = (funcMap)GetProcAddress(mapDLL, "map");
        if (map != NULL) {
          map(inputFileName, inputContent, threadOutputPath);
        } else {
          cout << "Map DLL not found" << endl;
          exit(1);
        }
      }
#else
      map(inputFileName, inputContent, threadOutputPath);
#endif
    }
  }
}


void reduceProcess(int threadId, string reduceDllPath, string inputDir, string tempDir) {

  #ifdef _WIN32
    // create DLL handles
    HINSTANCE reduceDLL = LoadLibraryA(reduceDllPath.c_str());
  #else
    void* ReducelibraryHandle = dlopen(reduceDllPath.c_str(), RTLD_LAZY);

    if (!ReducelibraryHandle) {
      printf("Error: %s\n", dlerror());
      exit(1);
    }

    typedef void (*ProcessSortResult)(const string, const string);

    ProcessSortResult processSortResult = (ProcessSortResult) dlsym(ReducelibraryHandle, "processSortResult");

    if (!processSortResult) {
      printf("Error: %s\n", dlerror());
      exit(1);
    }
  #endif

  vector<string> inputFilePaths = FileManager::getFilesFromDir(inputDir);
  int filesReduced = 0;
  for (string inputFilePath : inputFilePaths) {
    auto fileName = FileManager::getFilename(inputFilePath);
    auto firstUnderscore = fileName.find("_");
    auto secondUnderscore = fileName.find("_", firstUnderscore + 1);
    auto threadNum = fileName.substr(firstUnderscore + 1, secondUnderscore - firstUnderscore - 1);

    if (threadNum == to_string(threadId)) {
      auto inputFile = FileManager::readFile(inputFilePath);
      auto inputFileName = inputFile[0];
      auto inputContent = inputFile[1];

    #ifdef _WIN32
      // use reduce dll to sort and reduce
      if (reduceDLL != NULL) {
        funcReduce processSortResult = (funcReduce)GetProcAddress(reduceDLL, "processSortResult");
        if (processSortResult != NULL) {
          processSortResult(inputFilePath, tempDir);
        } else {
          cout << "Reduce DLL not found" << endl;
          exit(1);
        }
      }
    #else
      processSortResult(inputFilePath, tempDir);
    #endif
    }
  }
}

// Socket class constructor
Socket::Socket(string type, string mapDLL, string reduceDLL, string inputReduceDir, string tempDir, string outputMapDir){
    this->type = type;
    this->mapDLL = mapDLL;
    this->reduceDLL = reduceDLL;
    this->inputReduceDir = inputReduceDir;
    this->tempDir = tempDir;
    this->outputMapDir = outputMapDir;
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
            if(str_buf.find("start mapper:") != std::string::npos){
                // message from controller "start mapper:0,1,2,3    
                vector<int> thread_id = convertStringToVector(str_buf);
                
                cout << thread_id[0] << endl;

                std::vector<std::thread> mapThreads(thread_id.size());
                // Start each thread
                for (int i = 0; i < thread_id.size(); ++i) {
                    mapThreads[i] = thread(mapProcess, i, mapDLL, tempDir, outputMapDir);
                }
                
                // Wait for each thread to finish
                for (int i = 0; i < thread_id.size(); ++i) {
                    mapThreads[i].join();
                }
                cout << "Mapping complete!\n" << "Sorting and aggregating map output..." << endl;
            }
            else if(str_buf.find("start reducer:") != std::string::npos){

                // message from controller "start reduce:0,1,2,3    
                vector<int> thread_id = convertStringToVector(str_buf);
                std::vector<std::thread> reduceThreads(thread_id.size());
                
                // Start each thread
                for (int i = 0; i < thread_id.size(); ++i) {
                    reduceThreads[i] = thread(reduceProcess, i, reduceDLL, inputReduceDir, tempDir);
                }
                // Wait for each thread to finish
                for (int i = 0; i < thread_id.size(); ++i) {
                    reduceThreads[i].join();
                }

                cout << "Sorting and aggregating complete!\n" << "Aggregating sorted output..." << endl;
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
    
