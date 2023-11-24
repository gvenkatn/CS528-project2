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
int serverSD;

int ret_sequence_number(string msg){
    /*
    * This function returns the sequence number from the ping
    */

    int seq_number = 0;
    string seq_num = "";

    bool flag =false;
    for(char c:msg){
        if(c == ',' && flag == false){
            flag = true;
        } 
        else if(flag == true && c == ','){
            flag = false;
        }
        else if(flag == true){
            seq_num += c; 
        }
    }
    seq_number = atoi(seq_num.c_str());
    return seq_number;
}

std::string getCurrentTimeAsString() {
    //This function returns the current time as string

     auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm time_info = *std::localtime(&now);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &time_info);

    return std::string(buffer);
}

void termination_handler(int signum) {
   // This function is to catch the Ctrl+C signal ans close the socket
   // and exit the program
    
   cout<<endl;
   std::cout << "TERMINATING SERVER" << std::endl;
   close(serverSD);
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
     
    // setup server UDP socket
    // notice the use of SOCK_DGRAM for UDP
    sockaddr_in servAddr, cliAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    bzero((char*)&cliAddr, sizeof(cliAddr));
    
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);

    // server socket descriptor
    serverSD = socket(AF_INET, SOCK_DGRAM, 0);
    if(serverSD < 0)
    {
        cerr << "Socket error!" << endl;
        exit(EXIT_FAILURE);
    }
    
    // bind socket
    int bindSock = ::bind(serverSD, (const struct sockaddr *)&servAddr, sizeof(servAddr));
    if(bindSock < 0)
    {
        cerr << "Bind error!" << endl;
        exit(EXIT_FAILURE);
    }
    
    socklen_t len;
    //int MSG_CONFIRM = 0;

    cout << "Wait for client message..." << endl;
    while(1)
    {
        
        
        memset(&msg, 0, sizeof(msg));
        len = sizeof(&cliAddr);
        
        int bytes = 0;

        
        bytes = recvfrom(serverSD, (char *)&msg, sizeof(msg), MSG_WAITALL, (struct sockaddr *)&cliAddr, &len);

        // generate a random number between 0-99
        // if random number less than 30, we consider the received message lost and do not respond
        srand((unsigned)time(NULL));
        int random = rand() % 100;
        cout << "Random: " << random << endl;
        if(random < 30)
        {
            continue;
        }

        // otherwise, message not lost
        if(DEBUG) cout << "Client message: " << msg << endl;

        // TODO: prepare the server response
        int seq_num = ret_sequence_number(msg);
        
        auto timestamp = chrono::duration_cast<chrono::nanoseconds>(chrono::steady_clock::now().time_since_epoch()).count();

        auto start_time = std::chrono::steady_clock::now();
        
        string echo_message ="echo, " + to_string(seq_num) + ", " + to_string(timestamp);

        // send the server response message
        bytes = sendto(serverSD, echo_message.c_str(), echo_message.size(), MSG_CONFIRM, (const struct sockaddr *)&cliAddr, sizeof(cliAddr));
        
        cout << echo_message << endl;
    
        
    }

    // close socket descriptor
    close(serverSD);

    return EXIT_SUCCESS;
}

