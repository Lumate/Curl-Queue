//
/// \file testclient.cpp
/// \author James Marlowe
/// \brief A simple test to call the router and print the response
//
#include <zmq.hpp>
#include <string>
#include <iostream>
#include "url_request.pb.h"

using namespace std;

int main (int argc, char* argv[])
{
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);
    std::cout << "Connecting to annotation server…" << std::endl;
    socket.connect ("tcp://localhost:5555");

    int reqs = 10;
    if (argc > 1)
        reqs = atoi(argv[1]);
    
    URLRequest data, data2;
    data.set_request_url("http://www.example.com/path");
    data.add_request_headers("Header: whatever");
    
    std::cout << "Sending requests…" << std::endl;
    for (int i=0; i<reqs; i++)
    {
        zmq::message_t msg(data.ByteSize());
        data.SerializeToArray(msg.data(), data.GetCachedSize());
        socket.send(msg);
        zmq::message_t rsp;
        socket.recv(&rsp);
        data2.ParseFromArray(rsp.data(), rsp.size());
    }

    cout << data2.response() << endl;
    cout << data2.response_time() << endl;
    cout << data2.response_status() << endl;
    cout << data2.response_body() << endl;
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
