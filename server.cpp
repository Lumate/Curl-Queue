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
const char LISTEN_ADDR[] = "tcp://*:5555";
const char WORKER_ADDR[] = "tcp://*:5556";

/*!
 * \brief launches the program as a daemonize
 * \pre None
 * \post forks a process, and kills the current process
 */
void daemonize();

void blank(zmq::context_t*);

/*!
 * \brief Custom writer function for glog stack traces
 * \pre None
 * \post Message written to log file
 */
void write_stack_trace_to_log(const char* data, int size);

void launch_proxy(const std::string & in_port, const std::string & out_port,
                  const SocketPairType sockpair, zmq::context_t* ctx);

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
  std::vector<std::thread> workers;
  proxy_thread = std::unique_ptr<std::thread> (new std::thread (launch_proxy, LISTEN_ADDR, WORKER_ADDR, &context));
  for (int i=0; i<numthreads; i++)
    workers.push_back(std::thread(blank, &context));
  proxy_thread->join();
  
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

void launch_proxy(const std::string & in_port, const std::string & out_port,
                  const SocketPairType sockpair, zmq::context_t* ctx)
{
  zmq::socket_t clients(*ctx, ZMQ_ROUTER);
  zmq::socket_t workers(*ctx, ZMQ_DEALER);
  clients.bind(in_port.c_str());
  workers.bind(out_port.c_str());
  zmq_proxy(clients, workers, nullptr);
  zmq_close(clients);
  zmq_close(workers);
  return;
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
