/******************************************************************************
*
*   Author(s)	:   Maaike Bierenbroodspot
*
*   Last edit by:   Maaike Bierenbroodspot
*   Edit date   :   2022-07-08 17:18:11
*
******************************************************************************/

#pragma once

#pragma GCC diagnostic ignored "-Wunused-parameter"
//#pragma warning(disable : 4996) //Removes the strerror unsafe error

//#define _CRT_SECURE_NO_DEPRECATE

#include <cstdio>
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <algorithm> 
#include <cctype>
#include <assert.h>
#include <vector>
#include <map>
#include <algorithm>
#ifndef _PROPERTY_BAG_H
#define _PROPERTY_BAG_H
#include "BB_General.h"
#include "BB_DirList.h"

enum PROPERTYBAG_VERBS
{
	VERB_UNKNOWN,
	VERB_ASK,
	VERB_ASKNO,
	VERB_ASKYES,
	VERB_NO,
	VERB_YES,
	VERB_IGNORE,
	VERB_ABORT,
	VERB_AUTO,

};
enum CPROPERTYBAG_ERRORS
{
	PROP_ERR_START = 20000,
	PROP_ERR_EPERM = EPERM + PROP_ERR_START, // Operation not permitted
	PROP_ERR_ENOENT = ENOENT + PROP_ERR_START, // No such file or directory
	PROP_ERR_ESRCH = ESRCH + PROP_ERR_START, // No such process
	PROP_ERR_EINTR = EINTR + PROP_ERR_START, // Interrupted system call
	PROP_ERR_EIO = EIO + PROP_ERR_START, // Input / output error
	PROP_ERR_OPEN_FILE = 50 + PROP_ERR_START,
	PROP_ERR_KEY_NOT_FOUND,
	PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED,
	PROP_ERR_SECTION_NOT_FOUND,
	PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED,
	PROP_ERR_READ_FILE,
	PROP_ERR_SECTION_NO_CLOSE_BRACKED,
	PROP_ERR_CONF_INVALID_INCLUDE,
	PROP_ERR_END
};


class CPropertySection;

class CPropertyBag;



class CPropertySection
{
protected:
	std::string m_strSectionName;
	std::map<std::string, std::string> m_mapProps;
public:

	CPropertySection() {}
	virtual ~CPropertySection() {}
	CPropertySection(const char* szSectionName)
	{
		SetSectionName(szSectionName);
	}
	CPropertySection(const char* szSectionName, const char* szKey, const char* szValue)
	{
		SetSectionName(szSectionName);
		SetValueString(szKey, szValue);
	}

	CPropertySection(const CPropertySection& secProp)
	{
		m_strSectionName = secProp.m_strSectionName;
		m_mapProps = secProp.m_mapProps;
	}


	const char* GetSectionName() { return m_strSectionName.c_str(); }
	void SetSectionName(const char* szSectionName) { m_strSectionName = szSectionName; }

	virtual long RemoveProperty(const char* szKey)
	{
		return SetValueString(szKey, "");
	}


	long GetValueString(const char* szKey, std::string* pstrValue, const char* pszDefault = NULL)
	{
		long lRet = ERR_NOERROR;
		std::string strKey(szKey);

		//Now find the key
		std::map<std::string, std::string>::iterator iter = m_mapProps.find(strKey);
		if (iter != std::end(m_mapProps))
		{
			//Found!

			*pstrValue = iter->second;
		}
		else
		{
			lRet = PROP_ERR_KEY_NOT_FOUND;
			if (pszDefault)
			{
				lRet = PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED;
				*pstrValue = pszDefault;
			}
		}
		return lRet;
	}

