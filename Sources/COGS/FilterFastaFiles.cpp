#include "GlobalHelper.h"

long FindMatchingFastaFileFileName(std::vector<CFastaFile*>* pvecFastaFiles, char* pszFileName, CFastaFile** ppFoundFastaFile);

long Filtering(std::vector<CFastaFile*>* pvecOrthoGroups, std::vector<CFastaFile*>* pvecOrthoGroupsFiltered, std::vector<std::string*>* pvecOrthoGroupNamesFromSetOneAndTwo)
{ 
	long lReturn = ERR_NOERROR;

	size_t sizeFilesToFind = 0;
	sizeFilesToFind = pvecOrthoGroupNamesFromSetOneAndTwo->size();

	std::string strFileNametoFind;
	CFastaFile* pFastaFileToFind = nullptr;

	for (size_t idx = 0; idx < sizeFilesToFind; idx++)
	{
		strFileNametoFind = *pvecOrthoGroupNamesFromSetOneAndTwo->at(idx);

		lReturn = FindMatchingFastaFileFileName(pvecOrthoGroups, (char*)strFileNametoFind.c_str(), &pFastaFileToFind);
		if (lReturn != ERR_NOERROR)
			return lReturn;

		pvecOrthoGroupsFiltered->push_back(pFastaFileToFind);
	}

	return lReturn;
}