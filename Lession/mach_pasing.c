#include<mach/mach.h> 

struct message { 
    mach msg header t header; 
    int data; 
}; 

mach port t client; 
mach port t server; 

    /* Client Code */ 
struct message message; 
// construct the header 
message.header.msgh size = sizeof(message); 
message.header.msgh remote port = server; 
message.header.msgh local port = client; 
// send the message 
mach_msg(&message.header, // message header 
        MACH_SEND_MSG, // sending a message 
        sizeof(message), // size of message sent 
        0, // maximum size of received message - unnecessary 
        MACH_PORT_NULL, // name of receive port - unnecessary 
        MACH_MSG_TIMEOUT_NONE, // no time outs 
        MACH_PORT_NULL // no notify port 
); 
    
    /* Server Code */ 
struct message message; // receive the message 
mach_msg(&message.header, // message header 
        MACH_RCV_MSG, // sending a message 
        0, // size of message sent 
        sizeof(message), // maximum size of received message 
        server, // name of receive port 
        MACH_MSG_TIMEOUT_NONE, // no time outs 
        MACH_PORT_NULL // no notify port 
); 
