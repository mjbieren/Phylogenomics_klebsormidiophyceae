#include "GlobalHelper.h"
long SetFileBufferReuse(char** ppExistingBuffer, std::string strFileName, size_t* pSizeBuffer);


long GetListTreefFiles(std::vector<std::string>* pvecStringOfFiles)
{
	long lReturn = ERR_NOERROR;

	std::string strInputPath;
	std::string strFileFormat;
	size_t sizeFoundFiles =0 ;

	lReturn = glb.propertyBag.GetTreeFilesPath(&strInputPath);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETTREEDIRPATH_APPPF;

	lReturn = glb.propertyBag.GetTreeFormat(&strFileFormat);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETTREEFORMAT_APPPF;


	std::string strNewFileFormat;

	//Check if there is a . within the file format e.g .fasta and not fasta
	const char* pstrFileFormat = strFileFormat.c_str();
	if (pstrFileFormat[0] == 46) // 46 = .
	{
		//Yes it has a dot.
		strNewFileFormat = "*" + strFileFormat;
		sizeFoundFiles = GetFilteredDirList(strInputPath.c_str(), strNewFileFormat.c_str(), pvecStringOfFiles);
		if (sizeFoundFiles == ERR_NOERROR)
			return DIRLIST_FAIL;
	}
	else
	{
		//No it does not have a dot

		//Add the dot
		strNewFileFormat = "*." + strFileFormat;
		sizeFoundFiles = GetFilteredDirList(strInputPath.c_str(), strNewFileFormat.c_str(), pvecStringOfFiles);
		if (sizeFoundFiles == ERR_NOERROR)
			return DIRLIST_FAIL;

		//Also replace the old treeformat!
		lReturn = glb.propertyBag.SetTreeFormat(strNewFileFormat.c_str());
		if (lReturn != ERR_NOERROR)
			return PROPERTYBAG_SETTREEFORMAT_APPPF;

	}

	if (pvecStringOfFiles->size() == 0)
	{
		printf("No files were found with %s .%s", strNewFileFormat.c_str(), EOL);
		return DIRLIST_FAIL;
	}

	return lReturn;
}


long ReformatTreeSpecies(char* pStart, char* pEnd, CTaxonomicGroup* pTaxonomicGroupSorted)
{
	long lReturn = ERR_NOERROR;

	//Data between pStart and pEnd = SAG_2147_TRINITY_DN21301_c0_g1.p1:. We want to replace it towards = SAG_2147@TRINITY_DN21301_c0_g1.p1:. It can also be SAG_2147-1200:


	/*
	STEP 1) Find which CTaxonomicSpecies it is
	STEP 2) Based on the length of the CTaxonomicSpecies HeaderName we then know where have to replace the @ into.
	*/

	CTaxonomicSpecies* pTaxonomicSpecieToFind = nullptr;

	size_t sizeHeaderBuffer = pEnd - pStart + 10;

	char* szHeaderNameToCheck = new char[sizeHeaderBuffer];
	CSmartPtr<char> pAutoDelete = szHeaderNameToCheck;
	memset(szHeaderNameToCheck, '\0', sizeHeaderBuffer);
	memcpy(szHeaderNameToCheck, pStart, pEnd - pStart);

	//STEP ONE
	lReturn = pTaxonomicGroupSorted->FindTaxonomicSpeciesFromHeaderNameSource(szHeaderNameToCheck, &pTaxonomicSpecieToFind);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	
	//STEP TWO
	pStart[strlen(pTaxonomicSpecieToFind->GetHeaderName())] = '@';

	return lReturn;
}