	long SetValueString(const char* szKey, const char* pszValue)
	{
		long lRet = ERR_NOERROR;
		assert(szKey);
		std::string strKey(szKey);
		std::string strValue;

		trim(strKey);

		if (pszValue)
		{
			strValue = pszValue;
			trim(strValue);
		}

		//When pszValue is empty, we should remove the key
		if (strValue.empty())
		{
			std::map<std::string, std::string>::iterator iter = m_mapProps.find(strKey);
			if (iter != std::end(m_mapProps))
			{
				m_mapProps.erase(iter);
			}
			return lRet;
		}

		m_mapProps[strKey] = strValue;

		return lRet;
	}



	long  GetValueInt(const char* szKey, int* piValue, int iDefault = 0)
	{
		std::string strValue;
		long lRet = GetValueString(szKey, &strValue);
		switch (lRet)
		{
		case ERR_NOERROR:
			*piValue = atoi(strValue.c_str());
			break;
		case PROP_ERR_KEY_NOT_FOUND:
			lRet = PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED;
			*piValue = iDefault;
			break;
		default:
			assert(0);
			break;
		}
		return lRet;
	}

	long SetValueInt(const char* szKey, int iValue)
	{
		char szArr[50];
		sprintf_s(szArr, sizeof(szArr), "%i", iValue);
		SetValueString(szKey, szArr);
		return ERR_NOERROR;
	}

	long  GetValueLong(const char* szKey, long* plValue, long lDefault = 0)
	{
		std::string strValue;
		long lRet = GetValueString(szKey, &strValue);
		switch (lRet)
		{
		case ERR_NOERROR:
			*plValue = atol(strValue.c_str());
			break;
		case PROP_ERR_KEY_NOT_FOUND:
			lRet = PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED;
			*plValue = lDefault;
			break;
		default:
			assert(0);
			break;
		}
		return lRet;
	}

	long SetValueLong(const char* szKey, long lValue)
	{
		char szArr[50];
		sprintf_s(szArr, sizeof(szArr), "%li", lValue);
		SetValueString(szKey, szArr);
		return ERR_NOERROR;

	}

	long GetValueFloat(const char* szKey, float* pfValue, float fDefault = 0)
	{
		std::string strValue;
		long lRet = GetValueString(szKey, &strValue);
		switch (lRet)
		{
		case ERR_NOERROR:
			*pfValue = (float)atof(strValue.c_str());
			break;
		case PROP_ERR_KEY_NOT_FOUND:
			lRet = PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED;
			*pfValue = fDefault;
			break;
		default:
			assert(0);
			break;
		}
		return lRet;
	}

	long SetValueFloat(const char* szKey, float fValue)
	{
		char szArr[50];
		sprintf_s(szArr, sizeof(szArr), "%f", fValue);
		SetValueString(szKey, szArr);
		return ERR_NOERROR;
	}


	long GetValueBool(const char* szKey, bool* pbValue, bool bDefault = false)
	{
		std::string strValue;
		long lRet = GetValueString(szKey, &strValue);
		switch (lRet)
		{
		case ERR_NOERROR:
		{
			//It can be true, false or a number. Simple, if it is a number it is aways true
			int iValue = atoi(strValue.c_str());
			if (iValue || (_stricmp(strValue.c_str(), "true") == 0))
				*pbValue = true;
			else
				*pbValue = false;
		}
		break;
		case PROP_ERR_KEY_NOT_FOUND:
			lRet = PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED;
			*pbValue = bDefault;
			break;
		default:
			assert(0);
			break;
		}
		return lRet;
	}

	long SetValueBool(const char* szKey, bool bValue)
	{
		if (bValue)
			SetValueString(szKey, "True");
		else
			SetValueString(szKey, "False");

		return ERR_NOERROR;
	}

