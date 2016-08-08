//////////////////////////////////////////////////////////////////////////
// File Name: logger.h
// Author: SongLee
// E-mail: lisong.shine@qq.com 
// Created Time: Mon 08 Aug 2016 03:05:46 PM UTC
// Personal Blog: songlee24.github.io
//////////////////////////////////////////////////////////////////////////

#include <cstdarg>
#include <cstdio>

namespace reactor {

enum LogLevel {
	LOG_LEVEL_FATAL   = 0,
	LOG_LEVEL_ERROR   = 1,
	LOG_LEVEL_WARNING = 2,
	LOG_LEVEL_INFO    = 3,
	LOG_LEVEL_DEBUG   = 4
};

class Logger {
private:
	static LogLevel m_log_level;
public:
	static LogLevel log_level();
	static void set_log_level(LogLevel level);
	static void log_handler(LogLevel level, const char* filename, int line, const char* fmt,...); 
};

#define SET_LOG_LEVEL(level) \
	reactor::Logger::set_log_level(reactor::LOG_LEVEL_##level)

#define LOG(level, fmt, arg...) \
	(reactor::LOG_LEVEL_##level > reactor::Logger::log_level()) ? \
		(void)0 : reactor::Logger::log_handler(reactor::LOG_LEVEL_##level, __FILE__, __LINE__, fmt, ##arg)

}
