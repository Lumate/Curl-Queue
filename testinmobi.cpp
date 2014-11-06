//
/// \file testinmobi.cpp
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
    data.set_request_url("http://api.w.inmobi.com/showad/v2");
    data.add_request_headers("Content-Type: application/json");
    data.add_request_headers("X-Forwarded-For: 71.85.235.42");
    data.add_request_headers("x-device-user-agent: Mozilla");
    data.set_request_body("{\
            \"responseformat\": \"html\",\
            \"imp\": [\
                {\
                    \"ads\": \"1\",\
                    \"displaymanager\": \"s_mediatorname\",\
                    \"adtype\": \"ban\",\
                    \"displaymanagerver\": \"1.2.0\",\
                    \"banner\": {\
                        \"adsize\": \"15\"\
                    }\
                }\
            ],\
            \"site\": {\
                \"id\": \"af4208c4b5a3445daeac863fc8c9e51d\"\
            },\
            \"device\": {\
                \"adt\": \"\",\
                \"ip\": \"\",\
                \"ua\": \"\",\
                \"locale\": \"\",\
                \"geo\": {\
                    \"lat\": \"\",\
                    \"lon\": \"\",\
                    \"accu\": \"1\"\
                }\
            },\
            \"user\": {\
                \"data\": [\
                    {\
                        \"id\": \"\"\
                    }\
                ]\
            }\
        }");
    
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