	long GetValueEnumVerb(const char* szKey, PROPERTYBAG_VERBS* peValue)
	{
		std::string strValue;
		long lRet = GetValueString(szKey, &strValue);
		switch (lRet)
		{
		case ERR_NOERROR:
		{
			//It can be true, false or a number. Simple, if it is a number it is aways true

			if (_stricmp(strValue.c_str(), "ASKYES") == 0)
			{
				*peValue = VERB_ASKYES;
			}
			else if (_stricmp(strValue.c_str(), "ASKNO") == 0)
			{
				*peValue = VERB_ASKNO;
			}
			else if (_stricmp(strValue.c_str(), "ASK") == 0)
			{
				*peValue = VERB_ASK;
			}
			else if (_stricmp(strValue.c_str(), "NO") == 0)
			{
				*peValue = VERB_NO;
			}
			else if (_stricmp(strValue.c_str(), "YES") == 0)
			{
				*peValue = VERB_YES;
			}
			else if (_stricmp(strValue.c_str(), "IGNORE") == 0)
			{
				*peValue = VERB_IGNORE;
			}
			else if (_stricmp(strValue.c_str(), "ABORT") == 0)
			{
				*peValue = VERB_ABORT;
			}
			else
			{
				assert(0); //should never happen

				*peValue = VERB_UNKNOWN;
			}
		}

		break;
		case PROP_ERR_KEY_NOT_FOUND:
			lRet = PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED;
			*peValue = VERB_UNKNOWN;
			break;
		default:
			assert(1);
			break;
		}

		return lRet;
	}

	long SetValueEnumVerb(const char* szKey, PROPERTYBAG_VERBS eValue)
	{
		std::string strValue;
		switch (eValue)
		{
		case VERB_ASK:
			strValue = "ASK";
			break;
		case VERB_ASKNO:
			strValue = "ASKNO";
			break;
		case VERB_ASKYES:
			strValue = "ASKYES";
			break;
		case VERB_NO:
			strValue = "NO";
			break;
		case VERB_YES:
			strValue = "YES";
			break;
		case VERB_IGNORE:
			strValue = "IGNORE";
			break;
		case VERB_ABORT:
			strValue = "ABORT";
			break;
		default:
			assert(0);
			strValue = "UNKOWN";
			break;
		}
		return SetValueEnumVerb(szKey, strValue.c_str());
	}

	long SetValueEnumVerb(const char* szKey, const char* szVerb)
	{
		return CPropertySection::SetValueString(szKey, szVerb);
	}



};


class CPropertyFile
{
protected:
	std::vector<std::string> m_vecFilesRead;
	char m_szErrMessage[128];
	long m_lLastError = 0;
	std::string m_strConfFileName;
	bool FileAlreadyRead(const char* pFile)
	{
		std::string strFilename = pFile;
		trim(strFilename);
		//Filenames can be slashes (linux) or backlsashes(windows)
		//To be sure we store the right comparison, we all change baclslashes to slashes
		std::replace(strFilename.begin(), strFilename.end(), '\\', '/');
		std::vector<std::string>::iterator iter = std::find(m_vecFilesRead.begin(), m_vecFilesRead.end(), strFilename);
		if (iter != m_vecFilesRead.end())
			return true;

		m_vecFilesRead.push_back(strFilename);
		return false;
	}
public:
	virtual const char* GetErrorDescription(long lError)
	{
		switch (lError)
		{
		case ERR_NOERROR:
			return "Error ERR_NOERROR, No error occured.";
		case PROP_ERR_EPERM:// = EPERM, // Operation not permitted
			return "Error PROP_ERR_EPERM, Operation not permitted.";
		case PROP_ERR_ENOENT:// = ENOENT, // No such file or directory
			return "Error PROP_ERR_ENOENT, No such file or directory.";
		case PROP_ERR_ESRCH:// = ESRCH, // No such process
			return "Error PROP_ERR_ESRCH, No such process.";
		case PROP_ERR_EINTR:// = EINTR, // Interrupted system call
			return  "Error PROP_ERR_EINTR, Interrupted system call.";
		case PROP_ERR_EIO:// = EIO, // Input / output error
			return "Error PROP_ERR_EIO, Input / output error.";
		case PROP_ERR_OPEN_FILE: // = 50,
			return "Error PROP_ERR_OPEN_FILE, error while opening Property file.";
		case PROP_ERR_KEY_NOT_FOUND:
			return "Error PROP_ERR_KEY_NOT_FOUND, the given key was not found in the propery bag, default is used if available.";
		case PROP_ERR_SECTION_NOT_FOUND://
			return "Error PROP_ERR_SECTION_NOT_FOUND, the given Section was not found in the propery bag, default is used if available.";
		case PROP_ERR_READ_FILE:
			return "Error PROP_ERR_READ_FILE, there was an error reading the property file.\r\n";
		case PROP_ERR_SECTION_NO_CLOSE_BRACKED:
			return "Error PROP_ERR_SECTION_NO_CLOSE_BRACKED, while parsing the property file a closing section bracket was missing.";
		}
		assert(0);

		sprintf_s(m_szErrMessage, sizeof(m_szErrMessage), "Unexpected error: %li, an unknown error has accured", lError);



		return m_szErrMessage;
	}

