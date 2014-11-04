//
/// \file responder.cpp
/// \author James Marlowe
/// \brief Implementation of responder functionality
//

#include "responder.h"
#include <vector>
#include <thread>
#include <glog/logging.h>

using namespace std;

const std::string LOG_DIR = "/var/log/server/";

int main(int argc, char* argv[])
{
  for (int i = 1; i < argc; i ++)
  {
    if (std::strcmp(argv[i], "-d") == 0)
      daemonize();
  }
  google::SetLogDestination(google::INFO, LOG_DIR.c_str());
  google::InitGoogleLogging(argv[0]);
  
  google::InstallFailureSignalHandler();
  
  google::InstallFailureWriter(*write_stack_trace_to_log);
  responder();
  return 0;
}

void annotate_request(URLRequest& req)
{
  get_url(req)
  return;
}

void responder ()
{
  zmqcpp::Socket socket(ZMQ_REP);
  zmqcpp::Message mesg;
  URLRequest dat;
  std::string annotated;
  socket.connect(ROUTER);
  while (true)
  {
    socket >> mesg;
    dat.ParseFromString(mesg.last());
    annotate_request(dat);
    CHECK(dat.response() == true) << "Failed to return a response!";
    mesg.clear();
    dat.SerializeToString(& annotated);
    socket << zmqcpp::Message(annotated);
  }
  return;
}

void daemonize()
{
  pid_t pid;
  pid = fork();
  if (pid < 0)
    exit(EXIT_FAILURE);
  if (pid > 0)
    exit(EXIT_SUCCESS);
}

void write_stack_trace_to_log(const char* data, int size)
{
  std::ofstream log_file;
  std::string lfile = LOG_DIR + "stacktrace.log";
  log_file.open(lfile.c_str(), std::ios::out | std::ios::app);
  log_file << data;
  log_file.close();
  
  return;
}
