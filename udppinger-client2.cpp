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

long long min_rtt = 0;
long long max_rtt = 0;
int succ_rtt = 0;
int fail_rtt = 0;
float packet_loss = 0;
long long avg_rtt = 0;

bool first = true;

std::string getCurrentTimeAsString() {
    //Return the current time as string

    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm time_info = *std::localtime(&now);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &time_info);

    return std::string(buffer);
}

void output(){
    //Calculate the RTTs and output the data

    cout<<endl;
    cout<<" - - - DATA - - -"<<endl;
    if(!first){
    cout<<"MIN RTT :"<<min_rtt<<endl;
    cout<<"MAX RTT :"<<max_rtt<<endl;
    cout<<"TOTAL NUMBER OF PACKETS SENT :"<<succ_rtt + fail_rtt<<endl;
    cout<<"SUCCESSFUL RESPONSE :"<< succ_rtt<<endl;
    cout<<"NO RESPONSE :"<<fail_rtt<<endl;

    packet_loss = ((float)fail_rtt / (float)(fail_rtt + succ_rtt))*100.0;
    cout<<"PACKET LOSS :" <<packet_loss<<"%"<<endl;
    
    avg_rtt = avg_rtt / succ_rtt;
    cout<<"AVERAGE RTT :" << avg_rtt <<endl;
    }
    else{
        cout<<"No Response from Server"<<endl;
        cout<<"MIN RTT :"<<min_rtt<<endl;
        cout<<"MAX RTT :"<<max_rtt<<endl;
        cout<<"TOTAL NUMBER OF PACKETS SENT :"<<succ_rtt + fail_rtt<<endl;
        cout<<"SUCCESSFUL RESPONSE :"<< succ_rtt<<endl;
        cout<<"NO RESPONSE :"<<fail_rtt<<endl;
        cout<<"PACKET LOSS :" <<packet_loss <<"%"<<endl;
        cout<<"AVERAGE RTT :" << avg_rtt <<endl;
    }
    cout<<endl;

}

void termination_handler(int signum) {
   cout<<endl;
   std::cout << "TERMINATING: Input" << std::endl;
   close(clientSD);
   output();
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
    int MSG_CONFIRM = 0;
    int sequence_number = 0;
    
    while(true){  
        auto current = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current - start);
        if (elapsed >= duration) {
            std::cout<<endl;
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

        long long rtt = 0;
        if (bytes > 0) {
            auto end_time = chrono::steady_clock::now();

            // Parse and calculate RTT
            string responseString(msg, bytes);
            size_t pos = responseString.find_last_of(",");
        
            rtt = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
            
            if(first){
                min_rtt = rtt;
                max_rtt = rtt;
                first = false;
            }
            else{
                min_rtt = std::min(min_rtt, rtt);
                max_rtt = std::max(max_rtt, rtt);
            }

            succ_rtt++;
            avg_rtt += rtt;
            
            
            cout << msg << endl;
            cout<< "RTT : "<< rtt<<endl;
            cout<<endl;

        } else {
            cout << "Client ping timed out" << endl;
            fail_rtt++;
        }
    
        sleep(1); // Ping after 1 second
        sequence_number++;
    }

    output();

    return 0;
       
}