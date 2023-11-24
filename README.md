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

### SUBMISSION

I have done this assignment completely on my own. I have not copied it, nor have I given my solution to anyone else. I understand that if I am involved in plagiarism or cheating I will have to sign an official form that I have cheated and that this form will be stored in my official university record. I also understand that I will receive a grade of "0" for the involved assignment and my grade will be reduced by one level (e.g., from "A" to "A-" or from "B+" to "B") for my first offense, and that I will receive a grade of "F" for the course for any additional offense of any kind.

By signing my name below and submitting the project, I confirm the above statement is true and that I have followed the course guidelines and policies.

Submission date: November 8 2023 

Team member 1 name: VENKATNARAYAN GNANAGURUPARAN

Team member 2 name (N/A, if not applicable): Sowmya Achu

