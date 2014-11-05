//
/// \file testclient.cpp
/// \author Nathan Eloe (from http://zguide.zeromq.org/cpp:hwclient)
/// \brief A simple, dumb test client to test the threaded server
//
#include <zmq.hpp>
#include <string>
#include <iostream>
#include "url_request.pb.h"
#include "protobuf/zmq_wrapper/zmq_pbuff.h"

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
    for (int i=0; i<reqs; i++)
    {
      socket << data;
      socket >> data2;
    }

    cout << data2.response() << endl;
    cout << data2.response_body() << endl;
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
