#include "ai-music-profile-wrangler.hpp"
#include <qfile>
#include <QTextStream>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qjsonvalue>

static const char *szInputKey = "input";
static const char *szIsLocalFileKey = "is_local_file";

QString AiMusicProfileWrangler::replaceAiMusicStream(const QString &sceneFolder,
						  QString &sceneFilePath,
						  const QString &streamUri)
{
	// In the source name "Ossia Music", change the settings
	QFile currentScene(sceneFilePath);
	auto opened = currentScene.open( QFile::ReadWrite);
	if (!opened) {
		return "";
	}

	const QString fileContents(currentScene.readAll());
	currentScene.close();

	const auto aiMusicIndex = fileContents.indexOf("aimusic.services");
	if (aiMusicIndex <= 0) {
		return "";
	}

	const auto quoteAfter = fileContents.indexOf('"', aiMusicIndex);
	const auto quoteBefore = fileContents.lastIndexOf('"', aiMusicIndex);
	QString sceneForThisUri =
		fileContents.left(quoteBefore) + streamUri +
		fileContents.right(fileContents.length() - quoteBefore);

	if (!currentScene.open(QIODevice::ReadWrite)) {
		return "";
	}
	QTextStream stream(&currentScene);
	stream << fileContents;

	currentScene.close();

	const QString thisFilename = makeNewSceneFileName(streamUri);

	const QString saveToPath = sceneFolder + '/' + thisFilename;
	QFile updateFile(saveToPath);
	if (updateFile.open(QIODevice::WriteOnly)) {
		QTextStream stream(&updateFile);
		stream << sceneForThisUri;
	}
	updateFile.close();
	return saveToPath;
}

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

	//QString thisFilename = "OssiaLive";
	//auto colonIdx = streamUri.indexOf(':', 10);	// Need to skip the ":" in "https:"
	//auto slashIdx = streamUri.indexOf('/', colonIdx+2);
	//if (colonIdx > 0 && slashIdx > 0) {
	//	QString roomId =
	//		streamUri.mid(colonIdx + 1, (slashIdx - colonIdx) - 1);
	//	thisFilename += "-" + roomId;
	//}
	const QString thisFilename = makeNewSceneFileName(streamUri);

	const QString saveToPath = sceneFolder + '/' + thisFilename;
	QFile updateFile(saveToPath);
	if (updateFile.open(QIODevice::WriteOnly)) {
		QTextStream stream(&updateFile);
		stream << sceneForThisUri;
	}
	updateFile.close();
	return saveToPath;
}

QString AiMusicProfileWrangler::makeNewSceneFileName(const QString &streamUri)
{
	QString thisFilename = "OssiaLive";
	auto colonIdx =
		streamUri.indexOf(':', 10); // Need to skip the ":" in "https:"
	auto slashIdx = streamUri.indexOf('/', colonIdx + 2);
	if (colonIdx > 0 && slashIdx > 0) {
		QString roomId =
			streamUri.mid(colonIdx + 1, (slashIdx - colonIdx) - 1);
		thisFilename += "-" + roomId;
	}
	thisFilename += ".json";
	return thisFilename;
}
