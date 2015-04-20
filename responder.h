//
/// \file responder.h
/// \author James Marlowe
/// \brief Functions related to responding to a request made to Curl-Queue
//

#include <iostream>
#include <set>
#include <string>
#include <thread>
#include <sstream>
#include "zmq/zmqcpp.h"
#include "easy_curl.h"
#include "url_request.pb.h"

#include <fstream>

//
/// \brief Takes a request, and curls external resource for it
/// \pre TODO: figure out pres
/// \post Sends annotated data over ZeroMQ socket to original requester
void responder (const char *ROUTER);

//
/// \brief Annotates the request data
/// \pre TODO: figure out pres 
/// \post populates the request with data from curl
//
void annotate_request(URLRequest& req);

/*!
 * \brief launches the program as a daemonize
 * \pre None
 * \post forks a process, and kills the current process
 */
void daemonize();

/*!
 * \brief Custom writer function for glog stack traces
 * \pre None
 * \post Message written to log file
 */
void write_stack_trace_to_log(const char* data, int size);

//
/// \brief converts type T to a std::string
/// \pre stringstream << T must be defined
/// \post None
/// \return a string representation of t
//
template <class T>
std::string tostr(const T& t)
{
  std::stringstream ss;
  ss << t;
  return ss.str();
}
