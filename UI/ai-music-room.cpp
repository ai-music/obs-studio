#include <util/curl/curl-helper.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include "obs-app.hpp"
#include "ai-music-room.hpp"

static auto curl_deleter = [](CURL *curl) { curl_easy_cleanup(curl); };
using Curl = std::unique_ptr<CURL, decltype(curl_deleter)>;

void AiMusicRoom::doStep1_createApplication() {
	//POST https : //postman-echo.com/post
	//		     Accept : */*
	//Cache-Control: no-cache
	//Content-Type: application/json
	//{
	// "appName": "Ossialive"
	//}

	const QString url = "https://postman-echo.com/post/";
	const QString authorization = "";
	const QString contentType = "application/json";
	QJsonObject json;
	json["appName"] = "Ossialive";
	QJsonDocument doc(json);
	const QString content = doc.toJson(QJsonDocument::Compact);
	doHttp(Step1, url, authorization, content);
}

void AiMusicRoom::doStep2_authenticateApplication() {
	//POST https : //postman-echo.com/post
	//		     Accept : */*
	//Cache-Control: no-cache
	//Content-Type: application/json
	//{ 
	// "apiKey": "apiKey goes here from application create",
	// "apiSecret": "apiSecret goes here from application create"

	const QString url = "https://postman-echo.com/post/";
	const QString authorization = "";
	const QString contentType = "application/json";
	QJsonObject json;
	json["apiKey"] = apiKey;
	json["apiSecret"] = apiSecret;
	QJsonDocument doc(json);
	const QString content = doc.toJson(QJsonDocument::Compact);
	doHttp(Step2, url,  authorization, content);
}

void AiMusicRoom::doStep3_createRoomStartPlayback(const QString &style)
{
	//POST https : //postman-echo.com/post
	//		     Accept : */*
	//Cache-Control: no-cache
	//Authorization: Bearer {{activeApplicationToken}}
	//Content-Type: application/json
	//{
	// "musicStyle": ["JAZZ"]
	//}

	const QString url = "https://postman-echo.com/post/";
	const QString authorization = "Bearer " + activeApplicationToken;
	const QString contentType = "application/json";
	QJsonObject json;
	QJsonArray styleArray = {style};
	json["musicStyle"] = styleArray;
	QJsonDocument doc(json);
	const QString content = doc.toJson(QJsonDocument::Compact);
	doHttp(Step3, url, authorization, content);
}

void AiMusicRoom::doStep4_updateRoomChangeStyle(const QString& style) {
	//PUT https : //postman-echo.com/put/{{activeId}}
	//		    Accept : */*
	//Cache-Control: no-cache
	//Authorization: Bearer {{activeApplicationToken}}
	//Content-Type: application/json
	//{
	// "id": "roomID",
	// "state": "ACTIVE",
	// "createdAt": "25 December 2009",
	// "streamUri": "www.shouldntupdate.com",
	// "ownerId": "9153879a-3e6c-42de-8841-87fc9d98ea2c",
	// "musicStyle": ["HOUSE"]
	//}
	
	const QString url = "https://postman-echo.com/post/" + roomId;
	const QString authorization = "Bearer " + activeApplicationToken;
	const QString contentType = "application/json";
	QJsonObject json;
	json["id"] = roomId;
	json["state"] = "ACTIVE";
	json["createdAt"] = "25 December 2009";	// hardcode
	json["streamUri"] = "www.shouldntupdate.com"; // hardcode
	json["ownerId"] = "9153879a-3e6c-42de-8841-87fc9d98ea2c"; // hardcode
	QJsonArray styleArray = {style};
	json["musicStyle"] = styleArray;
	QJsonDocument doc(json);
	const QString content = doc.toJson(QJsonDocument::Compact);
	doHttp(Step4, url, authorization, content);
}

void AiMusicRoom::doStep5_deleteStream() {
	//DELETE https : //postman-echo.com/delete/{{activeId}}
	//		       Accept : */*
	//Cache-Control: no-cache
	//Authorization: Bearer {{activeApplicationToken}}

	const QString url = "https://postman-echo.com/delete/" + roomId;
	const QString authorization = "Bearer " + activeApplicationToken;
	const QString content = "";
	doHttp(Step5, url, authorization, content);
}

