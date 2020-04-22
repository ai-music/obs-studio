

#pragma once

#include <string>
#include <Qstring>
#include <QStringList>
class AiMusicRoom {
public:
	static void doStep1_createApplication();
	static void doStep2_authenticateApplication();
	static void doStep3_createRoomStartPlayback(const QString &style);
	static void doStep4_updateRoomChangeStyle(const QString &style);
	static void doStep5_deleteStream();

	AiMusicRoom();

	static int getCurrentStep() { return currentStep; }
private:
	enum InFlightRequest { NoRequest, Step1, Step2, Step3, Step4, Step5 };

	static int currentStep;
	static QString apiKey;
	static QString apiSecret;
	static QString activeApplicationToken;
	static QString roomId;
	static InFlightRequest inFlightRequest;
	static QStringList curlErrors;

	static size_t string_write(char *ptr, size_t size,
						size_t nmemb, std::string &str);
	static bool doHttp(InFlightRequest inFlightRequest, const QString &url,
			   const QString authorization,
			   const QString content);
	static void httpResponse();
};
