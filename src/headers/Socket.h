#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>

#include <netinet/in.h>
using std::string;
using std::vector;
using std::mutex;

class Socket{

    public:
        // Socket class constructor
        Socket(string type, string mapDLL, string reduceDLL, string inputReduceDir, string tempDir, string outputMapDir);
        
        // Socket class destructor
        ~Socket();

        // create a socket that listens for connections on a given port
        // this method is called by the controller & stubs 
        void listenTo(int port_num, int total_connections);

        // this method connects to a pre-exsiting socket
        // this method is called by the controller & threads (Map & Reduce)
        void connectTo(int port_num);
            
        // sends message to the pre-established connection
        // this method can only be called by objects that use connectTo
        void sendMessage(string message);

        bool sendSignal;
        
    private:
        // receives messages on opened socket and execute funcionality based on
        // message received
        void listenThread(int socket_fd, sockaddr_in *address, int addrlen);
        
        // sends messages to listner on specified port number  
        void sendThread(int port_num);
        
        // specifies the type of socket (either stub or controller)
        string type;

        // store all socket connection for later deletion within destructor
        vector<int> socket_connection;
        
        mutex locker;

        // stores messages for the sendThread to use
        vector <string> messageQueue;
        
        // condition variable checks if messageQueue is not empty
        // if message queue is not empty sendThread activates
        std::condition_variable cv;

        string mapDLL;
        string reduceDLL;
        string inputReduceDir;
        string tempDir;
        string outputMapDir;
        
        
};