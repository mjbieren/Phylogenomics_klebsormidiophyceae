#pragma once

/**
 * @file
 * @author Maaike Bierenbroodspot, Thom Bierenbroodspot
 * @brief Header file, containing all public functions and structures for directory listing support, supporting WIndows and Linux
 */


#ifdef _WINDOWS
#include <io.h>
#elif _CONSOLE
#include <io.h>
#else
#include "dirent.h"
#endif

#include <cstdio>
#include <string>
#include <vector>
#include <algorithm> 

#ifndef _HEADER_FILE
#define _HEADER_FILE
#include "BB_General.h"
#endif

#ifndef ERR_NOERROR
#define ERR_NOERROR 0
#endif

//using namespace std;

/**
* This function adds a slash to the given directory name if needed. If it has added a slash it returns true, else it returns false.
* @param pstrDirectory A pointer to the direcotry name to adjust.
*/
inline bool AppendSlash(std::string * pstrDirectory)
{
	const char *pPos = pstrDirectory->c_str();
	bool bUseBackSlash = false;
	bool bUseSlash = false;

	bool bAppended = false;
	while (*pPos)
	{
		if (*pPos == '\\')
			bUseBackSlash = true;
		else if (*pPos == '/')
			bUseSlash = true;

		pPos++;
	}
	pPos--;

	if (*pPos != '\\' && bUseBackSlash)
	{
		rtrim(*pstrDirectory);
		*pstrDirectory += '\\';
		bAppended = true;
	}
	else if (*pPos != '/' && bUseSlash)
	{
		rtrim(*pstrDirectory);
		*pstrDirectory += '/';
		bAppended = true;
	}


	return bAppended;
}

//
//  Case-sensitive pattern match
//
// Example
// Patern: a?cd
// Text:   abcd, axcd etc
inline bool patMatch(const char* szPattern, const char* szText)
{
	switch (szPattern[0])
	{
	case '\0':
		return !szText[0];

	case '*':
		return patMatch(szPattern + 1, szText) || (szText[0] && patMatch(szPattern, szText + 1));

	case '?':
		return szText[0] && patMatch(szPattern + 1, szText + 1);


	default:
		return (szPattern[0] == szText[0]) && patMatch(szPattern + 1, szText + 1);
	}
}

//
//  Case-insensitive pattern match
//
inline bool patiMatch(const char* szPattern, const char* szText)
{
	switch (szPattern[0])
	{
	case '\0':
		return !szText[0];

	case '*':
		return patiMatch(szPattern + 1, szText) || (szText[0] && patiMatch(szPattern, szText + 1));

	case '?':
		return szText[0] && patiMatch(szPattern + 1, szText + 1);

	default:
		return ((toupper(szPattern[0]) == toupper(szText[0])) && patiMatch(szPattern + 1, szText + 1));
	}
}

/**
* This function retrieves a filtered file listing from the given directory and using the given pattern.
* Patterns can be using different wild cards. Match is 'case insensitive'.
* Different patterns can be included by separating them with a semicolon ';'.
* The results are stored in the given vector, which is cleared first. THe vector is NOT sorted on return.
*
* On return the the function reutrns zero if no error occured or the dos errno.
* @param szDirectory The directory to retrieve the file listing from.
* @param szPattern The patterns to use.
* @param pvecFiles  A pointer to the string vector to store the file names.
*/

#ifdef _CONSOLE
long GetFilteredDirList(const char * szDirectory, const char * szPattern, std::vector<std::string> * pvecFiles)
{
	//implementation in WIndows is different, we use _findfirst etc
	std::string strFilename;
	//Just to be sure theres a \ or / a the end and what kind we snould use
	pvecFiles->clear();

	//implement6 multiple patterns
	char szPatterns[MAX_PATH];
	strcpy_s(szPatterns, szPattern);
	char * pNextToken;
	strcpy_s(szPatterns, szPattern);
	char * pToken = strtok_s(szPatterns, ";\r\n", &pNextToken);
	while (pToken)
	{
		strFilename = szDirectory;
		AppendSlash(&strFilename);
		strFilename += pToken;

		trim(strFilename);

		_finddata_t fInfo;

		intptr_t hFind = _findfirst(strFilename.c_str(), &fInfo);
		if (hFind != -1)
		{
			do
			{
				if (fInfo.attrib == _A_SUBDIR)
					continue;

				if (fInfo.name[0] == '.') //We skip the . and ..
					continue;
				//Now see if pattern matches
				pvecFiles->push_back(fInfo.name);
			} while (_findnext(hFind, &fInfo) == 0);

			_findclose(hFind);

		}
		pToken = strtok_s(NULL, ";\r\n", &pNextToken);
	}
	return ERR_NOERROR;
}
#else
inline long GetFilteredDirList(const char * szDirectory, const char * szPattern, std::vector<std::string> * pvecFiles)
{
	DIR *pDir;
	struct dirent *pDent;
	pDir = opendir(szDirectory);   //this part
	if (pDir != nullptr)
	{
		pvecFiles->clear();
		while ((pDent = readdir(pDir)) != nullptr)
		{
			//We only want fies, the rest we skip 
			if (pDent->d_type != DT_REG)
				continue;

			if (pDent->d_name[0] == '.') //We skip the . and ..
				continue;
			//Now see if pattern matches
			if (patiMatch(szPattern, pDent->d_name))
			{
				//Yep! We add it to the vextor
				pvecFiles->push_back(pDent->d_name);
			}
		}
		closedir(pDir);
	}
	else
		return 0;
	return pvecFiles->size();
}

#endif //_WINDOWS