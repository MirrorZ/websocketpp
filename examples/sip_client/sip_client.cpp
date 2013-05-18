#include "../../src/roles/client.hpp"
#include "../../src/websocketpp.hpp"

#include <iostream>

using websocketpp::client;

class echo_client_handler : public client::handler {
public:
    
    bool received;

    void on_message(connection_ptr con, message_ptr msg) {        
         std::cout << msg->get_payload()  << std::endl;
         m_case_count = atoi(msg->get_payload().c_str());
	 received=true;
    }
   
    void on_open(connection_ptr con){

      //SIP_msg :example OPTIONS message
    	std::string SIP_msg;

    	SIP_msg=" OPTIONS sip:carol@chicago.com SIP/2.0 Via: SIP/2.0/UDP pc33.atlanta.com;branch=z9hG4bKhjhs8ass877 Max-Forwards: 70 To: <sip:carol@chicago.com>  From: Alice <sip:alice@atlanta.com>;tag=1928301774  Call-ID: a84b4c76e66710  CSeq: 63104 OPTIONS  Contact: <sip:alice@pc33.atlanta.com>  Accept: application/sdp  Content-Length: 0/r/n/r/n";

	received=false;
	con->send(SIP_msg.c_str());
    }
    
    void on_fail(connection_ptr con) {
        std::cout << "connection failed" << std::endl;
    }
    
    int m_case_count;
};


int main(int argc, char* argv[]) {
    std::string uri = "ws://localhost:9001/";
    
    if (argc == 2) {
        uri = argv[1];
        
    } 
    else if (argc > 2) {
        std::cout << "Usage: `echo_client test_url`" << std::endl;
    }
    
    try {
        client::handler::ptr handler(new echo_client_handler());
        client::connection_ptr con;
        client endpoint(handler);
        
        endpoint.alog().unset_level(websocketpp::log::alevel::ALL);
        endpoint.elog().unset_level(websocketpp::log::elevel::ALL);
        
	con = endpoint.connect(uri);
                
        endpoint.run();
        
	 std::cout << "case count: " << boost::dynamic_pointer_cast<echo_client_handler>(handler)->m_case_count << std::endl;
        
        for (int i = 1; i <= boost::dynamic_pointer_cast<echo_client_handler>(handler)->m_case_count; i++) {
	     
	  if(boost::dynamic_pointer_cast<echo_client_handler>(handler)->received)
	     endpoint.reset();
            
            std::stringstream url;
            
            url << uri << "runCase?case=" << i << "&agent=WebSocket++/0.2.0-dev";
                        
            con = endpoint.connect(uri);
            
            endpoint.run();
	     }
        
        std::cout << "done" << std::endl;
        
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}