long ReformatTreeFile(std::string strTreeFilePath, CTaxonomicGroup* pTaxonomicGroupSorted, std::string strTreeFileName, std::string strOutputDir, size_t * psizeBuffer, char ** ppBuffer, std::string strTreeFormat)
{
	//This function reformats all the species within the tree.
	/*
	STEP 1) Get the File buffer
	STEP 2) 

	*/
	long lReturn = ERR_NOERROR;

	lReturn = SetFileBufferReuse(ppBuffer, strTreeFilePath, psizeBuffer);
	if (lReturn != ERR_NOERROR)
	{
		printf_s("Could not set the File Buffer for file %s %s", strTreeFileName.c_str(), EOL);
		return lReturn;
	}

	char* pBuffer = *ppBuffer;

	//Set the variables needed to go over the file buffer;
	char* pPos = pBuffer;
	char* pEnd = pBuffer;

	//(SAG_2147_TRINITY_DN21301_c0_g1.p1:0.0910688241,(SAG_2147_TRINITY_DN30637_c0_g1.p1:0.0773952761));
	while (*pEnd || *pEnd != SEMICOLON_ASCII) //Newick formatted trees end with a ;
	{
		MOVE_PTR_TO_COLON(pEnd); //pEnd now looks at :0.0910688241,(SAG_2147_TRINITY_DN30637_c0_g1.p1:0.0773952761));
		pPos = pEnd; //pPos also looks at :0.0910688241,(SAG_2147_TRINITY_DN30637_c0_g1.p1:0.0773952761));
		MOVE_PTR_TO_START_OF_TREE_NODE(pPos); //(SAG_2147_TRINITY_DN21301_c0_g1.p1:0.0910688241,(SAG_2147_TRINITY_DN30637_c0_g1.p1:0.0773952761));
		pPos++; //Now looks at SAG_2147_TRINITY_DN21301_c0_g1.p1:0.0910688241,(SAG_2147_TRINITY_DN30637_c0_g1.p1:0.0773952761));
		if (*pEnd == ':')
		{
			if (pEnd[-1] != ROUND_BRACKET_CLOSE_ASCII)
			{
				lReturn = ReformatTreeSpecies(pPos, pEnd, pTaxonomicGroupSorted); //The data between pPos and pEnd = SAG_2147_TRINITY_DN21301_c0_g1.p1:
				if (lReturn != ERR_NOERROR)
				{
					if(lReturn != TAXONOMIC_GROUP_FIND_SPECIES_HEADERNAME) //We can have branches in the tree that are shown with e.g. )99:0.25). But the 99 is not a specie, but it isn't wrong either!
						return lReturn;
					else
					{
						lReturn = ERR_NOERROR;
					}
				}
			}
			pPos = pEnd;
		}
		else //e.g. the 2nd )) in SAG_2147_TRINITY_DN30637_c0_g1.p1:0.0773952761
			break;


		pEnd++;
		MOVE_PTR_TO_COLON(pEnd); 
		MOVE_PTR_TO_START_OF_TREE_NODE(pEnd);//To set it already at the start of the new species and skip branch spaces.
	}

	//At this point we have reformatted the entire buffer (file).
	/*
	Time to write it to the output file.
	however the file names are probably looking now like this:
	OrthogroupName.mafft.treefile.
	What we want is OrthoGroupName.treefile


	*/

	
	std::string strNameChange = strTreeFileName;
	std::string strExtensionToFind = ".mafft" + strTreeFormat;
	size_t sizeidxExtension = strNameChange.find(strExtensionToFind);

	if(sizeidxExtension != (size_t) -1) // if it doesn't return an npos (aka not found) replace the .mafft.treefile -> .treefile
		strNameChange.replace(sizeidxExtension, strExtensionToFind.length(), ".treefile");

	std::string strFullFilePathToWrite = strOutputDir + strNameChange;


	//Write the output file

	FILE* hFileTreeFile;
	errno_t err = fopen_s(&hFileTreeFile, strFullFilePathToWrite.c_str(), "wb");
	if (err)
	{
		char szError[SIZE_ERRORBUFFER];
		sprintf_s(szError, sizeof(szError), "Error ERR_FOPEN_TREEFILE_FILE_WRITE on fopen_s() with err: %i%s", err, EOL);
		printf_s("%s", szError);

		return WRITE_OUTPUT_FILE_ERROR_FOPEN;

	}
	else
	{
		//Place the full content of strTotalFasta into the new file and close it
		fputs(*ppBuffer, hFileTreeFile);
	}
	if (hFileTreeFile != 0)
		fclose(hFileTreeFile);

	return lReturn;
}

long ReformatTreeFiles(CTaxonomicGroup* pTaxonomicGroupSorted)
{
	//This function reformats all the tree files to the PPP format

	long lReturn = ERR_NOERROR;

	std::string strOutputDirectory;
	lReturn = glb.propertyBag.GetOutputFolder(&strOutputDirectory);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETTAXONOMICGROUPFILE_APPPF;

	std::string strInputDirTrees;
	lReturn = glb.propertyBag.GetTreeFilesPath(&strInputDirTrees);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETTREEDIRPATH_APPPF;

	std::vector<std::string> vecTreeFileList;
	lReturn = GetListTreefFiles(&vecTreeFileList);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	size_t sizeTreeFiles = vecTreeFileList.size();

	std::string strTreeFile;
	std::string strTreeFileFullPath;

	//We want to reuse one file buffer, since we do not want to do a new for every file (roughly 15k files)
	size_t sizeBuffer = SIZE_BUFFER_HUNDRED_MB;
	char* pBuffer = new char[sizeBuffer];
	memset(pBuffer, '\0', sizeBuffer);

	std::string strTreeFormat;
	lReturn = glb.propertyBag.GetTreeFormat(&strTreeFormat);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETTREEFORMAT_APPPF;

	//But for the replacing of the name we do not want .mafft*.treefile
	strTreeFormat.replace(strTreeFormat.find("*"),1, "");

	for (size_t idx = 0; idx < sizeTreeFiles; idx++)
	{
		strTreeFile = vecTreeFileList.at(idx);
		strTreeFileFullPath = strInputDirTrees +strTreeFile;
		printf_s("Working on treefile %s, file number:%zu/%zu %s", strTreeFile.c_str(),idx+1, sizeTreeFiles, EOL);
		lReturn = ReformatTreeFile(strTreeFileFullPath, pTaxonomicGroupSorted, strTreeFile, strOutputDirectory, &sizeBuffer, &pBuffer, strTreeFormat);
		if (lReturn != ERR_NOERROR)
		{
			delete pBuffer;
			return lReturn;
		}


	}

	delete pBuffer;

	return lReturn;
}