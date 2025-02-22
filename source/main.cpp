#include <vector>
#include <fstream>
#include <iostream>
#include <list>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <malloc.h>
#include <cacert_pem.h>

#include <sys/stat.h>

#include <3ds.h>
#include <citro2d.h>

#pragma GCC diagnostic push 
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#pragma GCC diagnostic pop

#include <tinyxml2.h>
#include "rapidjson/stringbuffer.h"
#include <curl/curl.h>

#include "schemas.hpp"
#include "ui.hpp"
#include "download.hpp"

using namespace rapidjson;

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

#define ProjectRoot "/3ds/Masto3DS/"

C3D_RenderTarget* top;

Document settingsDoc;
char* settingsBuf;
uiStatus * currentUIstatus;

std::string defaultJSON = "{}";

// C2D_Text title;
// C2D_TextBuf titleBuf  = C2D_TextBufNew(100);
// C2D_Text menu;
// C2D_TextBuf menuBuf = C2D_TextBufNew(4096);

int retCode = 0;


// char* menuString;

// std::string titleText = "JCatcher";

// std::string initialMenuText = \
// 		"Add new podcast URL\n"\
// 		"View Saved Podcasts\n"\
// 		"Credits";

// std::string podcastOptionsText = \
// 		"Check Episodes\n"\
// 		"Edit\n"\
// 		"Remove";

// std::string creditsText = \
// 		"Application: James Smythe\n"\
// 		"Thanks to: TinyXML2, RapidJSON";

// std::vector<std::string> Names;
// std::vector<std::string> URLs;
// std::vector<std::string> EpisodeNames;
// std::vector<std::string> EpisodeURLs;



void printParseError(ParseErrorCode parseError) {
	switch (parseError) {
	case kParseErrorDocumentEmpty:
		std::cout << "Parser error kParseErrorDocumentEmpty" << std::endl;
		break;
	case kParseErrorDocumentRootNotSingular:
		std::cout << "Parser error kParseErrorDocumentRootNotSingular" << std::endl;
		break;
	case kParseErrorValueInvalid:
		std::cout << "Parser error kParseErrorValueInvalid" << std::endl;
		break;
	case kParseErrorObjectMissName:
		std::cout << "Parser error kParseErrorObjectMissName" << std::endl;
		break;
	case kParseErrorObjectMissColon:
		std::cout << "Parser error kParseErrorObjectMissColon" << std::endl;
		break;
	case kParseErrorObjectMissCommaOrCurlyBracket:
		std::cout << "Parser error kParseErrorObjectMissCommaOrCurlyBracket" << std::endl;
		break;
	case kParseErrorArrayMissCommaOrSquareBracket:
		std::cout << "Parser error kParseErrorArrayMissCommaOrSquareBracket" << std::endl;
		break;
	case kParseErrorStringUnicodeEscapeInvalidHex:
		std::cout << "Parser error kParseErrorStringUnicodeEscapeInvalidHex " << std::endl;
		break;
	case kParseErrorStringUnicodeSurrogateInvalid:
		std::cout << "Parser error kParseErrorStringUnicodeSurrogateInvalid" << std::endl;
		break;
	case kParseErrorStringEscapeInvalid:
		std::cout << "Parser error kParseErrorStringEscapeInvalid" << std::endl;
		break;
	case kParseErrorStringMissQuotationMark:
		std::cout << "Parser error kParseErrorStringMissQuotationMark" << std::endl;
		break;
	case kParseErrorStringInvalidEncoding:
		std::cout << "Parser error kParseErrorStringInvalidEncoding" << std::endl;
		break;
	case kParseErrorNumberTooBig:
		std::cout << "Parser error kParseErrorNumberTooBig" << std::endl;
		break;
	case kParseErrorNumberMissFraction:
		std::cout << "Parser error kParseErrorNumberMissFraction" << std::endl;
		break;
	case kParseErrorNumberMissExponent:
		std::cout << "Parser error kParseErrorNumberMissExponent" << std::endl;
		break;
	case kParseErrorTermination:
		std::cout << "Parser error kParseErrorTermination" << std::endl;
		break;
	case kParseErrorUnspecificSyntaxError:
		std::cout << "Parser error kParseErrorUnspecificSyntaxError" << std::endl;
		break;
	default:
		std::cout << "Parser error fell throught to OTHER" << std::endl;
	}
}

int saveFile(std::string fileContents, std::string fileName) {
	struct stat st = {0};
	if (stat(ProjectRoot, &st) == -1) {
		mkdir(ProjectRoot, 0777);
	}
	std::string filePath = ProjectRoot;
	filePath.append(fileName);
	std::cout << "Writing: " << filePath << std::endl;
	std::ofstream myfile;
	myfile.open(filePath, std::fstream::out | std::fstream::trunc);
	if (!myfile.is_open()) {
		return -1;
	}
	myfile << fileContents << std::endl;
	myfile.close();
	return 0;
}

int loadFile(std::string &fileContents, std::string fileName) {
	struct stat st = {0};

	if (stat(ProjectRoot, &st) == -1) {
		mkdir(ProjectRoot, 0777);
	}
	std::string filePath = ProjectRoot;
	filePath.append(fileName);
	FILE* file = fopen(filePath.c_str(), "r");
	if (file == NULL) {
		return -1;
	}
	int length = 0;
	while (!feof(file)) {
		fileContents.append(1, fgetc(file));
		length++;
	}
	length--;
	fileContents = fileContents.substr(0,length);
	return 0;
}

int jsonParse(char* fileBuf, Document &document) {
	document.ParseInsitu(fileBuf);
	if (document.HasParseError()) {
		printParseError(document.GetParseError());
		return -1;
	}
	return 0;
}

