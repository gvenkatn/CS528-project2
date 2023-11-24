# Binghamton University, Fall 2023

## CS428/528 Project-2: UDP Pinger

### SUMMARY

+ There are 2 PARTS :- 
    + PART 1  
        + udppinger-server1.cc - Simulates a 30% chance of losing the client's packets.
        + udppinger-client1.cpp - The client pings the server every 2 seconds and terminate the program after three minutes. The client waits for 1 second for a reply if it doesn't receive a reply. It transmits the next ping 
    + PART 2 - udppinger-server2.cc and udppinger-client2.cpp.feature
        + udppinger-server2.cc - Similar to Part 1, additionally the server has time-out o 30 seconds. It will terminate if it doesn't receive a ping for 30 seconds. 
        + udppinger-client2.cpp -Additionally calculates The minimum RTT, maximum RTT, total number of RTTs, packet loss rate (in percentage) and average RTTs.



### NOTES, KNOWN BUGS, AND/OR INCOMPLETE PARTS
+ MSG_CONFIRM is not compatible in mac, it is compatible only in linux based machines. It is set 0 in mac / might have to remove to work on linux.
+ No Bugs detected so far

### REFERENCES
+ [geeksforgeeks - Chrono](https://www.geeksforgeeks.org/chrono-in-c/)
+ [geeksforgeeks - UDP](https://www.geeksforgeeks.org/user-datagram-protocol-udp/)
+ [stackoverflow - MSG_CONFIRM](https://stackoverflow.com/questions/16594387/why-should-i-use-or-not-use-msg-confirm)

### INSTRUCTIONS
+ Execute the below commands for Part 1 :
    + To run the server
    +       g++ -o server1 udppinger-server1.cc  
            ./server1 <port number eg.52837>
    + To run the client
    +       g++ -o client1 udppinger-client1.cpp
            ./client1 <port number eg.52837>

+ Execute the below commands for Part 2 : 
    + To run the server
    +       g++ -o server2 udppinger-server2.cc
            ./server2 <port number eg.52837>
    + to run the client
    +       g++ -o client2 udppinger-client2.cpp
             ./client2 <port number eg.52837>

Teammate: Soumya Achu