	virtual void ResetLastError()
	{
		m_lLastError = ERR_NOERROR;
		m_szErrMessage[0] = '\0';
	}
	virtual void SetLastError(long lLastError)
	{
		m_lLastError = lLastError;
	}
	virtual long GetLastError() { return m_lLastError; }
	CPropertyFile()
	{
	}
	CPropertyFile(const char* szConfFileName)
	{
		SetConfFileName(szConfFileName);
	}

	CPropertyFile(const CPropertyFile& secFile)
	{
		assert(1); //We do not want a copy constructor!
	}

	virtual void SetConfFileName(const char* szConfFileName)
	{
		m_strConfFileName = szConfFileName;
	}
	virtual ~CPropertyFile()
	{
	}


	char* GetContent(const char* pConfFile)
	{
		FILE* hFile;
		if (!pConfFile)
		{
			pConfFile = m_strConfFileName.c_str();
		}

		//Now open
		struct stat statCfg;
		//Fist it file exists
		if (stat(pConfFile, &statCfg))
		{
			//error
#ifdef _DEBUG //For debugging purposes but the erro and the enum ar equal
			switch (errno)
			{
			case EPERM:  // Operation not permitted
				SetLastError(PROP_ERR_EPERM);
				break;
			case ENOENT:  // No such file or directory
				SetLastError(PROP_ERR_ENOENT);
				break;
			case ESRCH:  // No such process
				SetLastError(PROP_ERR_ESRCH);
				break;
			case EINTR:  // Interrupted system call
				SetLastError(PROP_ERR_EINTR);
				break;
			case EIO:  // Input / output error
				SetLastError(PROP_ERR_EIO);
				break;
			default:
				assert(1);
				break;
			}
#endif
			SetLastError(errno);
			return nullptr;
		}

		size_t sizeContent = statCfg.st_size;
		size_t sizeBuffer = sizeContent + 100;

		errno_t errnum = fopen_s(&hFile, pConfFile, "rb");

		if (!hFile || errnum)
		{
			int iErr = errno;
			std::cout << "ERROR opening file ";
			std::cout << pConfFile;
			std::cout << " with error: ";
			std::cout << strerror(iErr);
			std::cout << "\n";
			SetLastError(PROP_ERR_OPEN_FILE);

			return nullptr;
		}

		assert(sizeBuffer);
		char* pBuffer = new char[sizeBuffer];
		assert(pBuffer);

		//We blank the buffer
		memset(pBuffer, 0, sizeBuffer);

		//Now read it into the buffer
		size_t sizeRead = fread(pBuffer, 1, sizeContent, hFile);
		if (sizeContent != sizeRead)
		{
			SetLastError(PROP_ERR_READ_FILE);
			fclose(hFile);
			delete pBuffer;
			return nullptr;
		}
		//We don't need the file anymore
		fclose(hFile);

		return pBuffer;
	}

};


