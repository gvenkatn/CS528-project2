#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono> 
#include <ctime>
#include <sstream>
#include <signal.h>

using namespace std;

bool DEBUG = false;
int clientSD;

std::string getCurrentTimeAsString() {
    //Return current time as string

    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm time_info = *std::localtime(&now);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &time_info);

    return std::string(buffer);
}

void termination_handler(int signum) {
    //Handle CTRL+C Termination signal

   cout<<endl;
   std::cout << "TERMINATING: Input" << std::endl;
   close(clientSD);
   exit(signum);
}

int main(int argc, char *argv[])
{
    // get server port number
    if(argc != 2)
    {
        cerr << "Usage: port" << endl;
        exit(EXIT_FAILURE);
    }
    int port = atoi(argv[1]);


    signal(SIGINT, termination_handler);

    // server buffer
    char msg[1024];

    sockaddr_in servAddr;
    
    bzero((char*)&servAddr, sizeof(servAddr));
    
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
    
    // server socket descriptor
    clientSD = socket(AF_INET, SOCK_DGRAM, 0);
    if(clientSD < 0)
    {
        cerr << "Socket error!" << endl;
        exit(EXIT_FAILURE);
    }

    std::chrono::seconds duration(180);

    auto start = std::chrono::steady_clock::now();
      

    socklen_t len;
    //int MSG_CONFIRM = 0;
    int sequence_number = 1;

    while(true){  
        auto current = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current - start);
        if (elapsed >= duration) {
            std::cout << "TERMINATING: Duration 3 Minutes" << std::endl;
            break;
        }
        
        auto timestamp = chrono::duration_cast<chrono::nanoseconds>(chrono::steady_clock::now().time_since_epoch()).count();

        auto start_time = std::chrono::steady_clock::now();
        
        string pingMessage ="ping, " + to_string(sequence_number) + ", " + to_string(timestamp);
        
        std::cout<<pingMessage<<endl;

        int bytes = sendto(clientSD, pingMessage.c_str(), pingMessage.size(),  
                        MSG_CONFIRM, (const struct sockaddr *)&servAddr, 
                        sizeof(servAddr));
        
        sleep(1); //Wait for 1 second

        bytes = recvfrom(clientSD, (char *)&msg, sizeof(msg), 
                        MSG_DONTWAIT, (struct sockaddr *)&servAddr, 
                        &len);

        int rtt = -1;
        if (bytes > 0) {
            auto end_time = chrono::steady_clock::now();

            // Parse and calculate RTT
            string responseString(msg, bytes);
            size_t pos = responseString.find_last_of(",");
        
            rtt = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
            cout << msg << endl;
            cout<< "RTT : "<< rtt<<endl;
            

        } else {
            cout << "Client ping timed out" << endl;
            
        }

        cout<<endl;
        sleep(1); // Ping after 1 second
        sequence_number++;
    }
    close(clientSD);

    return 0;
       
}