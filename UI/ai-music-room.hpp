

#pragma once

#include <string>
#include <Qstring>
#include <QStringList>
class AiMusicRoom : public QObject{
Q_OBJECT

public:
	void doStep1_createApplication();
	void doStep2_authenticateApplication();
	void doStep3_createRoomStartPlayback(const QString &style);
	void doStep4_updateRoomChangeStyle(const QString &style);
	void doStep5_deleteStream();

	AiMusicRoom();

	int getCurrentStep() { return currentStep; }
	QString getStreamUrl() { return streamUrl; }
signals:
	void signalGotRoom(const QString& roomId, const QString& streamUrl);
private:
	enum InFlightRequest { NoRequest, Step1, Step2, Step3, Step4, Step5 };

	int currentStep;
	QString apiKey;
	QString apiSecret;
	QString activeApplicationToken;
	QString roomId;
	InFlightRequest inFlightRequest;
	QStringList curlErrors;
	QString streamUrl;

	static size_t staticStringWrite(char *ptr, size_t size, size_t nmemb,
					std::string &str);

	size_t stringWrite(char *ptr, size_t size, size_t nmemb,
				  std::string &str);

	bool doHttp(InFlightRequest inFlightRequest, const QString &url,
			   const QString authorization,
			   const QString content);

	static AiMusicRoom *theInstance;
};
