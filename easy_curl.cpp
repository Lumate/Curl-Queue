//
/// \file easy_curl.cpp
/// \author James Marlowe
/// \brief Implementation of curling functionality
//

#include "easy_curl.h"

std::string response_data;
long response_code;
double response_time;

static size_t data_write(char* buf, size_t size, size_t nmemb, void* userp)
{
    for (int c = 0; c<size*nmemb; c++)
    {
        response_data.push_back(buf[c]);
    }
    return size*nmemb;
}

CURLcode curl_read(const std::string& url, struct curl_slist *headerlist, const char *post_body, long timeout = 3)
{
    CURLcode code(CURLE_FAILED_INIT);
    CURL* curl = curl_easy_init();

    if(curl)
    {
        // set options for the request
        if(CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &data_write))
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist))
        && CURLE_OK == (code = (post_body!=NULL)?curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_body):CURLE_OK)
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str())))
        {
            code = curl_easy_perform(curl);
        }
        // get stats on the response
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &response_time);
        
        curl_easy_cleanup(curl);
    }
    return code;
}

void get_url(URLRequest& req)
{
    curl_global_init(CURL_GLOBAL_ALL);
    req.set_response(false);
    
    struct curl_slist *m_headerlist = NULL;
    const char *post_body = NULL;
    
    if(req.request_headers_size() > 0)
    {
        for (int i = 0; i < req.request_headers_size(); i++)
        {
            m_headerlist = curl_slist_append(m_headerlist, req.request_headers(i).c_str());
        }
    }
    
    if(req.has_request_body())
    {
        post_body = req.request_body().c_str();
    }
    
    if(req.has_request_url())
    {
        if(CURLE_OK == curl_read(req.request_url(), m_headerlist, post_body))
        {
            req.set_response(true);
            req.set_response_time(std::to_string(response_time));
            req.set_response_status(std::to_string(response_code));
            req.set_response_body(response_data);
        }
    }

    curl_slist_free_all(m_headerlist); 
    curl_global_cleanup();
    response_data.clear();
    response_code = 0;
    response_time = 0;
    return;
}
