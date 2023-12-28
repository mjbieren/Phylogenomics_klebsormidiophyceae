#include "GlobalHelper.h"

long SetFileBuffer(char** pPos, std::string strFileName);
long SortFastaFilesAlphabeticallyMatchingName(std::vector<CFastaFile*> *pvecFastaFiles);

long ParseOrthoGroups(std::vector<CTable*>* pvecOrthoGroups, size_t * pSizeOrthoGroups)
{
	//There are multiple orthogroup (.tsv files)
	/*
	Get A dir list of the tsv files
	Parse each individual tsv file into a table object
	push back the table object into the table vector.
	*/
	long lReturn = ERR_NOERROR;

	std::string strOrthoGroupFilePath;
	lReturn = glb.propertyBag.GetOrthoGroupDir(&strOrthoGroupFilePath);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_ORTHOGROUPDIR_OSG;

	std::vector<std::string> strDirList;

	size_t sizeOrthogroupDirList = GetFilteredDirList(strOrthoGroupFilePath.c_str(), "*.tsv", &strDirList);
	if (sizeOrthogroupDirList == 0)
	{
		printf_s("No orthogroup files in the folder %s %s", strOrthoGroupFilePath.c_str(), EOL);
		return DIRLIST_FAIL;
	}

	//We have now a list of file names in that folder time to parse the tables;

	std::string strFullPath;
	CTable* pOrthoGroupTable = nullptr;
	std::string strTableName;

	for (size_t idx = 0; idx < sizeOrthogroupDirList; idx++)
	{
		strTableName = strDirList.at(idx);
		strFullPath = strOrthoGroupFilePath + strTableName;
		pOrthoGroupTable = new CTable;
		pOrthoGroupTable->SetTableName(strTableName.c_str(), strTableName.c_str() + strTableName.length());
		printf_s("Working on Orthogroup file Nr.:%zu/%zu, %s %s",idx+1,sizeOrthogroupDirList, strDirList.at(idx).c_str(), EOL);
		lReturn = pOrthoGroupTable->ParseTableWithPath(strFullPath, 9, true); //They have headers, it's not a blast output file
		if (lReturn != ERR_NOERROR)
		{
			delete pOrthoGroupTable;
			return lReturn;
		}

		//We want to know the total amount of Orthogroups (for our summary file)
		*pSizeOrthoGroups += pOrthoGroupTable->GetAmountOfTableRows();
		//No Errors time to add the table into the vector
		pvecOrthoGroups->push_back(pOrthoGroupTable);

	}



	return lReturn;
}

