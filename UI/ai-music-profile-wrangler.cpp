#include "ai-music-profile-wrangler.hpp"
#include <qfile>
#include <QTextStream>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qjsonvalue>

static const char *szInputKey = "input";
static const char *szIsLocalFileKey = "is_local_file";

QString AiMusicProfileWrangler::makeOssiaSceneFile(const QString &sceneFolder,
					     const QString &streamUri)
{
	QFile templateFile(":/res/files/Ossia_Live_Scene_default.json");
	if (!templateFile.open(QIODevice::ReadOnly)) {
		return "";
	}

	QString sceneForThisUri = templateFile.readAll();
	templateFile.close();

	sceneForThisUri.replace("<OSSIA-URI>", streamUri);

	QString thisFilename = "OssiaLive";
	auto colonIdx = streamUri.indexOf(':', 10);	// Need to skip the ":" in "https:"
	auto slashIdx = streamUri.indexOf('/', colonIdx+2);
	if (colonIdx > 0 && slashIdx > 0) {
		QString roomId =
			streamUri.mid(colonIdx + 1, (slashIdx - colonIdx) - 1);
		thisFilename += "-" + roomId;
	}
	thisFilename += ".json";

	const QString saveToPath = sceneFolder + '/' + thisFilename;
	QFile updateFile(saveToPath);
	if (updateFile.open(QIODevice::WriteOnly)) {
		QTextStream stream(&updateFile);
		stream << sceneForThisUri;
	}
	updateFile.close();
	return saveToPath;
}
//	QFile jsonFile(jsonPath);
//	jsonFile.open(QFile::ReadOnly);
//	QByteArray fileContents = jsonFile.readAll();
//	QJsonDocument jsonDocument(QJsonDocument::fromJson(fileContents));
//	jsonFile.close();
//
//	if (jsonDocument.isNull() || !jsonDocument.isObject()) {
//		return false;
//	}
//
//	QJsonObject topLevel = jsonDocument.object();
//	if (topLevel.isEmpty()) {
//		return false;
//	}
//
//	QJsonValueRef refSources = topLevel["sources"];
//	QJsonArray sources = refSources.toArray();
//	if (sources.count() < 1) {
//		return false;
//	}
//
//	QJsonValueRef refSource0 = sources[0];
//	QJsonObject source0 = refSource0.toObject();
//
//	QJsonValueRef settingsVal = source0["settings"];
//	if (settingsVal.isNull()) {
//		return false;
//	}
//
//	QJsonObject settingsObj = settingsVal.toObject();
//	QJsonValueRef inputVal = settingsObj[szInputKey];
//	QString currentInput = inputVal.toString();
//	if (currentInput == streamUri) {
//		return false;	
//	}
//
//	// QJson won't simply recreate the JSON stream from the top. So lets just
//	// load the file in as a stream and do a replace of the stream URL.
//
//	//settingsObj.remove(szInputKey);
//	//settingsObj.remove(szIsLocalFileKey);
//
//	//QJsonValue jsonNewUrl(streamUri);
//	//QJsonValue jsonFalse(false);
//	//settingsObj.insert(szInputKey, jsonNewUrl);
//	//settingsObj.insert(szIsLocalFileKey, jsonFalse);
//
//	QJsonDocument newDoc(topLevel);
//	auto json = newDoc.toJson(QJsonDocument::Compact);
//
//	QString strFileContents =
//		QString::fromStdString(fileContents.toStdString());
//
//	strFileContents.replace(currentInput, streamUri);
//
//	QFile updateFile(jsonPath);
//	if (updateFile.open(QIODevice::WriteOnly)) {
//		QTextStream stream(&updateFile);
//		stream << strFileContents;
//	}
//	updateFile.close();
//	return true;
//}
