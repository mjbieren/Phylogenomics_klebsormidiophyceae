#pragma once

#include "stdafx.h"
#include "Management.h"

class CGetPositiveDataSet : CPropertyBag
{
protected:
	char m_szGetPositiveDataSetErrMessage[255];
public:
	CGetPositiveDataSet() {};
	~CGetPositiveDataSet() {};
	const char* GetLastBlastSequenceGrabberErrorMessage() { return m_szGetPositiveDataSetErrMessage; }


	long SetColumnNumber(long lColumnNumber)
	{
		return SetValueLong("BlastOutput", "ColumnEvalue", lColumnNumber);
	}

	long GetColumnNumber(long* plColumnNumber)
	{
		return GetValueLong("BlastOutput", "ColumnEvalue", plColumnNumber);
	}

	long SetBlastFileExtension(const char* szBlastFileExtension)
	{
		return SetValueString("BlastOutput", "FileExtension", szBlastFileExtension);
	}

	long GetBlastFileExtension(std::string* pstrBlastFileExtension)
	{
		return GetValueString("BlastOutput", "FileExtension", pstrBlastFileExtension);
	}


	long SetFastaQuery(const char* szFastaQueryPath)
	{
		return SetValueString("Paths", "FastaQuery", szFastaQueryPath);
	}

	long GetFastaQuery(std::string* pstrFastaQueryPath)
	{
		return GetValueString("Paths", "FastaQuery", pstrFastaQueryPath);
	}

	long SetGenesPathSearch(const char* szGenesPath)
	{
		return SetValueString("Paths", "Genes", szGenesPath);
	}

	long GetGenesPathSearch(std::string* pstrGenesPathh)
	{
		return GetValueString("Paths", "Genes", pstrGenesPathh);
	}

	long SetBoolGeneFile(bool bGenesFileAdd = true)
	{
		return SetValueBool("Genes", "FileExist", bGenesFileAdd);
	}

	long GetBoolGeneFile(bool* pbGenesFileAdd)
	{
		long lReturn = ERR_NOERROR;
		lReturn =  GetValueBool("Genes", "FileExist", pbGenesFileAdd, false);

		if (lReturn == PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED)
		{
			printf_s("%s %s","GeneFile Bool Not Found: Default used", EOL);
			return ERR_NOERROR;
		}
		return lReturn;
	}


	long SetBlastFilePath(const char* szBlastPath)
	{
		return SetValueString("Paths", "Blast", szBlastPath);
	}

	long GetBlastFilePath(std::string* pstrBlastPath)
	{
		return GetValueString("Paths", "Blast", pstrBlastPath);
	}

	long SetOutputPath(const char* szOutputPath)
	{
		return SetValueString("Paths", "Output", szOutputPath);
	}

	long GetOutputPath(std::string* pstrOutputPath)
	{
		return GetValueString("Paths", "Output", pstrOutputPath);
	}

	long SetDecontaminationListPath(const char* szDecontaminationListPath)
	{
		return SetValueString("Paths", "Decontamination", szDecontaminationListPath);
	}

	long GetDecontaminationListPath(std::string* pstrDecontaminationListPath)
	{
		return GetValueString("Paths", "Decontamination", pstrDecontaminationListPath);
	}

	long SetStrainName(const char* szStrainName)
	{
		return SetValueString("Strain", "Name", szStrainName);
	}

	long GetStrainName(std::string* pstrStrainName)
	{
		return GetValueString("Strain", "Name", pstrStrainName);
	}

	long SetBoolStrictDecontamination(bool bStrictDecontamination = true)
	{
		return SetValueBool("Decontamination", "Strict", bStrictDecontamination);
	}

	long GetBoolStrictDecontamination(bool* pbStrictDecontamination)
	{
		long lReturn = ERR_NOERROR;
		lReturn = GetValueBool("Decontamination", "Strict", pbStrictDecontamination, false);
		if (lReturn == PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED)
		{
			printf_s("%s %s","Strict Decontamination Bool Not Found: Default used", EOL);
			return ERR_NOERROR;
		}

		return lReturn;

	}
};