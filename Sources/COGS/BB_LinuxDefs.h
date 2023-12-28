#pragma once
#ifndef ERR_NOERROR
#define ERR_NOERROR 0
#endif

//#ifdef _LINUX
#if !defined( _CONSOLE ) && !defined( _WINDOWS)
#include <stdarg.h> 
#include <dirent.h>
#include <stdint.h>
#include <assert.h>
#define _strtoui64 strtoull
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#define strcpy_s strcpy
#define printf_s printf
#define sprintf_s  std::snprintf
#define strtok_s strtok_r
#define errno_t int

typedef uint32_t DWORD;
typedef uint16_t WORD;

typedef uint8_t BYTE;
/*
int ::sprintf_s(char *pBuffer,size_t sizeBuf, const char *pFormat,...)
{
	sizeBuf = sizeBuf;
	va_list args;
	va_start (args, pFormat);
	return sprintf(pBuffer, pFormat, args);
}
*/


#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS 0
#endif

inline errno_t fopen_s(FILE** f, const char* name, const char* mode)
{
	errno_t ret = 0;
	assert(f);
	*f = fopen(name, mode);
	/* Can't be sure about 1-to-1 mapping of errno and MS' errno_t */
	if (!*f)
		ret = errno;
	return ret;
}


#endif

