#include <util/curl/curl-helper.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include "obs-app.hpp"
#include "ai-music-room.hpp"

static auto curl_deleter = [](CURL *curl) { curl_easy_cleanup(curl); };
using Curl = std::unique_ptr<CURL, decltype(curl_deleter)>;

const QString putVerb = "PUT";
const QString getVerb = "GET";
const QString postVerb = "POST";
const QString deleteVerb = "DELETE";

const QString stateDeleted = "DELETED";
const QString statePending = "PENDING";
const QString stateActive = "ACTIVE";

AiMusicRoom *AiMusicRoom::theInstance = nullptr;

AiMusicRoom::AiMusicRoom()
	: QObject(),
	  permission(-1),
	  inFlightRequest(NoRequest),
	  musicStyle("AMBIENT"),
	  state(stateDeleted)
{
	theInstance = this;
}

AiMusicRoom::~AiMusicRoom()
{
	preDelete();	// Called here, just in case.
}

void AiMusicRoom::preDelete() {
	doDisconnect();
}

bool AiMusicRoom::doConnect() {
	if (inFlightRequest != NoRequest) {
		return false;
	}

	if (state == stateActive) {
		return true;
	}

	if (state == stateDeleted) {
		doStep1_createApplication();
		return true;
	}

	// Pending State - so either we need to do step2 or step3.
	if (activeApplicationToken.isEmpty()) {
		doStep2_authenticateApplication();
		return true;
	}

	doStep3_createRoomStartPlayback(musicStyle);
	return true;
}

bool AiMusicRoom::doDisconnect() {
	doStep5_deleteRoom();
	return true;
}

bool AiMusicRoom::doChangeMusicStyle(const QString& musicStyle) {
	if (state != stateActive) {
		return false;
	}

	doStep4_updateRoomChangeStyle(musicStyle);
	return true;
}


void AiMusicRoom::doStep1_createApplication() {
	//POST https : //urban-api.aimusic.services/applications
	//Accept: */*
	//Cache-Control: no-cache
	//Content-Type: application/json
	//{
	//	"appName": "ossialive"
	//}

	const QString url = "https://urban-api.aimusic.services/applications";
	const QString authorization = "";
	const QString contentType = "application/json";
	QJsonObject json;
	json["appName"] = "ossialive";
	QJsonDocument doc(json);
	const QString content = doc.toJson(QJsonDocument::Compact);
	doHttp(Step1, url, postVerb, authorization, content);
}

void AiMusicRoom::doStep2_authenticateApplication() {
	// POST https://urban-api.aimusic.services/auth
	// Accept: */*
	// Cache-Control: no-cache
	// Content-Type: application/json
	// {
	//	"apiKey": "apiKey goes here from application create",
	//	"apiSecret": "apiSecret goes here from application create"
	// }

	if (apiSecret.isEmpty()) {
		return;
	}

	const QString url = "https://urban-api.aimusic.services/auth";
	const QString authorization = "";
	const QString contentType = "application/json";
	QJsonObject json;
	json["apiKey"] = apiKey;
	json["apiSecret"] = apiSecret;
	QJsonDocument doc(json);
	const QString content = doc.toJson(QJsonDocument::Compact);
	doHttp(Step2, url, postVerb, authorization, content);
}

void AiMusicRoom::doStep3_createRoomStartPlayback(const QString &style)
{
	// POST https : //urban-api.aimusic.services/rooms
	//  Accept : */*
	// Cache-Control: no-cache
	// Authorization: Bearer {{activeApplicationToken}}
	// Content-Type: application/json
	// {
	//	"musicStyle": ["JAZZ"]
	// }

	if (activeApplicationToken.isEmpty()) {
		return;
	}

	const QString url = "https://urban-api.aimusic.services/rooms";
	const QString authorization = "Bearer " + activeApplicationToken;
	const QString contentType = "application/json";
	QJsonObject json;
	QJsonArray styleArray = {style.toUpper()};
	json["musicStyle"] = styleArray;
	QJsonDocument doc(json);
	const QString content = doc.toJson(QJsonDocument::Compact);
	doHttp(Step3, url, postVerb, authorization, content);
}