void holdForExit() {
	std::cout << "Press start to exit" << std::endl;
	while (aptMainLoop()) {
		gspWaitForVBlank();
		hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) break; // break in order to return to hbmenu
	}
	
	gfxExit();
}

int stringToBuffer(std::string string, char* &buffer) {
	const char* cstr = string.c_str();
	int contentSize = strlen(cstr);
	buffer = (char*)malloc(contentSize+1);
	if (buffer == nullptr) {
		return -1;
	}
	memcpy(buffer, cstr, contentSize);
	memset(buffer+contentSize, 0, 1);
	return 0;
}

void setupGraphics() {
	gfxInitDefault();

	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	
	consoleInit(GFX_BOTTOM,NULL);
	top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
}

void cleanupGraphics() {
	C2D_Fini();
	C3D_Fini();
	gfxExit();
}

void drawUI() {
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, color_background);
	C2D_SceneBegin(top);
	float origin_x = 0;
	float origin_y = 0;
	const float maxWidth = SCREEN_WIDTH - (2*uiPad);
	float width = 0, height = 0;
	currentUIstatus->Draw(origin_x, origin_y, width, height, maxWidth);

	C3D_FrameEnd(0);
}

void writeSettings() {
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	settingsDoc.Accept(writer);
	const char* output = buffer.GetString();
	saveFile(output, "settings.json");
}

int setupSettings() {
	std::string settingsContents;
	retCode = loadFile(settingsContents, "settings.json");
	if (retCode != 0) {
		std::cout << "settings.json not found, creating" << std::endl;
		settingsContents = defaultJSON;
		retCode = saveFile(defaultJSON, "settings.json");
		if (retCode != 0) {
			std::cout << "Failed to write file." << std::endl;
		}
	} else {
		std::cout << "Read settings" << std::endl;
	}
	if (stringToBuffer(settingsContents, settingsBuf) != 0) {
		std::cout << "Failed to make buffer on line " << __LINE__ << std::endl;
		return -1;
	}
	
	retCode = jsonParse(settingsBuf, settingsDoc);
	if (retCode != 0) {
		std::cout << "Corrupted settings JSON: " << retCode << std::endl;
		return -1;
	}
	return 0;
}

void setupDirectory() {
	struct stat st = {0};
	if (stat(ProjectRoot, &st) == -1) {
		mkdir(ProjectRoot, 0777);
	}
}

std::string current_post_id = "";
std::string domain = "https://mastodon.social";
// std::string url = domain + "/api/v1/timelines/public?limit=1";
// std::string url = domain + "/api/v1/timelines/public?limit=1&max_id=114045395536353577";
// std::string url = domain + "/api/v1/timelines/public?limit=1&max_id=114045395361763936";
// std::string url = domain + "/api/v1/timelines/public?limit=1&max_id=114045708160490317";
// std::string url = domain + "/api/v1/timelines/public?limit=1&min_id=114045708571271726"; // THIS LEAVES EMOJIS BLANKS. TODO: GO LOOK 'EM UP!
std::string url = domain + "/api/v1/timelines/public?limit=1&min_id=114045708897701054"; // THIS HAS &QUOT; ENTITYS


int fetchPost() {
	std::cout << "Fetching " << url << std::endl;
	std::string fileContents = "";
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
		if(res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			std::cout << "HTTP" << res << std::endl;
		}

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	char* currentBuf;
	rapidjson::Document currentDoc;
	if(stringToBuffer(fileContents, currentBuf) != 0) {
		std::cout << "Failed to make buffer on line " << __LINE__ << std::endl;
		return -1;
	}
	retCode = jsonParse(currentBuf, currentDoc);
	if (retCode != 0) {
		std::cout << "Received corrupted JSON: " << retCode << std::endl;
		return -1;
	}

	Timeline currentTimeline = Timeline();
	retCode = currentTimeline.Ingest(currentDoc);
	if (retCode != 0) {
		std::cout << "Bad ingest: " << retCode << std::endl;
		return -1;
	}

	free(currentBuf);

	if (currentUIstatus)
		delete currentUIstatus;
	currentUIstatus = new uiStatus {currentTimeline.statuses[0]};
	current_post_id = currentTimeline.statuses[0].id;

	drawUI();
	std::cout << "UI Stack updated" << std::endl;
	return 0;
}

int main() {
	setupGraphics();
	setupDirectory();
	setupCurl();
	if(setupSettings() != 0) {
		std::cout << "Failed to setup settings" << std::endl;
		holdForExit();
		return -1;
	}
	romfsInit();
	load_efonts();

	if (fetchPost() != 0) {
		holdForExit();
		return -1;
	}
	
	// Main loop
	while (aptMainLoop()){
		hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START || kDown & KEY_X)
			break; // break in order to return to hbmenu
		
		if (kDown & KEY_B) {
		}
		if (kDown & KEY_Y) {
			if (fetchPost() !=0) {
				holdForExit();
				return -1;
			}
		}
		if (kDown & KEY_DRIGHT) {
			url = domain + "/api/v1/timelines/public?limit=1&max_id=" + current_post_id;
			if (fetchPost() !=0) {
				holdForExit();
				return -1;
			}
		}
		if (kDown & KEY_DLEFT) {
			url = domain + "/api/v1/timelines/public?limit=1&min_id=" + current_post_id;
			if (fetchPost() !=0) {
				holdForExit();
				return -1;
			}
		}
	}

	// Exit services
	free(settingsBuf);
	cleanupCurl();
	cleanupGraphics();
	return 0;
}