class CPropertyBag : public CPropertyFile
{
protected:
	std::map<std::string, CPropertySection> m_mapSections;
public:
	CPropertyBag() {}
	CPropertyBag(const char* pConvFileName) : CPropertyFile(pConvFileName)
	{}
	virtual ~CPropertyBag()
	{}
	CPropertyBag(const CPropertyBag& secBag)
	{
		assert(0);
		m_mapSections = secBag.m_mapSections;
	}

	virtual long RemoveProperties(const char* szSection, const char* szKey)
	{
		//This function removes the property of a section or if no section it removes all properties.
		//If also a key is given it just removes the key
		//first find the section
		long lRet = ERR_NOERROR;
		std::string strSection;

		if (!szSection || !*szSection)
		{
			//We just remove everything!
			m_mapSections.clear();
		}
		else
		{
			strSection = szSection;
		}

		std::map<std::string, CPropertySection>::iterator iter = m_mapSections.find(strSection);

		if (iter != std::end(m_mapSections))
		{
			//Found!
			//Now get the property
			//Now see if there's a skey
			if (szKey)
			{
				return iter->second.RemoveProperty(szKey);
			}
			else
			{
				//Just remove the complete secion
				m_mapSections.erase(iter);
			}
			return lRet;
		}
		else
		{
			lRet = PROP_ERR_SECTION_NOT_FOUND;
		}

		return lRet;

	}