void AiMusicRoom::doStep4_updateRoomChangeStyle(const QString& style) {
	//PUT https : https://urban-api.aimusic.services/rooms/{{activeId}}
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

	if (state != stateActive) {
		return;
	}

	const QString url = "https://urban-api.aimusic.services/rooms/" + id;
	const QString authorization = "Bearer " + activeApplicationToken;
	const QString contentType = "application/json";
	QJsonObject json;
	json["id"] = id;
	json["state"] = state;
	json["createdAt"] = createdAt;
	json["streamUri"] = streamUri;
	json["ownerId"] = ownerId;
	QJsonArray styleArray = { style.toUpper() };
	json["musicStyle"] = styleArray;
	QJsonDocument doc(json);
	const QString content = doc.toJson(QJsonDocument::Compact);
	doHttp(Step4, url, putVerb, authorization, content);
}

void AiMusicRoom::doStep5_deleteRoom() {
	//DELETE https : //postman-echo.com/delete/{{activeId}}
	//		       Accept : */*
	//Cache-Control: no-cache
	//Authorization: Bearer {{activeApplicationToken}}

	if (state == stateDeleted) {
		return;
	}

	const QString url = "https://urban-api.aimusic.services/rooms/" + id;
	const QString authorization = "Bearer " + activeApplicationToken;
	const QString content = "";
	doHttp(Step5, url, deleteVerb, authorization, content);
}

size_t AiMusicRoom::staticStringWrite(char *ptr,
				      size_t size,
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

	 const QByteArray response(QByteArray::fromStdString(str));
	 QJsonDocument jsonDocument(QJsonDocument::fromJson(response));
	 QJsonObject jsonObject = jsonDocument.object();
	 bool isValid = jsonObject["isValid"].toBool();
	 QJsonObject jsonDataObject = jsonObject["data"].toObject();

	 // Decode the response based on the request that was in-flight
	 switch (inFlightRequest) {
		 case Step1:	// Create Application - returns apiKey and apiSecret
			 inFlightRequest = NoRequest;
			 if (isValid) {
				 permission = jsonDataObject["permission"].toInt();
				 appName = jsonDataObject["appName"].toString();
				 apiKey = jsonDataObject["apiKey"].toString();
				 apiSecret = jsonDataObject["apiSecret"].toString();
				 role = jsonDataObject["role"].toString();
				 state = jsonDataObject["state"].toString();
				 createdAt = jsonDataObject["createdAt"].toString();
				 id = jsonDataObject["id"].toString();
			 } else {
				 emit signal_failed_Step1_createApplication();
			 }
			 // Hard Coded until it's implemented
			 apiKey = "eec1be1df1d625d7be20f0eb"; 
			 apiSecret =
				 "230a3ace65d0213aca35a173c8998ebaee8a7264683e96f77fe8c2c50140511c";
			 state = statePending;
			 doStep2_authenticateApplication();
		break;
		case Step2:	// Authenticate - returns activeApplicationTone
			 inFlightRequest = NoRequest;
			 if (isValid) {
				 activeApplicationToken =
					 jsonDataObject["token"].toString();
				 doStep3_createRoomStartPlayback( musicStyle);
			 } else {
				 emit signal_failed_Step2_authenticateApplication();
			 }
		break;
		case Step3:	// Create a room - returns RoomId
			inFlightRequest = NoRequest;
			if (isValid) {
				decodeRoomJson(jsonDataObject);
				//streamUri =
				//	 "https://urbanlive.aimusic.services:30510/hls/show.m3u8";
				// "https://stream.radioparadise.com/rock-320"; // Example until we get the real thing
				emit signalGotRoom(id, musicStyle, streamUri);
			} else {
				emit signal_failed_Step3_createRoomStartPlayback();
			}
		break;
		case Step4:	// Update a Room
			 inFlightRequest = NoRequest;
			 if(isValid)
			 {
				 decodeRoomJson(jsonDataObject);
				 emit signalChangedRoomStyle(id, musicStyle, streamUri);
			 } else {
				 emit signal_failed_Step4_updateRoomChangeStyle();
			 }

		break;
		case Step5:	// Delete the room
			inFlightRequest = NoRequest;
			if (isValid) {
				decodeRoomJson(jsonDataObject);
				apiSecret.clear();
				activeApplicationToken.clear();
				emit signalDeletedRoom(id, musicStyle, streamUri);
			} else {
				emit signal_failed_Step5_deleteRoom();
			}
			break;
		case NoRequest:
			break;
	 }

	 return total;
}
 void AiMusicRoom::decodeRoomJson(const QJsonObject &jsonDataObject) {
	 QJsonArray musicStyleArray = jsonDataObject["musicStyle"].toArray();
	 musicStyle = (musicStyleArray.count() > 0)
			      ? musicStyleArray.at(0).toString()
			      : "";
	 ownerId = jsonDataObject["ownerId"].toString();
	 streamUri = jsonDataObject["streamUri"].toString();
	 state = jsonDataObject["state"].toString();
	 createdAt = jsonDataObject["createdAt"].toString();
	 id = jsonDataObject["id"].toString();
 }

