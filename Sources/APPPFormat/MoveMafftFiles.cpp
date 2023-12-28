#include "GlobalHelper.h"

long SetFileBufferReuse(char** ppExistingBuffer, std::string strFileName, size_t* pSizeBuffer);

long MoveMafftFile(std::string strFileName, std::string strDirPath, std::string strOutput, char** ppBuffer, size_t* psizeBuffer)
{
	//Set the File buffer from the input
	//Write an output based on the output path
	long lReturn = ERR_NOERROR;

	std::string strFileNameNew = strFileName;
	strFileNameNew.replace(strFileNameNew.find(".mafft"),6, "");
	std::string strFullPathInput = strDirPath + strFileNameNew + ".mafft";
	std::string strFullPathOutput = strOutput + strFileNameNew + ".fa";

	lReturn = SetFileBufferReuse(ppBuffer, strFullPathInput, psizeBuffer);
	if (lReturn != ERR_NOERROR)
		return lReturn;


	FILE* hFileTreeFile;
	errno_t err = fopen_s(&hFileTreeFile, strFullPathOutput.c_str(), "wb");
	if (err)
	{
		char szError[SIZE_ERRORBUFFER];
		sprintf_s(szError, sizeof(szError), "Error ERR_FOPEN_MOVE_MAFFT_FILE_WRITE on fopen_s() with err: %i%s", err, EOL);
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

long MoveMafftFiles()
{
	//This function moves all the .mafft file and replaces the names to .fa instead of .mafft and write them to the output folder where also the tree files are.
	/*
	STEP 1) Get the folder paths
	STEP 2) Get a dirlist of all the .mafft files.
	STEP 3) GO over 1 by one. read the file buffer (reusable) and write them to the
	*/
	long lReturn = ERR_NOERROR;

	//STEP ONE
	std::string strMafftPath;
	lReturn = glb.propertyBag.GetMoveMafftFilesPath(&strMafftPath);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETMOVEMAFFTFILEPATH_APPPF;

	std::string strOutputPath;
	lReturn = glb.propertyBag.GetOutputFolder(&strOutputPath);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETOUTPUTDIR_APPPF;

	//We want to reuse one file buffer, since we do not want to do a new for every file (roughly 15k files)
	size_t sizeBuffer = SIZE_BUFFER_HUNDRED_MB;
	char* pBuffer = new char[sizeBuffer];
	memset(pBuffer, '\0', sizeBuffer);
	 
	//STEP TWO
	std::vector<std::string> vecDirList;
	size_t sizeDirList = GetFilteredDirList(strMafftPath.c_str(), "*.mafft", &vecDirList);

	//long MoveMafftFile(std::string strFileName, std::string strDirPath, std::string strOutput, char** ppBuffer, size_t* psizeBuffer)

	std::string strMafftFile;
	//STEP THREE
	for (size_t idx = 0; idx < sizeDirList; idx++)
	{
		strMafftFile = vecDirList.at(idx);
		printf_s("Moving file msa file %s, %zu/%zu %s", strMafftFile.c_str(), idx + 1, sizeDirList, EOL);
		lReturn = MoveMafftFile(strMafftFile.c_str(), strMafftPath, strOutputPath,&pBuffer, &sizeBuffer);
		if (lReturn != ERR_NOERROR)
		{
			delete pBuffer;
			vecDirList.clear();
			return lReturn;
		}


	}

	delete pBuffer;

	return lReturn;
}