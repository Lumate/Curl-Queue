//
/// \file easy_curl.h
/// \author James Marlowe
/// \brief Functions related to curling things
//

#include <curl/curl.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "url_request.pb.h"

//
/// \brief libcurl callback for CURLOPT_WRITEFUNCTION
/// \param buf: a pointer to the data that curl has for us
/// \param size*nmemb is the size of the buffer
/// \return length of written data
//
static size_t data_write(char* buf, size_t size, size_t nmemb, void* userp);

//
/// \brief libcurl callback for CURLOPT_WRITEFUNCTION
/// \param url: the url of the resource to request
/// \param timeout: how long to wait for the resource in seconds
/// \return curl status code
//
CURLcode curl_read(const std::string& url, struct curl_slist *headerlist, long timeout);

//
/// \brief curls a url and gets response
/// \param req: protobuf containing request/response
/// \return bool success
//
void get_url(URLRequest& req);
