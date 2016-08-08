//////////////////////////////////////////////////////////////////////////
// File Name: logger.cc
// Author: SongLee
// E-mail: lisong.shine@qq.com 
// Created Time: Mon 08 Aug 2016 03:07:21 PM UTC
// Personal Blog: songlee24.github.io
//////////////////////////////////////////////////////////////////////////

#include <time.h>
#include <sys/time.h>
#include "logger.h"

namespace reactor {

// default log level is ERROR
LogLevel Logger::m_log_level = LOG_LEVEL_ERROR;

LogLevel Logger::log_level() {
	return m_log_level;
}

// set log level
void Logger::set_log_level(LogLevel level) {
	m_log_level = level;
} 

// logs are written to stderr
void Logger::log_handler(LogLevel level, const char* filename, int line, const char* format,...) {
	static const char* level_names[] = {"FATAL","ERROR","WARNING","INFO","DEBUG"};
	char buf[1024];
	// 格式化可变参数
	va_list args;
	va_start(args, format);
	vsnprintf(buf, 1024, format, args);
	va_end(args);
	// 获取当前系统时间
	struct timeval now_tv;
    gettimeofday(&now_tv, NULL);
    const time_t seconds = now_tv.tv_sec;
    struct tm t;
    localtime_r(&seconds, &t);
	// 打印 
	fprintf(stderr, "%04d-%02d-%02d %02d:%02d:%02d %s %s:%d:%s\n", 
					t.tm_year+1900, t.tm_mon+1, t.tm_mday,
					t.tm_hour, t.tm_min, t.tm_sec,
					level_names[level], filename, line, buf);
	fflush(stderr);
} 

}
