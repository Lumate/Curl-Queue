//
/// \file easy_curl.cpp
/// \author James Marlowe
/// \brief Implementation of curling functionality
//

#include "easy_curl.h"

string data;

static size_t data_write(void* buf, size_t size, size_t nmemb, void* userp)
{
    for (int c = 0; c<size*nmemb; c++)
    {
        data.push_back(buf[c]);
    }
    return size*nmemb;
}

CURLcode curl_read(const std::string& url, long timeout = 3)
{
	CURLcode code(CURLE_FAILED_INIT);
	CURL* curl = curl_easy_init();

	if(curl)
	{
		if(CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &data_write))
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str())))
		{
			code = curl_easy_perform(curl);
		}
		curl_easy_cleanup(curl);
	}
	return code;
}

bool get_url(URLRequest& req)
{
	curl_global_init(CURL_GLOBAL_ALL);
    
    if(req.has_request_url() == true)
    {
    	if(CURLE_OK == curl_read(req.request_url()))
	    {
            req.set_response(true)
            req.set_response_body(data)
        }
        else
        {
            req.set_response(false)
        }
    }

	curl_global_cleanup();
}