#pragma once
#include "stdafx.h"
#include "Management.h"


class CCOGSPropertyBag : CPropertyBag
{
protected:
	char m_szGetCombineOrthoGroupSetsErrMessage[255];
public:
	CCOGSPropertyBag() {};
	~CCOGSPropertyBag() {};
	const char* GetLastCombineOrthogroupSetsErrorMessage() { return m_szGetCombineOrthoGroupSetsErrMessage; }

	/*
	Fasta File Input Set 1
	Fasta File Input Set 2
	Fastas File Base data
	Orthogroup Dir
	PhylopyPruner Format

	*/

	long SetFastaDirSetOne(const char* szFastaDirPath)
	{
		std::string strFastaPath = szFastaDirPath;
		AppendSlash(&strFastaPath); //This is to make sure the path ends with a /
		return SetValueString("Paths", "FastaFilesOne", strFastaPath.c_str());
	}

	long GetFastaDirSetOne(std::string* pstrFastaDirSetOne)
	{
		return GetValueString("Paths", "FastaFilesOne", pstrFastaDirSetOne);
	}

	long SetFastaDirSetTwo(const char* szFastaDirPath)
	{
		std::string strFastaPath = szFastaDirPath;
		AppendSlash(&strFastaPath); //This is to make sure the path ends with a /
		return SetValueString("Paths", "FastaFilesTwo", strFastaPath.c_str());
	}

	long GetFastaDirSetTwo(std::string* pstrFastaDirSetTwo)
	{
		return GetValueString("Paths", "FastaFilesTwo", pstrFastaDirSetTwo);
	}

	long SetFastaDirSetBase(const char* szFastaDirPath)
	{
		std::string strFastaPath = szFastaDirPath;
		AppendSlash(&strFastaPath); //This is to make sure the path ends with a /
		return SetValueString("Paths", "FastaFilesBase", strFastaPath.c_str());
	}

	long GetFastaDirSetbase(std::string* pstrFastaDirSetBase)
	{
		return GetValueString("Paths", "FastaFilesBase", pstrFastaDirSetBase);
	}

	long SetOrthoGroupDir(const char* szOrthoGroupDir)
	{
		std::string strOrthoGroupDir = szOrthoGroupDir;
		AppendSlash(&strOrthoGroupDir); //This is to make sure the path ends with a /
		return SetValueString("Paths", "OrthoGroup", strOrthoGroupDir.c_str());
	}

	long GetOrthoGroupDir(std::string* pstrOrthoGroupDir)
	{
		return GetValueString("Paths", "OrthoGroup", pstrOrthoGroupDir);
	}

	long SetOutputDir(const char* szOutputDir)
	{
		std::string strOrthoGroupDir = szOutputDir;
		AppendSlash(&strOrthoGroupDir); //This is to make sure the path ends with a /
		return SetValueString("Paths", "Output", strOrthoGroupDir.c_str());
	}

	long GetOutputDir(std::string* pstrOutputDir)
	{
		return GetValueString("Paths", "Output", pstrOutputDir);
	}

	long SetBoolPhyloPyPruner(bool bSetPhyloPyPrunerFormat = true)
	{
		return SetValueBool("Bool", "PhyloPyPruner", bSetPhyloPyPrunerFormat);
	}

	long GetBoolPhyloPyPruner(bool* pbGetPhyloPyPrunerFormat)
	{
		long lReturn = ERR_NOERROR;
		lReturn = GetValueBool("Bool", "PhyloPyPruner", pbGetPhyloPyPrunerFormat, false);
		if (lReturn == PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED || lReturn == PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED)
		{
			printf_s("%s %s", "pbGetPhyloPyPrunerFormat Bool Not Found: Default used", EOL);
			return ERR_NOERROR;
		}
		return lReturn;
	}

};