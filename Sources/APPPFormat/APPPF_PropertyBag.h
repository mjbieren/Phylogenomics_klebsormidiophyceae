#pragma once
#include "stdafx.h"
#include "Management.h"

class CAPPPFPropertyBag : public CPropertyBag
{
	protected:
	char m_szGetOrthoGroupSequenceGrabberErrMessage[255];
public:
	CAPPPFPropertyBag() {};
	~CAPPPFPropertyBag() {};
	const char* GetApplyPhyloPyPrunerFormatErrorMessage() { return m_szGetOrthoGroupSequenceGrabberErrMessage; }

	/*
	Tree format (req)
	TreeDir (req)
	PathTaxonomicGroupFile (req)
	OutputFolder (req)
	bMoveMafftFile (optional)
	MoveMafftFilePath (optional)
	*/

	long SetTreeFormat(const char* szTreeFormat) //TreeFormat
	{
		return SetValueString("Tree", "Format", szTreeFormat);
	}

	long GetTreeFormat(std::string* pstrTreeFormat)
	{
		return GetValueString("Tree", "Format", pstrTreeFormat);
	}

	long SetTreeFilesPath(const char* szTreeFilesPath) //TreeDir
	{
		std::string strTreeFilesPath = szTreeFilesPath;
		AppendSlash(&strTreeFilesPath);
		return SetValueString("Paths", "Tree", strTreeFilesPath.c_str());

	}

	long GetTreeFilesPath(std::string* pstrTreeFilesPath)
	{
		return GetValueString("Paths", "Tree", pstrTreeFilesPath);
	}

	long SetTaxonomicGroupFilePath(const char* szTaxonomicGroupFilePath) //TaxonomicGroupFile
	{
		return SetValueString("Paths", "TaxonomicGroup", szTaxonomicGroupFilePath);
	}

	long GetTaxonomicGroupFilePath(std::string * pstrTaxonomicGroupFilePath)
	{
		return GetValueString("Paths", "TaxonomicGroup", pstrTaxonomicGroupFilePath);
	}

	long SetOutputFolder(const char* szOutputFolder) //Output Folder
	{
		std::string strOutputFolder = szOutputFolder;
		AppendSlash(&strOutputFolder);
		return SetValueString("Paths", "Output", strOutputFolder.c_str());
	}

	long GetOutputFolder(std::string* pstrOutputFolder)
	{
		return GetValueString("Paths", "Output", pstrOutputFolder);
	}


	long SetMoveMafftFiles() //bMoveMafftFile
	{
		return SetValueBool("Bool", "MoveMafftFiles", true);
	}

	long GetMoveMafftFile(bool* pbMoveMafftFiles)
	{
		return GetValueBool("Bool", "MoveMafftFiles", pbMoveMafftFiles, false);
	}

	long SetMoveMafftFilesPath(const char* szMoveMafftFilesPath) //MoveMafftFilePath
	{
		std::string strMafftFileFolder = szMoveMafftFilesPath;
		AppendSlash(&strMafftFileFolder);
		return SetValueString("Paths", "MoveMafft", strMafftFileFolder.c_str());
	}

	long GetMoveMafftFilesPath(std::string* pstrMoveMafftFilesPath)
	{
		return GetValueString("Paths", "MoveMafft", pstrMoveMafftFilesPath);
	}

};

