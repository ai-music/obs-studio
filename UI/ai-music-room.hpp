

#pragma once

#include <string>
#include <Qstring>
#include <QStringList>
class AiMusicRoom : public QObject{
Q_OBJECT

public:
	AiMusicRoom();
	~AiMusicRoom();

	void preDelete(); // Call preDelete when you know the application is closing
			  // down - becuase its better to do the clean-up outside of
			  // the destructor.
	bool doConnect();
	bool doDisconnect();
	bool doChangeMusicStyle(const QString &musicStyle);

	QString getStreamUrl() { return streamUri; }
signals:
	void signalGotRoom(const QString& roomId, const QString& style,
		const QString& streamUri);
	void signalChangedRoomStyle(const QString &roomId, const QString &style,
				const QString &streamUri);
	void signalDeletedRoom(const QString &roomId, const QString &style,
			       const QString &streamUri);

	void signal_failed_Step1_createApplication();
	void signal_failed_Step2_authenticateApplication();
	void signal_failed_Step3_createRoomStartPlayback();
	void signal_failed_Step4_updateRoomChangeStyle();
	void signal_failed_Step5_deleteRoom();

private:
	void doStep1_createApplication();
	void doStep2_authenticateApplication();
	void doStep3_createRoomStartPlayback(const QString &style);
	void doStep4_updateRoomChangeStyle(const QString &style);
	void doStep5_deleteRoom();

	explicit AiMusicRoom(const AiMusicRoom &);
	enum InFlightRequest { NoRequest, Step1, Step2, Step3, Step4, Step5 };

	int permission;
	QString appName;
	QString apiKey;
	QString apiSecret;
	QString createdAt;
	QString id;
	QString activeApplicationToken;
	QString ownerId;
	QString role;
	QString state;
	InFlightRequest inFlightRequest;
	QStringList curlErrors;
	QString streamUri;
	QString musicStyle;

	static size_t staticStringWrite(char *ptr, size_t size, size_t nmemb,
					std::string &str);

	size_t stringWrite(char *ptr, size_t size, size_t nmemb,
				  std::string &str);

	void decodeRoomJson(const QJsonObject &jsonDataObject);

	bool doHttp(InFlightRequest inFlightRequest, const QString& url,
		           const QString& verb,
			   const QString& authorization,
			   const QString& content);

	static AiMusicRoom *theInstance;
};
