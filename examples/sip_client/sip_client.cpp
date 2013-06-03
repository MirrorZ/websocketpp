#include "../../src/roles/client.hpp"
#include "../../src/websocketpp.hpp"

#include <iostream>

using websocketpp::client;

class sip_client_handler : public client::handler {
public:
    
    bool received;
	
     void on_open(connection_ptr con){

      //connection can be used now      
         std::cout << "Connection Ready" << std::endl;
      //SIP_msg :example OPTIONS message
    	std::string SIP_msg="OPTIONS sip:carol@chicago.com SIP/2.0\r\nVia: SIP/2.0/WS df7jal23ls0d.invalid;rport;branch=z9hG4bKhjhs8ass877\r\nMax-Forwards: 70\r\nTo: <sip:carol@chicago.com>\r\nFrom: Alice <sip:alice@atlanta.com>;tag=1928301774\r\nCall-ID: a84b4c76e66710\r\nCSeq: 63104 OPTIONS\r\nContact: <sip:alice@pc33.atlanta.com>\r\nAccept: application/sdp\r\nContent-Length: 0\r\n\r\n";
	
	received=false;

	con->send(SIP_msg.c_str());
    }
	
    void on_close(connection_ptr con) {
       // no longer safe to use the connection
       std::cout << "connection closed" << std::endl;
    }

    void on_fail(connection_ptr con) {
        std::cout << "connection failed" << std::endl;
    }
    
    void on_message(connection_ptr con, message_ptr msg) {  
	  std::cout << msg->get_payload()  << std::endl; 
	 received=true;
    }
   
   
    
    
    int m_case_count;
};


int main(int argc, char* argv[]) {
    std::string uri = "ws://localhost:9001/";
    
    if (argc == 2) {
        uri = argv[1];
        
    } 
    else if (argc > 2) {
        std::cout << "Usage: `sip_client test_url`" << std::endl;
    }
    
    try {
        client::handler::ptr handler(new echo_client_handler());
        client::connection_ptr con;
        client endpoint(handler);
        
        endpoint.alog().unset_level(websocketpp::log::alevel::ALL);
        endpoint.elog().unset_level(websocketpp::log::elevel::ALL);
        
	con = endpoint.get_connection(uri);
	con->add_subprotocol("sip");
	con->set_origin("http://zaphoyd.com");
        endpoint.connect(con);
                
        endpoint.run();
         	
        while(!boost::dynamic_pointer_cast<sip_client_handler>(handler)->received) {
            boost::this_thread::sleep(boost::posix_time::milliseconds(100)); 
        }
        
        std::cout << "done" << std::endl;
        
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}
	
       
