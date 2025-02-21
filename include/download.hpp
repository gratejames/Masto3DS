#pragma once
#include <fstream>
#include <iostream>
#include <inttypes.h>
#include <malloc.h>
#include <sys/stat.h>

#include <citro2d.h>
#include <curl/curl.h>
#include <cacert_pem.h>

#include "downloadedstruct.hpp"

#define pCACertFilePath "/3ds/Masto3DS/" "cacert.pem"

int setupCurl();
void cleanupCurl();
CURLcode download(std::string url, std::string &fileContents);
CURLcode download(std::string url, downloaded &chunk);
size_t WriteStringAppend(void *contents, size_t size, size_t nmemb, void *userp);
size_t WriteMemory(char *data, size_t size, size_t nmemb, void *clientp);