	virtual long ReadProperties(const char* pConfigFile = NULL)
	{
		//We reset the previous errror if any
		SetLastError(ERR_NOERROR);
		//Now we retrieve the buffer with the content of the file
		if (!pConfigFile)
		{
			assert(m_strConfFileName.length());
			pConfigFile = m_strConfFileName.c_str();
		}

		if (FileAlreadyRead(pConfigFile)) //We do not read a file twice!
		{
			return GetLastError();
		}

		char* pBuffer = GetContent(pConfigFile);
		//assert(pBuffer != NULL);
		if (pBuffer == NULL)
			return GetLastError();

		char* pPos = pBuffer;
		//The buffer points to the beginning of a the contents of the file
		//It may or my not have any sections with parameters
		//We ignote everything after a #
		std::string strSection;
		std::string strValue;
		while (*pPos && GetLastError() == ERR_NOERROR)
		{
			if (*pPos == '#')
			{
				pPos = FindStartOfLine(pPos);
			}
			else
			{
				if (*pPos)
				{
					//We are now at the beginnng of a new line
					//It can be the beginning of a section or a parameter
					//if it is the begining of a section, the line starts with a '['
					//If not the file is starting without any sections
					if (*pPos == '[')
					{
						//We have found a section
						pPos++;
						strSection = "";
						while (*pPos && *pPos != ']' && *pPos != '\r' && *pPos != '\n')
						{
							strSection += *pPos++;
						}
						assert(*pPos && *pPos == ']'); //check that we have a close
						if (*pPos && *pPos != ']')
						{
							delete pBuffer;
							SetLastError(PROP_ERR_SECTION_NO_CLOSE_BRACKED);
							return GetLastError();
						}
						pPos = FindStartOfLine(pPos);
						//We now have to find the properties and property values
					}
					else
					{
						//It can also be that we have a line in the form of 
						//include /etc/ld.so.conf.d/*.conf
						if (strncmp("include ", pPos, 8) == 0)
						{
							//So we have to take care of an include!
							//We do that by recursively calling the ReadProperties function
							char* pLine = pPos;
							pPos = FindStartOfLine(pPos, true);
							//Walk to first space
							while (*pLine && (*pLine != ' '))
								pLine++;

							//Point to first non space
							if (*pLine)
							{
								while (*pLine && *pLine == ' ')
									pLine++;
							}
							else
							{
								//invalid include without a filename
								SetLastError(PROP_ERR_CONF_INVALID_INCLUDE);
								delete pBuffer;
								return PROP_ERR_CONF_INVALID_INCLUDE;
							}
							//Ceck if there is indeed something
							if (!*pLine)
							{
								//invalif dinclude so just skip
								continue;
							}
							//Now find filter (or filename)
							char* pFilter = strrchr(pLine, '/');
							if (!pFilter)
							{
								pFilter = strrchr(pLine, '\\');
							}

							if (pFilter)
							{
								*pFilter = '\0';
								pFilter++;
							}
							else
							{
								//No path available
								//We use the same path of the original conf file
								pFilter = pLine;
								char szBuffer[255];
#ifdef _WINDOWS
								strcpy_s(szBuffer, pConfigFile);
#elif __linux__
								strcpy_s(szBuffer, pConfigFile);
#endif // _WINDOWS

								//Now remove the filename
								pLine = strrchr(szBuffer, '/');
								if (!pLine)
								{
									pLine = strrchr(szBuffer, '\\');
								}
								if (pLine)
									*pLine = '\0';

								pLine = szBuffer;
							}
							std::vector<std::string> vecFiles;
							//Now we have to split the directory and the filter
							GetFilteredDirList(pLine, pFilter, &vecFiles);
							std::vector<std::string>::iterator iterFiles = vecFiles.begin();
							while (iterFiles != vecFiles.end())
							{
								std::string strFullName = pLine;
								AppendSlash(&strFullName);
								strFullName += *iterFiles;

								//Now read the include property file
								if (ERR_NOERROR != ReadProperties(strFullName.c_str()))
								{
									delete pBuffer;
									return GetLastError();
								}
								iterFiles++;
							}
							continue;
						}

						//It can be that we have a line like 'reset properties'
						//That means we need to reset al the properties for this section and just restart
						if (strncmp("reset properties", pPos, 16) == 0)
						{
							//We need to reset all the properties for this section 
							RemoveProperties(strSection.c_str(), NULL);
							pPos = FindStartOfLine(pPos, true);
							continue;
						}
						//Or
						//We are in the position af point to a property in the form of
						//<Key>=<prop value> #comment



						//Sometimes a = is not used and ater the value comment can occure
						char* pKey = pPos;
						while (*pPos && *pPos != '=' && *pPos != ' ')
							pPos++;
						*pPos++ = '\0';
						while (*pPos && (*pPos == '=' || *pPos == ' '))
							pPos++;

						char* pValue = pPos;

						pPos = FindStartOfLine(pPos, true);

						//Not set the valuse
						SetLastError(SetValueString(strSection.c_str(), pKey, pValue));
					}
				}
			}
		}

		//And we release the buffer
		delete pBuffer;
		return GetLastError();
	}

	long GetValueString(const char* szKey, std::string* pstrValue, const char* pszDefault = NULL)
	{
		return GetValueString("", szKey, pstrValue, pszDefault/* = NULL*/);
	}

	long GetValueString(const char* szSection, const char* szKey, std::string* pstrValue, const char* pszDefault = NULL)
	{
		//first find the section
		long lRet = ERR_NOERROR;
		std::string strSection;

		if (!szSection || !*szSection)
		{
			strSection = "Empty Section";
		}
		else
		{
			strSection = szSection;
			trim(strSection);
		}

		std::map<std::string, CPropertySection>::iterator iter = m_mapSections.find(strSection);

		if (iter != std::end(m_mapSections))
		{
			//Found!
			//Now get the property
			return iter->second.GetValueString(szKey, pstrValue, pszDefault);
		}
		else
		{
			lRet = PROP_ERR_SECTION_NOT_FOUND;
			if (pszDefault)
			{
				lRet = PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED;
				*pstrValue = pszDefault;
			}
		}

		return lRet;
	}
	long GetValueInt(const char* szKey, int* piValue, int iDefault = 0)
	{
		return GetValueInt("", szKey, piValue, iDefault);
	}

