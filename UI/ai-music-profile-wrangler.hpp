#pragma once

#include <QString>

class AiMusicProfileWrangler { 
public:
	QString replaceAiMusicStream(const QString &sceneFolder,
				  const QString &sceneFileName,
				  const QString &streamUri);

	QString makeOssiaSceneFile(const QString &sceneFolder,
				   const QString &streamUri);

private:
	QString makeNewSceneFileName(const QString &streamUri);
};
