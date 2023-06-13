#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <map>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <dlfcn.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

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
        void sendMessage(string message, int port_num);


        void getPortToQ();
        //void waitForThreads();

        static mutex msg_locker;
        
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

        // stores messages for the sendThread to use
        vector <string> messageQueue;
        
        // condition variable checks if messageQueue is not empty
        // if message queue is not empty sendThread activates
        static std::condition_variable cv;

        //std::condition_variable thread_wait_cv;
        
        //int controller_thread_count;
        static std::map <int, vector<string>> port_to_queue;


        //bool controller_terminate;
        //bool controller_stop;

        string mapDLL;
        string reduceDLL;
        string inputReduceDir;
        string tempDir;
        string outputMapDir;
        
        
};