	long GetValueInt(const char* szSection, const char* szKey, int* piValue, int iDefault = 0)
	{
		std::string strValue;
		long lRet = GetValueString(szSection, szKey, &strValue);
		switch (lRet)
		{
		case ERR_NOERROR:
			*piValue = atoi(strValue.c_str());
			break;
		case PROP_ERR_SECTION_NOT_FOUND:
			lRet = PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED;
			*piValue = iDefault;
			break;
		case PROP_ERR_KEY_NOT_FOUND:
			lRet = PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED;
			*piValue = iDefault;
			break;
		default:
			assert(0);
			break;
		}
		return lRet;
	}
	long GetValueLong(const char* szKey, long* plValue, long lDefault = 0)
	{
		return GetValueLong("", szKey, plValue, lDefault);
	}

	long GetValueLong(const char* szSection, const char* szKey, long* plValue, long lDefault = 0)
	{
		std::string strValue;
		long lRet = GetValueString(szSection, szKey, &strValue);
		switch (lRet)
		{
		case ERR_NOERROR:
			*plValue = atol(strValue.c_str());
			break;
		case PROP_ERR_SECTION_NOT_FOUND:
			lRet = PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED;
			*plValue = lDefault;
			break;
		case PROP_ERR_KEY_NOT_FOUND:
			lRet = PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED;
			*plValue = lDefault;
			break;
		default:
			assert(0);
			break;
		}
		return lRet;
	}

	long GetValueFloat(const char* szKey, float* pfValue, float fDefault = 0)
	{
		return GetValueFloat("", szKey, pfValue, fDefault);
	}

	long GetValueFloat(const char* szSection, const char* szKey, float* pfValue, float fDefault = 0)
	{
		std::string strValue;
		long lRet = GetValueString(szSection, szKey, &strValue);
		switch (lRet)
		{
		case ERR_NOERROR:
			*pfValue = (float) atof(strValue.c_str());
			break;
		case PROP_ERR_SECTION_NOT_FOUND:
			lRet = PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED;
			*pfValue = fDefault;
			break;
		case PROP_ERR_KEY_NOT_FOUND:
			lRet = PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED;
			*pfValue = fDefault;
			break;
		default:
			assert(0);
			break;
		}
		return lRet;
	}


	long GetValueBool(const char* szKey, bool* pbValue, bool bDefault /*= false*/)
	{
		return GetValueBool("", szKey, pbValue, bDefault);
	}

	long GetValueBool(const char* szSection, const char* szKey, bool* pbValue, bool bDefault /*= false*/)
	{
		//first find the section
		long lRet = ERR_NOERROR;
		std::string strSection;

		if (!szSection || !*szSection)
		{
			strSection = "Empty Section";
		}
		else
		{
			strSection = szSection;
		}

		std::map<std::string, CPropertySection>::iterator iter = m_mapSections.find(strSection);

		if (iter != std::end(m_mapSections))
		{
			//Found!
			//Now get the property
			return iter->second.GetValueBool(szKey, pbValue, bDefault);
		}
		else
		{
			lRet = PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED;
			*pbValue = bDefault;
		}

		return lRet;
	}

	long GetValueSizeT(const char* szKey, size_t* psizeValue, size_t sizeDefault = 0)
	{
		return GetValueSizeT("", szKey, psizeValue, sizeDefault);
	}

	long GetValueSizeT(const char* szSection, const char* szKey, size_t* psizeValue, size_t sizeDefault = 0)
	{
		std::string strValue;
		long lRet = GetValueString(szSection, szKey, &strValue);
		switch (lRet)
		{
		case ERR_NOERROR:
			//*psizeValue = _strtoui64(strValue.c_str());
			sscanf(strValue.c_str(), "%zu", psizeValue);
			break;
		case PROP_ERR_SECTION_NOT_FOUND:
			lRet = PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED;
			*psizeValue = sizeDefault;
			break;
		case PROP_ERR_KEY_NOT_FOUND:
			lRet = PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED;
			*psizeValue = sizeDefault;
			break;
		default:
			assert(0);
			break;
		}
		return lRet;
	}

