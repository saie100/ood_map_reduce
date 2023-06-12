#ifndef STUB_H
#define STUB_H

#include <iostream>
#include <unistd.h>
#include "Socket.h"

class Stub {
public:
    Stub();
    ~Stub();
    void startListening(int port);

private:
    Socket* socket;
    std::string receiveMessage();
    void processMessage(const std::string& message);
    void createMapProcess();
    void createReduceProcess();
};

#endif  // STUB_H