bool AiMusicRoom::doHttp(InFlightRequest thisRequest, const QString &url,
	const QString& verb,
	const QString& authorization,
	const QString& content)
{

	if (inFlightRequest != NoRequest) {
		return false;
	}

	inFlightRequest = thisRequest;
	Curl curl{curl_easy_init(), curl_deleter};
	if (curl) {
		char* postDataBuffer = new char[1024];

		memset(postDataBuffer, 0, 1024);	
	
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
#ifdef _DEBUG
		curl_easy_setopt(curl.get(), CURLOPT_PROXY, "127.0.0.1:8888");	// So that Fiddler can see the request.
#endif
		curl_easy_setopt(curl.get(), CURLOPT_CUSTOMREQUEST, verb.toStdString().data());
		curl_easy_setopt(curl.get(), CURLOPT_URL, url.toStdString().data());
		curl_easy_setopt(curl.get(), CURLOPT_ACCEPT_ENCODING, "");
		curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, header);
		if (!content.isEmpty()) {
			QByteArray byteArray = content.toLocal8Bit();
			const char *data = byteArray.data();
			long len = static_cast<long>( strlen(data) );
			strncpy(postDataBuffer, data, len);
			curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS,
					 postDataBuffer);
					 //"{\"appName\":\"ossialive\"}");
			curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDSIZE, len);
		}

		char curlErrorBuffer[CURL_ERROR_SIZE];
		curl_easy_setopt(curl.get(), CURLOPT_ERRORBUFFER,
				 curlErrorBuffer);
		theInstance = this;
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
			switch (inFlightRequest) {
			case Step1:
				emit signal_failed_Step1_createApplication();
				break;
			case Step2:
				emit
				signal_failed_Step2_authenticateApplication();
				break;
			case Step3:
				emit
				signal_failed_Step3_createRoomStartPlayback();
				break;
			case Step4:
				emit signal_failed_Step4_updateRoomChangeStyle();
				break;
			case Step5:
				emit signal_failed_Step5_deleteRoom();
				break;
			}

			inFlightRequest = NoRequest;	
		} else {
			if (inFlightRequest == Step4 ||	// Step4 and Step5 don't return a 
			    inFlightRequest == Step5) {	// body - so we need to change it here.
				inFlightRequest = NoRequest;	
			}
			// OK
		}
		curl_slist_free_all(header);
		delete postDataBuffer;
	}
	
	return true;
 }

