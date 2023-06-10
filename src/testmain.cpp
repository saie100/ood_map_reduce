#include "headers/Socket.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::cerr;
using std::vector;
using std::thread;
using std::mutex;

// this is an example program to demo use of methods
int main(){
    
    Socket controller("controller");
    Socket stub1("stub");

    stub1.listenTo(8080, 3);
    controller.connectTo(8080);
    
    while(1){
        controller.sendMessage("start stub");
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        
        controller.sendMessage("stop stub");
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        
    }
    cout << "Done" << endl;
    return 0;
}