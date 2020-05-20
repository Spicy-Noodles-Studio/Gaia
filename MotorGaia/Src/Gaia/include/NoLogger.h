#pragma once
#ifndef NO_LOGGER_H
#define NO_LOGGER_H
#include <CEGUI\CEGUI.h>

class NoLogger : public CEGUI::Logger
{
public:
	NoLogger();
	~NoLogger();

	void logEvent(const CEGUI::String& message, CEGUI::LoggingLevel level = CEGUI::Standard);
	void setLogFilename(const CEGUI::String& filename, bool append = false);
};
#endif