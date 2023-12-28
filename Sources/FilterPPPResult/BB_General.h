#pragma once

#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <algorithm> 
#include <ctype.h>
#include <string.h>

#define MAX_PATH 255

#ifndef ERR_NOERROR
	#define ERR_NOERROR 0
#endif

#ifndef ERR_SUCCESS
	#define ERR_SUCCESS 0
#endif


#ifdef _LINUX
#include "BB_LinuxDefs.h"
//#define strcpy_s strcpy 
//#define _stricmp strcasecmp
#endif



// String trimming functions
// trim from start (in place)
inline void ltrim(std::string &s) 
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !isspace(ch);
		}));
}

// trim from end (in place)
inline void rtrim(std::string &s) 
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !isspace(ch);
		}).base(), s.end());
}

// trim from both ends (in place)
inline void trim(std::string &s) 
{
	ltrim(s);
	rtrim(s);
}

// trim from start (copying)
inline std::string ltrim_copy(std::string s) 
{
	ltrim(s);
	return s;
}

// trim from end (copying)
inline std::string rtrim_copy(std::string s) 
{
	rtrim(s);
	return s;
}

inline char *  FindStartOfLine(char * pPos, bool bStrip = false)
{
	if (bStrip)
	{
		//This function walks the buffer untill is passes \r\n
		while (*pPos && *pPos != '\n' && *pPos != '\r')
		{
			if (*pPos && *pPos == '#')
			{
				*pPos = '\0';
			}
			pPos++;
		}
		//now find beginning of line
		while (*pPos && (*pPos == '\n' || *pPos == '\r'))
		{
			*pPos = '\0';
			pPos++;
		}
	}
	else
	{
		//This function walks the buffer untill is passes \r\n
		while (*pPos && *pPos != '\n' && *pPos != '\r')
			pPos++;
		//now find beginning of line
		while (*pPos && (*pPos == '\n' || *pPos == '\r'))
			pPos++;
	}
	return pPos;
}
//Sample Use of CSmartPtr

//CSmartPtr<int> pFoobar = new int(100);
//In case the ointer is pointing to an array of a certian class (this while delete calls every destructor
//CSmartPtr<CFooBar> pFoobar( new CFooBar[100], true);


template < typename T>
class CSmartPtr 
{
public:
	CSmartPtr(bool bIsArray = false) 
	{
		m_pType = nullptr;
		m_puCount = nullptr;
		m_bIsArray = bIsArray;
	}

	CSmartPtr(T* pRawPtr, bool bIsArray = false) 
	{
		m_pType = nullptr;
		m_puCount = nullptr;
		m_bIsArray = bIsArray;
		Attach(pRawPtr, m_bIsArray);
	}

	CSmartPtr(const CSmartPtr<T>& CopyFrom) 
	{
		m_pType = nullptr;
		m_puCount = nullptr;

		*this = CopyFrom;

	}

	~CSmartPtr() 
	{
		Release();
	}

	// Overloaded operators
	T& operator *() const { assert(m_pType); return *m_pType; }
	T* operator ->() const { assert(m_pType); return m_pType; }

	CSmartPtr<T>& operator =(T* pRawPtr) { Attach(pRawPtr); return *this; }

	CSmartPtr<T>& operator =(const CSmartPtr<T>& CopyFrom) 
	{
		if (&CopyFrom != this) 
		{              // Check for assignment to self
			Release();                    // Release the current pointer

			if (CopyFrom.IsNull()) 
			{      // If the source raw pointer is null
				return *this;         // Then just bail out
			}

			m_pType = CopyFrom.m_pType;   // Make our pointers reference the same raw pointer and counter
			m_puCount = CopyFrom.m_puCount;
			m_bIsArray = CopyFrom.m_bIsArray;
			assert(m_puCount);            // We now point to something valid, so increment the counter
			(*m_puCount)++;

		}

		return *this;

	}
	// !Overloaded operators

	operator bool() const 
	{
		return !IsNull();
	}

	bool IsNull() const 
	{
		return (m_pType == NULL);
	}

	CSmartPtr<T>& Attach(T* pRawPtr, bool bIsArray = false)
	{
		if (pRawPtr != m_pType) 
		{                        // Check for assignment to self
			Release();                               // Release the current pointer
			m_pType = pRawPtr;                       // Point to the passed raw pointer
			m_bIsArray = bIsArray;

			if (m_pType) 
			{                           // If the passed pointer was valid
				m_puCount = new unsigned int(1); // Create a new counter starting at 1 (us)
			}

		}

		return *this;

	}

	void Release() 
	{
		if (m_pType) 
		{              // Only release if there is something to be released
			assert(m_puCount);
			(*m_puCount)--;     // Decrement our counter

			if (!*m_puCount) 
			{  // If we were the last reference to this pointer, then clean up
				delete m_puCount;
				if (m_bIsArray)
					delete[] m_pType;
				else
					delete m_pType;
			}

			m_pType = nullptr;     // Get rid of our references
			m_puCount = nullptr;
		}
	}

	// Getters
	T* GetPtr() const { return m_pType; }
	unsigned int GetCount() const { return (m_puCount) ? *m_puCount : 0; }
	// !Getters
private:
	T*            m_pType = nullptr;
	unsigned int* m_puCount = 0;
	bool m_bIsArray = false;
};

template < typename T>
bool operator ==(T* lhs, const CSmartPtr<T>& rhs) { return (lhs == rhs.GetPtr()); }

template < typename T>
bool operator ==(const CSmartPtr<T>& lhs, T* rhs) { return (lhs.GetPtr() == rhs); }

template < typename T>
bool operator ==(const CSmartPtr<T>& lhs, const CSmartPtr<T>& rhs) { return (lhs.GetPtr() == rhs.GetPtr()); }


//ASCII DEFINITIONS. Since we often have to search based on single characters in a file

#ifndef ZERO_TERMINATION_ASCII
#define ZERO_TERMINATION_ASCII 0
#endif

#ifndef TAB_ASCII
#define TAB_ASCII 9
#endif

#ifndef SPACE_ASCII
#define SPACE_ASCII 32
#endif

#ifndef ROUND_BRACKET_OPEN_ASCII
#define ROUND_BRACKET_OPEN_ASCII 40
#endif

#ifndef ROUND_BRACKET_CLOSE_ASCII
#define ROUND_BRACKET_CLOSE_ASCII 41
#endif

#ifndef COMMA_ASCII
#define COMMA_ASCII 44
#endif

#ifndef COLON_ASCII
#define COLON_ASCII 58
#endif

#ifndef SEMICOLON_ASCII
#define SEMICOLON_ASCII 59
#endif

#ifndef AT_ASCII
#define AT_ASCII 60
#endif

#ifndef SQUARE_BRACKET_OPEN_ASCII
#define SQUARE_BRAKCET_OPEN_ASCII 91
#endif

#ifndef SQUARE_BRACKET_CLOSE_ASCII
#define SQUARE_BRACKET_CLOSE_ASCII 93
#endif

#ifndef CURLY_BRACKET_OPEN_ASCII
#define CURLY_BRACKET_OPEN_ASCII 123
#endif

#ifndef CURLY_BRACKET_CLOSE_ASCII
#define CURLY_BRACKET_CLOSE_ASCII 125
#endif




