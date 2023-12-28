#pragma once

#include "stdafx.h"
#include "Management.h"

class CFPPPRPropertyBag :  public CPropertyBag
{
protected:
	char m_szGetFilterPhylopyPrunerResultErrMessage[255];
public:
	CFPPPRPropertyBag() {};
	~CFPPPRPropertyBag() {};
	const char* GetLastFilterPhylopyPrunerResultErrorMessage() { return m_szGetFilterPhylopyPrunerResultErrMessage; }

	/*
	-f Fasta Files path (PhylopyPruner output)
-t Taxonomic Group file Path
-n Taxonomic group Threshold Value
-r Output folder path
-s Summary File Path
	
	*/

	long SetTaxonomicGroupVectorSize(size_t sizeTaxonomicGroupsize)
	{
		return SetValueSizeT("TaxonomicGroup", "VectorSize", sizeTaxonomicGroupsize);
	}

	long GetTaxonomicGroupVectorSize(size_t* psizeTaxonomicGroup)
	{
		return GetValueSizeT("TaxonomicGroup", "VectorSize", psizeTaxonomicGroup, 0);
	}


	long SetFastaDirPath(const char* szFastaDirPath)
	{
		std::string strFastaPath = szFastaDirPath;
		AppendSlash(&strFastaPath); //This is to make sure the path ends with a /
		return SetValueString("Paths", "FastaFiles", strFastaPath.c_str());
	}

	long GetFastaDirPath(std::string* pstrFastaQueryPath)
	{
		return GetValueString("Paths", "FastaFiles", pstrFastaQueryPath);
	}

	long SetOrthoGroupDirPath(const char* szOGDirPath)
	{
		std::string strOGPath = szOGDirPath;
		AppendSlash(&strOGPath);
		return SetValueString("Paths", "OrthoGroups", strOGPath.c_str());
	}

	long GetOrthoGroupDirPath(std::string* pstrOrthoGroupsPath)
	{
		return GetValueString("Paths", "OrthoGroups", pstrOrthoGroupsPath);
	}

	long SetOutputFolderPath(const char* szOutputPath)
	{
		std::string strOutputPath = szOutputPath;
		AppendSlash(&strOutputPath);
		return SetValueString("Paths", "Output", strOutputPath.c_str());
	}

	long GetOutputFolderPath(std::string* pstrOutputPath)
	{
		return GetValueString("Paths", "Output", pstrOutputPath);
	}

	long SetOutputSummaryPath(const char* szOutputSummary)
	{
		std::string strOutputSummary = szOutputSummary;
		AppendSlash(&strOutputSummary);
		return SetValueString("Paths", "Summary", strOutputSummary.c_str());
	}

	long GetOutputSummary(std::string* pstrOutputSummary)
	{
		return GetValueString("Paths", "Summary", pstrOutputSummary);
	}

	long SetBoolSummary(bool bSetSummary = true)
	{
		return SetValueBool("Bool", "Summary", bSetSummary);
	}

	long GetBoolSummary(bool* pBGetSummary)
	{
		long lReturn = ERR_NOERROR;
		lReturn = GetValueBool("Bool", "Summary", pBGetSummary, false);
		if (lReturn == PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED)
		{
			printf_s("%s %s","Summary Bool Not Found: Default used",EOL);
			return ERR_NOERROR;
		}
		return lReturn;
	}

	long SetBoolNoHeader(bool bSetNoHeader = true)
	{
		return SetValueBool("Bool", "Header", bSetNoHeader);
	}


	long GetBoolNoHeader(bool* pbGetBoolNoHeader)
	{
		long lReturn = ERR_NOERROR;
		lReturn = GetValueBool("Bool", "Header", pbGetBoolNoHeader, false);
		if (lReturn == PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED || lReturn == PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED)
		{
			printf_s("%s %s","bGetNoHeader Bool Not Found: Default used",EOL);
			return ERR_NOERROR;
		}
		return lReturn;
	}


	long SetBoolNoGaps(bool bSetNoGaps = true)
	{
		return SetValueBool("Bool", "Gaps", bSetNoGaps);
	}

	long GetBoolNoGaps(bool* pbGetBoolNoGaps)
	{
		long lReturn = ERR_NOERROR;
		lReturn = GetValueBool("Bool", "Gaps", pbGetBoolNoGaps, false);
		if (lReturn == PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED || lReturn == PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED)
		{
			printf_s("%s %s","pbGetBoolNoGaps Bool Not Found: Default used",EOL);
			return ERR_NOERROR;
		}
		return lReturn;
	}


	long SetTaxonomicGroupFilePath(const char* szTaxonomicGroupFile)
	{
		return SetValueString("Paths", "TaxonomicGroupFile", szTaxonomicGroupFile);
	}

	long GetTaxonomicGroupFilePath(std::string* pstrTaxonomicGroupFile)
	{
		return GetValueString("Paths", "TaxonomicGroupFile", pstrTaxonomicGroupFile);
	}

	long SetTaxonomicGroupThreshold(long lTaxonomicGroupThreshold) 
	{
		//Check if the value is at least 1
		if (lTaxonomicGroupThreshold < 1)
			return PARSECOMMANDLINE_TAXONOMICTHRESHOLD_TOOLOW_FPPPR;

		return SetValueLong("Filter", "TaxonomicGroup", lTaxonomicGroupThreshold);
	}

	long GetTaxonomicGroupThreshold(long* plTaxonomicGroupThreshold)
	{
		return GetValueLong("Filter", "TaxonomicGroup", plTaxonomicGroupThreshold, 1);
	}

	long GetTaxonomicGroupThreshold(size_t * psizeTaxonomicGroupThreshold)
	{
		long lReturn = ERR_NOERROR;
		long lTreshold = 0;
		
		lReturn = GetValueLong("Filter", "TaxonomicGroup", &lTreshold, 1);
		if (lReturn != ERR_NOERROR)
			return lReturn;

		*psizeTaxonomicGroupThreshold = (size_t)lTreshold;

		return ERR_NOERROR;
		
	}
};

