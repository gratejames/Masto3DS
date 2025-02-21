#include "download.hpp"

#define SOC_ALIGN       0x1000
#define SOC_BUFFERSIZE  0x100000
static u32 *SOC_buffer = NULL;

size_t WriteStringAppend(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	((std::string *)userp)->append((char*)contents, realsize);
	return realsize;
}

size_t WriteMemory(char *data, size_t size, size_t nmemb, void *clientp)
{
    size_t realsize = size * nmemb;
    struct downloaded *mem = (struct downloaded *)clientp;

    char *ptr = (char *)realloc(mem->response, mem->size + realsize + 1);
    if(!ptr)
        return 0;  /* out of memory! */

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

int setupCACERT() {
	std::cout << "Checking for cacert.pem" << std::endl;
	struct stat st = {0};
	if (stat(pCACertFilePath, &st) == -1) {
		std::cout << "Creating cacert.pem" << std::endl;
		FILE* file = fopen(pCACertFilePath, "w");
		if (file == NULL) {
			return -1;
		}
		fseek(file, 0, SEEK_SET);
		fwrite(cacert_pem, 1, cacert_pem_size, file);
		fclose(file);
	}
	return 0;
}

CURLcode download(std::string url, std::string &fileContents) {
	CURL *curl;
	CURLcode res = CURLE_OK;
	const char* urlc = url.c_str();


	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, urlc);
		curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
		curl_easy_setopt(curl, CURLOPT_CAINFO, pCACertFilePath);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
		curl_easy_setopt(curl, CURLOPT_CA_CACHE_TIMEOUT, 604800L); // cache the CA cert bundle in memory for a week
	    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteStringAppend);
 	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&fileContents);

		/* Perform the request, res gets the return code */
		res = curl_easy_perform(curl);

		/* Check for errors */
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
 
	return res;
}

CURLcode download(std::string url, downloaded &chunk) {
	CURL *curl;
	CURLcode res = CURLE_OK;
	const char* urlc = url.c_str();


	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, urlc);
		curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
		curl_easy_setopt(curl, CURLOPT_CAINFO, pCACertFilePath);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
		curl_easy_setopt(curl, CURLOPT_CA_CACHE_TIMEOUT, 604800L); // cache the CA cert bundle in memory for a week
	    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemory);
 	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

		/* Perform the request, res gets the return code */
		res = curl_easy_perform(curl);

		/* Check for errors */
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
 
	return res;
}

int setupSoc() {
	std::cout << "Socket Setup" << std::endl;
	int ret;
	SOC_buffer = (u32*)memalign(SOC_ALIGN, SOC_BUFFERSIZE);

	if(SOC_buffer == NULL) {
		std::cout << "memalign: failed to allocate" << std::endl;
		return -1;
	}

	// Now intialise soc:u service
	if ((ret = socInit(SOC_buffer, SOC_BUFFERSIZE)) != 0) {
    	printf("socInit: 0x%08X\n", (unsigned int)ret);
		return -1;
	}
	return 0;
}

int setupCurl() {
	std::cout << "Curl Setup" << std::endl;
	setupCACERT();
	setupSoc();
	return 0;
}

void cleanupCurl() {
	curl_global_cleanup();
	socExit();
}
