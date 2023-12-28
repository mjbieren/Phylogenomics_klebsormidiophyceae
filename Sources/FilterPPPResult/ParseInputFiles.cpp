#include "GlobalHelper.h"

long SetFileBuffer(char** pPos, std::string strFileName);

long ParseTaxonomicGroup(CTaxonomicGroup* pTaxonomicGroup, const char* pStart, const char* pEnd)
{
	//SpeciesName(HeaderName),SpeciesName2(HeaderName2),etc, EOL			#group 1
	
	long lReturn = ERR_NOERROR;

	const char* pPosStart = pStart;
	const char* pPos = pStart;
	const char* pPosEnd = pStart;

	CTaxonomicSpecies* pSpecies = nullptr;

	while (pPosEnd != pEnd)
	{
		while (*pPosEnd != ',' && pPosEnd != pEnd)		
			pPosEnd++;

		if (pPosEnd == pEnd) //Found the last one
			{
				//To make sure we don't end with a , at the end aka "SpeciesName(HeaderName),SpeciesName2(HeaderName2),"
				while (*pPos != '(' && pPos != pEnd)
					pPos++;

				if(pPos == pEnd) //Ended with a , at the end and not a header/species name.
					break;

				//Didn't end with a , instead of a ( meaning a headername start.
				pSpecies = new CTaxonomicSpecies;
				lReturn = pSpecies->SetSpeciesName(pPosStart, pPos - pPosStart);
				if (lReturn != ERR_NOERROR)
				{
					delete pSpecies;
					return lReturn;
				}


				pPosStart = pPos + 1; //we want to end it with 1 further than the (.
				
				while (*pPos != ')' && pPos != pEnd)
					pPos++;

				if (pPos == pEnd)
					{
						delete pSpecies;
						return PARSE_INPUT_FILE_TAXONOMICGROUPFILE_WRONG_FORMAT;//Error cuz we didn't end it at the right one.
					}

				lReturn = pSpecies->SetHeaderName(pPosStart, pPos - pPosStart);
				if (lReturn != ERR_NOERROR)
				{
					delete pSpecies;
					return lReturn;
				}
					
				//Push Back the taxonomic group
				pTaxonomicGroup->PushBack(pSpecies);

			}
		else //It's not the end
		{
			while (*pPos != '(' && pPos != pPosEnd)
				pPos++;
			
			if (pPos == pPosEnd)
				return PARSE_INPUT_FILE_TAXONOMICGROUPFILE_WRONG_FORMAT;
			
			pSpecies = new CTaxonomicSpecies;
			lReturn = pSpecies->SetSpeciesName(pPosStart, pPos - pPosStart);
			if (lReturn != ERR_NOERROR)
				{
					delete pSpecies;
					return lReturn;
				}

			pPosStart = pPos + 1; //we want to end it with 1 further than the (.

			while (*pPos != ')' && pPos != pPosEnd)
				pPos++;

			if (pPos == pPosEnd)
				return PARSE_INPUT_FILE_TAXONOMICGROUPFILE_WRONG_FORMAT;

			lReturn = pSpecies->SetHeaderName(pPosStart, pPos - pPosStart);
			if (lReturn != ERR_NOERROR)
				{
					delete pSpecies;
					return lReturn;
				}

			//Set them all time to push it back in the taxonomic group
			
			pTaxonomicGroup->PushBack(pSpecies);


			//move the pointers
			pPosEnd++;
			pPosStart = pPosEnd;
			pPos = pPosEnd;

		}



	}

	return lReturn;
}