	long GetValueEnumVerb(const char* szKey, PROPERTYBAG_VERBS* peValue)
	{
		return GetValueEnumVerb("", szKey, peValue);
	}

	long GetValueEnumVerb(const char* szSection, const char* szKey, PROPERTYBAG_VERBS* peValue)
	{
		//first find the section
		long lRet = ERR_NOERROR;
		std::string strSection;

		if (!szSection || !*szSection)
		{
			strSection = "Empty Section";
		}
		else
		{
			strSection = szSection;
		}

		std::map<std::string, CPropertySection>::iterator iter = m_mapSections.find(strSection);

		if (iter != std::end(m_mapSections))
		{
			//Found!
			//Now get the property
			return iter->second.GetValueEnumVerb(szKey, peValue);
		}
		else
		{
			lRet = PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED;
		}

		return lRet;
	};

	long SetValueString(const char* szKey, const char* pszValue)
	{
		return SetValueString("", szKey, pszValue);
	}

	long SetValueString(const char* szSection, const char* szKey, const char* pszValue)
	{
		//first find the section
		long lRet = ERR_NOERROR;
		std::string strSection;

		if (!szSection || !*szSection)
			strSection = "Empty Section";
		else
		{
			strSection = szSection;
			trim(strSection);
		}

		m_mapSections[strSection].SetValueString(szKey, pszValue);

		return lRet;
	}

	long SetValueInt(const char* szKey, int iValue)
	{
		return SetValueInt("", szKey, iValue);
	}

	long SetValueInt(const char* szSection, const char* szKey, int iValue)
	{
		char szValue[32];
		sprintf_s(szValue, sizeof(szValue), "%i", iValue);
		return SetValueString(szSection, szKey, szValue);
	}
	long SetValueLong(const char* szKey, long lValue)
	{
		return SetValueLong("", szKey, lValue);
	}

	long SetValueLong(const char* szSection, const char* szKey, long lValue)
	{
		char szValue[32];
		sprintf_s(szValue, sizeof(szValue), "%li", lValue);
		return SetValueString(szSection, szKey, szValue);

	}

	long SetValueFloat(const char* szKey, float fValue)
	{
		return SetValueFloat("", szKey, fValue);
	}

	long SetValueFloat(const char* szSection, const char* szKey, float fValue)
	{
		char szValue[32];
		sprintf_s(szValue, sizeof(szValue), "%f", fValue);
		return SetValueString(szSection, szKey, szValue);

	}

	long SetValueBool(const char* szKey, bool bValue)
	{
		return SetValueBool("", szKey, bValue);
	}


	


	long SetValueBool(const char* szSection, const char* szKey, bool bValue)
	{
		if (bValue)
			return SetValueString(szSection, szKey, "True");
		else
			return SetValueString(szSection, szKey, "False");
	}

	long SetValueSizeT(const char* szKey, size_t sizeValue)
	{
		return SetValueSizeT("", szKey, sizeValue);
	}

	long SetValueSizeT(const char* szSection, const char* szKey, size_t sizeValue)
	{
		char szValue[32];
		sprintf_s(szValue, sizeof(szValue), "%zu", sizeValue);
		return SetValueString(szSection, szKey, szValue);

	}


	long SetValueEnumVerb(const char* szKey, PROPERTYBAG_VERBS eValue)
	{
		return SetValueEnumVerb("", szKey, eValue);
	};

	long SetValueEnumVerb(const char* szSection, const char* szKey, PROPERTYBAG_VERBS eValue)
	{
		return SetValueInt(szSection, szKey, eValue);

	}

};


#endif //_PROPERTY_BAG_H


////////////////////////////////////////////////
/////////////   CPropertyBag   /////////////////
////////////////////////////////////////////////

