/*!
 * \author Nathan ELoe
 * \brief proxy server copied from idificator for curl-queue
 */

#include "mt-proxy/mt-proxy.h"
#include <cstring>
#include <zmq.hpp>
#include <unistd.h>
#include <fstream>
#include <glog/logging.h>
#include <iostream>

const std::string LOG_DIR = "/var/log/server/";
const char LISTEN_ADDR[] = "tcp://*:5555";
const char WORKER_ADDR[] = "tcp://*:5556";

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

void do_nothing(zmq::context_t*){}

int main(int argc, char* argv[])
{
  int numthreads = 0;
  for (int i = 1; i < argc; i ++)
  {
    if (std::strcmp(argv[i], "-d") == 0)
      daemonize();
  }

  google::SetLogDestination(google::INFO, LOG_DIR.c_str());
  google::InitGoogleLogging(argv[0]);
  
  google::InstallFailureSignalHandler();
  
  google::InstallFailureWriter(*write_stack_trace_to_log);
  
  LOG(INFO) << "Initializing server with " << numthreads << " threads" << std::endl;

  zmq::context_t context(1);
  mtproxy::MTProxy<do_nothing> proxy(numthreads, mtproxy::REQ_REP, LISTEN_ADDR, WORKER_ADDR, &context);
  proxy.start();
  proxy.join();
  return 0;
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
  std::string file = LOG_DIR + "stacktrace.log";
  log_file.open(file.c_str(), std::ios::out | std::ios::app);
  log_file << data;
  log_file.close();
  
  return;
}
