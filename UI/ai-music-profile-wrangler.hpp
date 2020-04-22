#pragma once

#include <QString>

class AiMusicProfileWrangler {
public:
	bool updateStreamUrl(const QString& jsonPath, const QString &streamUrl);
};
