//////////////////////////////////////////////////////////////////////////
// File Name: common.h
// Author: SongLee
// E-mail: lisong.shine@qq.com 
// Created Time: Tue 09 Aug 2016 02:32:41 AM UTC
// Personal Blog: songlee24.github.io
//////////////////////////////////////////////////////////////////////////

#ifndef __REACTOR_COMMON_H__
#define __REACTOR_COMMON_H__

#include <cstdint>
#include "logger.h"

// Magic string "SONG" in little endian
#define MAGIC_CODE 1196314451u 

struct PackageHead {
	union {
		char     magic_code[4];
		uint32_t magic_code_value;
	};
	int32_t length;  // 包体的长度

	PackageHead():magic_code_value(MAGIC_CODE),length(0){}

	bool check_magic_code() const {
		return magic_code_value == MAGIC_CODE;
	}	
};

#endif // __REACTOR_COMMON_H__