long ParseTaxonomicGroups(std::vector<CTaxonomicGroup*> * pvecTaxonomicGroup, CTaxonomicGroup* pTaxonomicGroupsSorted)
{
	//This function parses the taxonomic group file into different taxonomic group objects and then add all the species into one taxonomic group sorted alphabetically.
	/*
	1) Parse the taxonomic group file into different taxonomic groups. 1 line = 1 group and push it back into one taxonomic group vector
	2) Set the idx of all taxanomic species to the same value in one taxonomic group.
	3) Parse all taxonomic species of each group into one taxanomic group object (everything in one object).
	4) Sort the taxonomic species vector within the taxonomic group sorted object alphabetically

	SpeciesName(HeaderName),SpeciesName2(HeaderName2),etc, EOL			#group 1
	SpeciesName3(HeaderName3),SpeciesName4(HeaderName4), etc, EOL		#group 2

	Every line is 1 HeadersFilters belonging to 1 group, every species within it belongs to 1 headerFilter.
	
	*/
	long lReturn = ERR_NOERROR;

	std::string strTaxonomicGroupFile;

	lReturn = glb.propertyBag.GetTaxonomicGroupFilePath(&strTaxonomicGroupFile);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_TAXONOMICGROUPFILE_FPPPR;

	//Get the file buffer;
	char* pBuffer = nullptr;
	lReturn = SetFileBuffer(&pBuffer, strTaxonomicGroupFile);
	CSmartPtr<char> pAutoDelete = pBuffer;
	if (lReturn != ERR_NOERROR)
		return lReturn;

	char* pPos = pBuffer;
	char* pEnd = pBuffer;

	CTaxonomicGroup* pTaxonomicGroup = nullptr;


	//STEP ONE
	while (*pPos)
	{
		MOVE_PTR_TO_EOL_REAL(pEnd); //Looking at EOL or EOF. But not the one before
		pTaxonomicGroup = new CTaxonomicGroup;
		lReturn = ParseTaxonomicGroup(pTaxonomicGroup, pPos, pEnd);
		if (lReturn != ERR_NOERROR)
		{
			delete pTaxonomicGroup;
			return lReturn;
		}

		pvecTaxonomicGroup->push_back(pTaxonomicGroup);
		MOVE_PTR_TO_BOL(pEnd);
		pPos = pEnd;

	}

	//STEP TWO & STEP THREE
	size_t sizeTaxonomicGroupVector = pvecTaxonomicGroup->size();

	//Now we also set the taxonomic groups vector size in the property bag, since we need it later for filtering purposes.
	lReturn = glb.propertyBag.SetTaxonomicGroupVectorSize(sizeTaxonomicGroupVector);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_SETTAXONOMICGROUPSIZE_VECTOR_FPPPR;

	size_t sizeTaxonomicSpecies = 0;
	CTaxonomicSpecies* pSpecies = nullptr;

	for (size_t idx = 0; idx < sizeTaxonomicGroupVector; idx++)
	{
		pTaxonomicGroup = pvecTaxonomicGroup->at(idx);
		pTaxonomicGroup->SetDeleteTaxonomicSpecies(); //Also set that we delete the items, since this is the original.
		sizeTaxonomicSpecies = pTaxonomicGroup->GetVectorSize();
		for (size_t idx2 = 0; idx2 < sizeTaxonomicSpecies; idx2++)
		{
			pSpecies = pTaxonomicGroup->GetSpeciesVectorItem(idx2);
			pSpecies->SetFilterNumber(idx);
			pTaxonomicGroupsSorted->PushBack(pSpecies);
		}
	}



	//STEP FOUR
	lReturn = pTaxonomicGroupsSorted->SortTaxonomicGroupAlphabeticallyHeaders();
	return lReturn;
}


long ParseFastaFiles(std::vector<CFastaFile*> *pvecFastaFilesSource, std::vector<std::string> *pvecDirList, size_t sizeDirList, std::string strFolderPath, long lCutOffStrainName, size_t * pIdxFastaFile, bool bRemoveAlignments)
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

		if (bRemoveAlignments == true)
		{
			lReturn = pFasta->RemoveGapsFromFastaBlocks();
			if (lReturn != ERR_NOERROR)
			{
				delete pFasta;
				return lReturn;
			}
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
	lReturn = glb.propertyBag.GetFastaDirPath(&strFastaFileDir);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETFASTADIR_FPPPR;


	bool bRemoveAlignments = false;
	lReturn = glb.propertyBag.GetBoolNoGaps(&bRemoveAlignments);
	if (lReturn != ERR_NOERROR)
		return lReturn;


	//.FASTA
	std::vector<std::string> vecFastaFilesPaths;
	sizeFastaDirList = GetFilteredDirList(strFastaFileDir.c_str(), "*.fasta", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;

	size_t idxFasta = 1;

	if (sizeFastaDirList != 0) //There are fasta files wtih the format .fasta
	{
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 6, &idxFasta,bRemoveAlignments);
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
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4,&idxFasta,bRemoveAlignments);
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
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4,&idxFasta,bRemoveAlignments);
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
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4, &idxFasta,bRemoveAlignments);
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
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4, &idxFasta,bRemoveAlignments);
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
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 3, &idxFasta,bRemoveAlignments);
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
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4, &idxFasta,bRemoveAlignments);
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
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4, &idxFasta,bRemoveAlignments);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();

	std::vector<std::string> vecFastaFilesPathsPEP;


	if (sizeFastaFilesTotal == 0)
		return DIRLIST_FAIL;

	return lReturn;
}


long ParseInputFiles(std::vector<CFastaFile*>* pvecFastaFilesSource, std::vector<CTaxonomicGroup*> * pvecTaxonomicGroupSource, CTaxonomicGroup* pTaxonomicGroupsSorted)
{
	/*This function parses all the different input files which include :
	The Fasta Files
	TaxonomicGroup File
	and sort the Taxonomic Group file into one big taxonomic group object, for easy search later for the matching.

	*/
	long lReturn = ERR_NOERROR;

	//Fasta Files
	printf_s("Parsing all the fasta files into the Fasta File Vector %s", EOL);
	lReturn = ParseFastaFiles(pvecFastaFilesSource);
	if (lReturn != ERR_NOERROR)
		return lReturn;
	printf_s("Done with parsing all the fasta files in to fasta file vector %s", EOL);

	//TaxonomicGroup
	printf_s("Parsing the taxonomic group file %s", EOL);
	lReturn = ParseTaxonomicGroups(pvecTaxonomicGroupSource, pTaxonomicGroupsSorted);
	if (lReturn != ERR_NOERROR)
		return lReturn;
	printf_s("Done with parsing the taxonomic group file %s", EOL);



	return lReturn;
}