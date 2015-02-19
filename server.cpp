/*!
 * \author Nathan ELoe
 * \brief proxy server copied from idificator for curl-queue
 */

#include <cstring>
#include <zmq.hpp>
#include <unistd.h>
#include <fstream>
#include <glog/logging.h>
#include <iostream>

const std::string LOG_DIR = "/var/log/server/";

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

int main(int argc, char* argv[])
{
  std::string base = "tcp://*:";
  if (argc < 3)
  {
    std::cerr << "Usage: " << argv[0] << " client_port(example: 5555) worker_port(example:5556) [-d]" << std::endl;
    exit (1);
  }
  for (int i = 1; i < argc; i ++)
  {
    if (std::strcmp(argv[i], "-d") == 0)
      daemonize();
  }

  google::SetLogDestination(google::INFO, LOG_DIR.c_str());
  google::InitGoogleLogging(argv[0]);
  
  google::InstallFailureSignalHandler();
  
  google::InstallFailureWriter(*write_stack_trace_to_log);
  
  LOG(INFO) << "Initializing server" << std::endl;
  
  zmq::context_t context(1);
  zmq::socket_t clients(context, ZMQ_ROUTER);
  zmq::socket_t workers(context, ZMQ_DEALER);
  clients.bind((base + argv[1]).c_str());
  workers.bind((base + argv[2]).c_str());
  zmq_proxy(clients, workers, nullptr);
  zmq_close(clients);
  zmq_close(workers);
  
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
