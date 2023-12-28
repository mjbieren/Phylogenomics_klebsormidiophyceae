#pragma once
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <errno.h>
#include <cstdio>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h> 
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <math.h>
#include <algorithm>

//Boost Lib
#include <boost/config.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/test/debug.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/multi_index/global_fun.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string.hpp>

#ifndef ERR_NOERROR
#define ERR_NOERROR 0
#endif

#ifndef TAB_ASCII
#define TAB_ASCII 9
#endif


#define _DEBUG_TEST

#ifdef _WINDOWS
//#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <WinBase.h>
#include <io.h>
#define EOL "\r\n"
#elif _LINUX
#endif
//#include "smartptr.h"
//#include "sha2.h"

#ifdef _LINUX
#include <unistd.h>
#ifndef VECTOR_H
#define VECTOR_H
#endif

#include "BB_PropertyBag.h"

#include <iosfwd>
#include <cstddef>
#define EOL "\n"

#endif //_LINUX

#define SIZE_BUFFER_MICRO 8
#define SIZE_BUFFER_TINY 64
#define SIZE_BUFFER_SMALL 128
#define SIZE_BUFFER_MEDIUM 256
#define SIZE_BUFFER_BIG 10*1024
#define SIZE_BUFFER_ONE_KB 1024
#define SIZE_BUFFER_ONE_MB 1024*1024
#define SIZE_BUFFER_HUNDRED_MB 1024*1024*100
#define SIZE_BUFFER_ONE_GIG 1024*1024*1024
#define SIZE_BUFFER_HUNDRED_MB 1024*1024*100
#define SIZE_ERRORBUFFER SIZE_BUFFER_SMALL
#define COLUMN_QUERY_BLASTFILE 0
#define COLUMN_DATABASE_BLASTFILE 1

// This macro moves thepointer to the end of a line or to the closing terminator
#define MOVE_PTR_TO_EOL(pValue) while (*pValue && *pValue != '\r' && *pValue != '\n') pValue++; pValue--;
#define MOVE_PTR_TO_EOL_REAL(pValue) while (*pValue && *pValue != '\r' && *pValue != '\n') pValue++;

//This macro moves thepointer to the end of a previous line or to the closing terminator. It also takes care of trailing tabs or spaces
#define MOVE_PTR_TO_PREV_EOL(pValue) while (*pValue && *pValue != '\r' && *pValue != '\n') pValue--;while (*pValue && (*pValue == '\r' || *pValue == '\n' || *pValue == ' ' || *pValue == '\t')) pValue--;

//This Macro moves the pointer back to the start of a protein id within a treefile
#define MOVE_PTR_TO_START_OF_TREE_NODE(pValue) while (*pValue && *pValue != '(' && *pValue != ')' && *pValue != ',') pValue--;

//This macro moves thepointer to the beginning of the next new line or to the closing terminator
#define MOVE_PTR_TO_BOL(pValue) while (*pValue && *pValue != '\r' && *pValue != '\n') pValue++; while (*pValue && (*pValue == '\r' || *pValue == '\n' )) pValue++;

//This macro moves thepointer to the beginning of the first char that is not a space or tab or to the closing terminator 
#define MOVE_PTR_TO_NON_SPACE(pValue) while (*pValue && (*pValue == ' ' || *pValue == '\t')) pValue++;
#define MOVE_PTR_TO_SPACE(pValue) while (*pValue && *pValue != ' ' && *pValue != '\t') pValue++;
#define MOVE_PTR_TO_TAB(pValue) while (*pValue && *pValue != '\t') pValue++;
#define MOVE_PTR_TO_TAB_OR_EOL(pValue)while(*pValue && *pValue != '\t' && *pValue != '\r' && *pValue != '\n') pValue++;
#define MOVE_PTR_TO_MONKEYTAIL(pValue) while(*pValue && *pValue != '@') pValue++;
#define MOVE_PTR_TO_SEMI_COLON(pValue) while(*pValue && *pValue != ';') pValue++;
#define MOVE_PTR_TO_COLON(pValue) while(*pValue && *pValue != ':') pValue++;
#define MOVE_PTR_TO_COMMA(pValue) while(*pValue && *pValue != ',') pValue++;
#define MOVE_PTR_TO_CHAR(pValue, cValue) while (*pValue && *pValue != cValue && *pValue != '\r' && *pValue != '\n' ) pValue++;

#define MOVE_PTR_TO_DIGIT(pValue) while (*pValue && !isdigit(*pValue) && *pValue != '\r' && *pValue != '\n' ) pValue++;
#define MOVE_PTR_TO_ALPHA(pValue)  while (*pValue && (*pValue < 'a' || *pValue > 'z') && *pValue != '\r' && *pValue != '\n') pValue++;
#define MOVE_PTR_TO_ALPHA2(pValue) while(*pValue && (*pValue < 'a' || *pValue > 'z') && (*pValue < 'A' || *pValue > 'Z') && *pValue != '\r' && *pValue != '\n') pValue++;

#define MOVE_PTR_TO_NEXT_SEQUENCE(pValue) while (*pValue && *pValue != '>') pValue++;

//Move pointer to previous non digit
#define MOVE_PTR_TO_PREV_NONDIGIT(pValue) while (*pValue && isdigit(*pValue) && *pValue != '\r' && *pValue != '\n' ) pValue--;

//We define ASSERT and VERIFY if needed. On release build, ASSERT is gone and VERIFY still execs the parms section 
#ifndef ASSERT
#ifdef _DEBUG
#define ASSERT( expr ) if(!(expr)) assert(0)
#else
#define ASSERT( expr ) ((void)0)
#endif
#endif