AiMusicRoom *AiMusicRoom::theInstance = nullptr; 

AiMusicRoom::AiMusicRoom() : QObject(),
	inFlightRequest(NoRequest)
{
	
	theInstance = this;
}

size_t AiMusicRoom::staticStringWrite(char *ptr, size_t size,
					     size_t nmemb, std::string &str)
{
	return theInstance ? theInstance->stringWrite(ptr, size, nmemb, str)
			   : 0;
}

size_t AiMusicRoom::stringWrite(char *ptr, size_t size, size_t nmemb,
					 std::string &str)
 {
	 size_t total = size * nmemb;
	 if (total)
		 str.append(ptr, total);
	 
	 // Decode the response based on the request that was in-flight
	 switch (inFlightRequest) {
		 case Step1:	// Create Application - returns apiKey and apiSecret
			apiKey = "123";	// Hard Coded until it's implemented
			apiSecret = "456";
			inFlightRequest = NoRequest;
			doStep2_authenticateApplication();
			break;
		case Step2:	// Authenticate - returns activeApplicationTone
			activeApplicationToken = "123";
			inFlightRequest = NoRequest;
			doStep3_createRoomStartPlayback("");
			break;
		case Step3:	// Create a room - returns RoomId
			roomId = "1";
			inFlightRequest = NoRequest;
			streamUrl = "https://urbanlive.aimusic.services:30510/hls/show.m3u8";
			// "https://stream.radioparadise.com/rock-320"; // Example until we get the real thing
			emit signalGotRoom(roomId,streamUrl);
			break;
		case Step4:	// Update a Room
			inFlightRequest = NoRequest;
			break;
		case Step5:	// Delete the room
			inFlightRequest = NoRequest;
			break;
		case NoRequest:
			break;
	 }

	 return total;
 }

bool AiMusicRoom::doHttp(InFlightRequest thisRequest, const QString &url,
	const QString authorization,
	const QString content)
 {
	if (inFlightRequest != NoRequest) {
		return false;
	}

	inFlightRequest = thisRequest;
	Curl curl{curl_easy_init(), curl_deleter};
	if (curl) {
		struct curl_slist *header = nullptr;
		std::string str;

		std::string versionString("User-Agent: OssiaStudio");
		versionString += App()->GetVersionString();

		header = curl_slist_append(header, versionString.c_str());
		header = curl_slist_append(header, "Accept: */*");
		header = curl_slist_append(header, "Cache - Control : no - cache");
		if (!content.isEmpty()) {
			header = curl_slist_append(header, "Content-Type: application/json");
		}
		if (!authorization.isEmpty()) {
			header = curl_slist_append(
				header, ("Authorization: " + authorization).toStdString().data());
		}

		curl_easy_setopt(curl.get(), CURLOPT_URL, url.toStdString().data());
		curl_easy_setopt(curl.get(), CURLOPT_ACCEPT_ENCODING, "");
		curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, header);
		if (!content.isEmpty()) {
			curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS,
					 content.toStdString().data());
		}

		char curlErrorBuffer[CURL_ERROR_SIZE];
		curl_easy_setopt(curl.get(), CURLOPT_ERRORBUFFER,
				 curlErrorBuffer);
		curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION,
				 staticStringWrite);
		curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &str);
		curl_obs_set_revoke_setting(curl.get());

		const int timeoutSec = 30;
		if (timeoutSec)
			curl_easy_setopt(curl.get(), CURLOPT_TIMEOUT,
					 timeoutSec);

#if LIBCURL_VERSION_NUM >= 0x072400
		// A lot of servers don't yet support ALPN
		curl_easy_setopt(curl.get(), CURLOPT_SSL_ENABLE_ALPN, 0);
#endif
		auto code = curl_easy_perform(curl.get());

		if (code != CURLE_OK) {
			// Error
			curlErrors.append(curlErrorBuffer);
			inFlightRequest = NoRequest;	
		} else {
			if (inFlightRequest == Step4 ||	// Step4 and Step5 don't return a 
			    inFlightRequest == Step5) {	// body - so we need to change it here.
				inFlightRequest = NoRequest;	
			}
			// OK
		}
		curl_slist_free_all(header);
	}
	
	return true;
 }

