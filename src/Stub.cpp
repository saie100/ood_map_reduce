#include "Stub.h"

Stub::Stub() {
    socket = new Socket("stub");
}

Stub::~Stub() {
    delete socket;
}

void Stub::startListening(int port) {
    socket->connectTo(port);

    while (true) {
        std::string message = receiveMessage();
        processMessage(message);
    }
}

std::string Stub::receiveMessage() {
    std::string message;
    // TODO: Receive the message using the socket's receive functionality
    // Example: message = socket->receiveMessage();
    return message;
}

void Stub::processMessage(const std::string& message) {
    if (message == "start map") {
        createMapProcess();
    } else if (message == "start reduce") {
        createReduceProcess();
    } else {
        std::cout << "Unknown message received: " << message << std::endl;
    }
}

void Stub::createMapProcess() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process code for Map task
        // ...
        exit(0);
    } else if (pid < 0) {
        // Error handling
        // ...
    } else {
        // Parent process code
        // ...
    }
}

void Stub::createReduceProcess() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process code for Reduce task
        // ...
        exit(0);
    } else if (pid < 0) {
        // Error handling
        // ...
    } else {
        // Parent process code
        // ...
    }
}