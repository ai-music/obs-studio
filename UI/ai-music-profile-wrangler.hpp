#pragma once

#include <QString>

class AiMusicProfileWrangler { 
public:
	QString makeOssiaSceneFile(const QString& jsonPath, const QString &streamUri);
};