long ParseFastaFiles(std::vector<CFastaFile*> *pvecFastaFilesSource, std::vector<std::string> *pvecDirList, size_t sizeDirList, std::string strFolderPath, long lCutOffStrainName, size_t * pIdxFastaFile)
{
	long lReturn = ERR_NOERROR;

	CFastaFile* pFasta = nullptr;
	
	std::string strFileFullPath;
	std::string strFastaFileName;

	for (size_t idx = 0; idx < sizeDirList; idx++)
	{
		pFasta = new CFastaFile;
		strFastaFileName = pvecDirList->at(idx);
		//We want to copy the fasta file name, but we do not want to add .fasta/.fa/etc so we remove that.
		lReturn = pFasta->SetFastaFileName(strFastaFileName.c_str(), (strFastaFileName.c_str() + (strFastaFileName.length() - lCutOffStrainName)));
		lReturn = pFasta->SetMatchingName(strFastaFileName.c_str(), (strFastaFileName.c_str() + (strFastaFileName.length() - lCutOffStrainName)));
		if (lReturn != ERR_NOERROR)
		{
			delete pFasta;
			return lReturn;
		}

		//Set the full path
		strFileFullPath = strFolderPath + strFastaFileName;
		//Parse the fasta file into the object
		printf_s("Working on Fasta File Nr:%zu %s %s", *pIdxFastaFile,strFastaFileName.c_str(), EOL);
		*pIdxFastaFile = *pIdxFastaFile + 1;
		lReturn = pFasta->ParseFastaFile(strFileFullPath.c_str());
		//Check if there is an error;
		if (lReturn != ERR_NOERROR)
		{
			delete pFasta;
			return lReturn;
		}
		//if not order the fasta file now
		lReturn = pFasta->SortFastaFileAlphabetically();
		if (lReturn != ERR_NOERROR)
		{
			delete pFasta;
			return lReturn;
		}

		//It's sorted alphabetically now and there are no errors, we can add the file into the vector.
		pvecFastaFilesSource->push_back(pFasta);
		//NExt one :)
	}
	return lReturn;

}
long ParseFastaFiles(std::vector<CFastaFile*>* pvecFastaFilesSource)
{
	//Parse all fasta files into fasta file object.
	/*
	1) Get the Path of the fasta files
	2) Get a dir list of all files in there, with .fasta, .fna, .ffn, .faa, .frn, .fa, .pep in them (7 in total).
	3) Check if there are some fasta files in them if not return error
	4) Parse the fasta files
	5) Order the Fasta files as well.
	
	*/
	long lReturn = ERR_NOERROR;

	size_t sizeFastaFilesTotal = 0;
	size_t sizeFastaDirList = 0;

	std::string strFastaFileDir;
	lReturn = glb.propertyBag.GetFastaDirSetbase(&strFastaFileDir);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETFASTADIR_OSG;


	//.FASTA
	std::vector<std::string> vecFastaFilesPaths;
	sizeFastaDirList = GetFilteredDirList(strFastaFileDir.c_str(), "*.fasta", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;

	size_t idxFasta = 1;

	if (sizeFastaDirList != 0) //There are fasta files wtih the format .fasta
	{
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 6, &idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();

	//.FNA
	sizeFastaDirList = GetFilteredDirList(strFastaFileDir.c_str(), "*.fna", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .fna
	{
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4,&idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();

	//.FFN
	sizeFastaDirList = GetFilteredDirList(strFastaFileDir.c_str(), "*.ffn", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .ffn
	{
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4,&idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();


	//.FAA
	sizeFastaDirList = GetFilteredDirList(strFastaFileDir.c_str(), "*.faa", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .faa
	{
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4, &idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();
	

	//.FRN
	sizeFastaDirList = GetFilteredDirList(strFastaFileDir.c_str(), "*.frn", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .frn
	{
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4, &idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();

	//.FA
	sizeFastaDirList = GetFilteredDirList(strFastaFileDir.c_str(), "*.fa", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .fa
	{
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 3, &idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();

	//.FAS
	sizeFastaDirList = GetFilteredDirList(strFastaFileDir.c_str(), "*.fas", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .fas
	{
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4, &idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();


	//.PEP
	sizeFastaDirList = GetFilteredDirList(strFastaFileDir.c_str(), "*.pep", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .pep
	{
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4, &idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();

	if (sizeFastaFilesTotal == 0)
		return DIRLIST_FAIL;


	//Sort the fasta files alphabetically
	lReturn = SortFastaFilesAlphabeticallyMatchingName(pvecFastaFilesSource);
	return lReturn;
}

long ParseOrthoGroupNameFromDirList(std::vector<std::string>* pvecOrthoGroupDirLost, std::vector<std::string*>* pvecOrthoGroupnameSet, size_t sizeCutOffStrainName)
{
	//First remove suffixes and order the vector, then use unique to remove all the duplicates

	long lReturn = ERR_NOERROR;

	std::vector<std::string> vecStringRemovedSuffixes;
	size_t sizeFastaFile = pvecOrthoGroupDirLost->size();
	std::string strIDX;

	size_t sizeOriginalString = 0;
	std::string strToSearch = "_pruned";
	size_t sizePosPruned = 0;
	//Remove the suffixes
	for (size_t idx = 0; idx < sizeFastaFile; idx++)
	{
		strIDX = pvecOrthoGroupDirLost->at(idx);
		sizeOriginalString = strIDX.size();
		strIDX.resize(sizeOriginalString - sizeCutOffStrainName);
		sizePosPruned = strIDX.find(strToSearch);
		if (sizePosPruned != std::string::npos)
		{
			//Filename has a _pruned in it.
			strIDX.resize(sizePosPruned);
		}
		vecStringRemovedSuffixes.push_back(strIDX);
	}

	//Sort the Vector
	std::sort(vecStringRemovedSuffixes.begin(), vecStringRemovedSuffixes.end());

	size_t sizeTotalFiles = vecStringRemovedSuffixes.size();

	//Remove the duplicates (because we do not want duplicated OrthoGroups in our files.
	auto it = std::unique(vecStringRemovedSuffixes.begin(), vecStringRemovedSuffixes.end());
	vecStringRemovedSuffixes.erase(it, vecStringRemovedSuffixes.end());

	size_t sizeFilesAfterRemovingDuplicates = vecStringRemovedSuffixes.size();

	printf_s("Started with %zu files, removed %zu due to duplicates %s", sizeTotalFiles, sizeTotalFiles - sizeFilesAfterRemovingDuplicates, EOL);

	//Now we add the file names to the real vector!.
	std::string* pstrString;
	size_t sizeFastaFilesSuffixes = vecStringRemovedSuffixes.size();
	for (size_t idx = 0; idx < sizeFastaFilesSuffixes; idx++)
	{
		pstrString = new std::string(vecStringRemovedSuffixes.at(idx).c_str());
		pvecOrthoGroupnameSet->push_back(pstrString);
	}

	vecStringRemovedSuffixes.clear();

	return lReturn;
}

long ParseOrthoGroupNamesFromPaths(std::vector<std::string*> * pvecOrthoGroupnameSet, const char* szPath)
{
	long lReturn = ERR_NOERROR;

	size_t sizeFastaDirList = 0;
	size_t sizeFastaFilesTotal = 0;

	//.FASTA
	std::vector<std::string> vecFastaFilesPaths;
	sizeFastaDirList = GetFilteredDirList(szPath, "*.fasta", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;

	lReturn = ParseOrthoGroupNameFromDirList(&vecFastaFilesPaths, pvecOrthoGroupnameSet, 6);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	vecFastaFilesPaths.clear();

	//.FNA
	sizeFastaDirList = GetFilteredDirList(szPath, "*.fna", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;

	lReturn = ParseOrthoGroupNameFromDirList(&vecFastaFilesPaths, pvecOrthoGroupnameSet, 4);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	vecFastaFilesPaths.clear();

	//.FFN
	sizeFastaDirList = GetFilteredDirList(szPath, "*.ffn", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;

	lReturn = ParseOrthoGroupNameFromDirList(&vecFastaFilesPaths, pvecOrthoGroupnameSet, 4);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	vecFastaFilesPaths.clear();

	//.FAA
	sizeFastaDirList = GetFilteredDirList(szPath, "*.faa", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;

	lReturn = ParseOrthoGroupNameFromDirList(&vecFastaFilesPaths, pvecOrthoGroupnameSet, 4);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	vecFastaFilesPaths.clear();

	//.FRN
	sizeFastaDirList = GetFilteredDirList(szPath, "*.frn", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;

	lReturn = ParseOrthoGroupNameFromDirList(&vecFastaFilesPaths, pvecOrthoGroupnameSet, 4);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	vecFastaFilesPaths.clear();

	//.FAS
	sizeFastaDirList = GetFilteredDirList(szPath, "*.fas", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;

	lReturn = ParseOrthoGroupNameFromDirList(&vecFastaFilesPaths, pvecOrthoGroupnameSet, 4);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	vecFastaFilesPaths.clear();

	//.PEP
	sizeFastaDirList = GetFilteredDirList(szPath, "*.pep", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;

	lReturn = ParseOrthoGroupNameFromDirList(&vecFastaFilesPaths, pvecOrthoGroupnameSet, 4);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	vecFastaFilesPaths.clear();

	//.FA
	sizeFastaDirList = GetFilteredDirList(szPath, "*.fa", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;

	lReturn = ParseOrthoGroupNameFromDirList(&vecFastaFilesPaths, pvecOrthoGroupnameSet, 3);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	vecFastaFilesPaths.clear();


	return lReturn;
}

long SortOrthoGroupNameVector(std::vector<std::string*> *pvecOrthoGroupNameVector)
{
	long lReturn = ERR_NOERROR;
	std::sort(pvecOrthoGroupNameVector->begin(), pvecOrthoGroupNameVector->end(), [](std::string* a, std::string* b)
		{
			int iRes = strcasecmp(a->c_str(), b->c_str());
			return iRes < 0;
		});

	return lReturn;
}

long FilterOrthoGroupnames(std::vector<std::string*>* pvecOrthoGroupNamesFromSetOneAndTwo, std::vector<std::string*>* pvecOrthoGroupNamesFromSetOneAndTwoFiltered)
{
	long lReturn = ERR_NOERROR;

	size_t sizeOrthoGroupNamesSetOneAndTwo = pvecOrthoGroupNamesFromSetOneAndTwo->size();
	std::string * pstrOld;
	std::string * pstrNew;

	//Add the first one already
	pvecOrthoGroupNamesFromSetOneAndTwoFiltered->push_back(pvecOrthoGroupNamesFromSetOneAndTwo->at(0));
	pstrOld = pvecOrthoGroupNamesFromSetOneAndTwo->at(0);

	//Go over the rest
	for (size_t idx = 1; idx < sizeOrthoGroupNamesSetOneAndTwo; idx++) //we start at 1 because we already added the first one!
	{
		pstrNew = pvecOrthoGroupNamesFromSetOneAndTwo->at(idx);
		if (memcmp(pstrOld->c_str(), pstrNew->c_str(), pstrNew->size()) == 0)
		{
			//They are equal so we continue witht he next in the vector
			continue;
		}

		//They are NOT equal!
		pvecOrthoGroupNamesFromSetOneAndTwoFiltered->push_back(pstrNew);
		//Replace old with the new one
		pstrOld = pstrNew;
	}

	size_t sizeFilteredVector = pvecOrthoGroupNamesFromSetOneAndTwoFiltered->size();

	printf_s("Started with %zu OrthoGroup Files, Removed %zu files due to duplicates %s", sizeOrthoGroupNamesSetOneAndTwo, sizeOrthoGroupNamesSetOneAndTwo - sizeFilteredVector, EOL);

	return lReturn;
}


long ParseOrthoGroupNamesFromSetOneAndSetTwo(std::vector<std::string*>* pvecOrthoGroupNamesFromSetOneAndTwo, std::vector<std::string*>* pvecOrthoGroupNamesFromSetOneAndTwoFiltered)
{
	/*
	1) Get Filtered dir list for both the first and second list
	2) Remove the _pruned ones from the list
	3) Remove duplicates

	
	*/

	long lReturn = ERR_NOERROR;
	
	std::string strPathSetOne;
	lReturn = glb.propertyBag.GetFastaDirSetOne(&strPathSetOne);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_FASTAFILE_ONE_COGS;

	lReturn = ParseOrthoGroupNamesFromPaths(pvecOrthoGroupNamesFromSetOneAndTwo, strPathSetOne.c_str());
	if (lReturn != ERR_NOERROR)
		return lReturn;


	std::string strPathSetTwo;
	lReturn = glb.propertyBag.GetFastaDirSetTwo(&strPathSetTwo);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_FASTAFILE_TWO_COGS;

	lReturn = ParseOrthoGroupNamesFromPaths(pvecOrthoGroupNamesFromSetOneAndTwo, strPathSetTwo.c_str());
	if (lReturn != ERR_NOERROR)
		return lReturn;

	//Now Sort the vector
	lReturn = SortOrthoGroupNameVector(pvecOrthoGroupNamesFromSetOneAndTwo);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	//Now add only the unique ones
	lReturn = FilterOrthoGroupnames(pvecOrthoGroupNamesFromSetOneAndTwo, pvecOrthoGroupNamesFromSetOneAndTwoFiltered);
	if (lReturn != ERR_NOERROR)
		return lReturn;


	return lReturn;
}


long ParseInputFiles(std::vector<CFastaFile*>* pvecFastaFilesSource, std::vector<CTable*>* pvecOrthoGroups, size_t* psizeOrthoGroups, std::vector<std::string*>* pvecOrthoGroupNamesFromSetOneAndTwo, std::vector<std::string*>* pvecOrthoGroupNamesFromSetOneAndTwoFiltered)
{
	long lReturn = ERR_NOERROR;

	//Fasta Files
	printf_s("Parsing all the fasta files into the Fasta File Vector %s", EOL);
	lReturn = ParseFastaFiles(pvecFastaFilesSource);
	if (lReturn != ERR_NOERROR)
		return lReturn;
	printf_s("Done with parsing all the fasta files in to fasta file vector %s", EOL);

	//OrthoGroups
	printf_s("Parsing all the orthogroup files into orthogroup objects %s", EOL);
	lReturn = ParseOrthoGroups(pvecOrthoGroups, psizeOrthoGroups);
	if (lReturn != ERR_NOERROR)
		return lReturn;
	printf_s("Done with parsing all the orthogroup files %s", EOL);


	//FastaSetNames
	printf_s("Time to parse the two OrthoGroup Fasta sets %s", EOL);
	lReturn = ParseOrthoGroupNamesFromSetOneAndSetTwo(pvecOrthoGroupNamesFromSetOneAndTwo, pvecOrthoGroupNamesFromSetOneAndTwoFiltered);
	if (lReturn != ERR_NOERROR)
		return lReturn;
	printf_s("Done with parsing the two orthogorup fasta sets, only uniques are left and they are alphabetically ordered. %s", EOL);


	return lReturn;
}