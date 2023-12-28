#include "GlobalHelper.h"

long DetermineFilebufferSize(size_t* psizeBuffer, std::string strFileName);

long CreateSummaryFile(std::vector<CFastaFile*>* pvecFastaOutputFiles)
{
	long lReturn = ERR_NOERROR;

	//Create a .txt summary file which writes down how many proteins were discovered vs total
	char* pOutputBuffer = new char[SIZE_BUFFER_ONE_GIG];
	memset(pOutputBuffer, '\0', SIZE_BUFFER_ONE_GIG); //Zero terminate it
	CSmartPtr<char> pAutoDelete = pOutputBuffer;
	char* pPos = pOutputBuffer;
	size_t sizeBufferLeft = SIZE_BUFFER_ONE_GIG;

	uint64_t u64PositiveMatched = 0;
	uint64_t u64NegativeMatched = 0;
	uint64_t u64matched = 0;
	uint64_t u64TotalMatched = 0;


	pPos += sprintf_s(pPos, sizeBufferLeft, "Header Name\tMatched Genes\tType Of Header<+/->\tPercentage of Matched genes %s",EOL);
	sizeBufferLeft = SIZE_BUFFER_ONE_GIG - (pPos - pOutputBuffer);

	CFastaFile* pFastaFile = nullptr;

	size_t sizeFastaVector = pvecFastaOutputFiles->size();
	size_t sizeFastaBlocks = 0;
	
	long double dPercentage = 0.0;

	//Species group /t number of matched genes /t Decontaminant yes/no /t %percentage of matched genes vs total.

	//Check first how many total matched and negative vs Positive
	for (size_t idx = 0; idx < sizeFastaVector; idx++)
	{
		pFastaFile = pvecFastaOutputFiles->at(idx);
		sizeFastaBlocks = pFastaFile->GetVectorSize();
		u64TotalMatched = u64TotalMatched + (uint64_t)sizeFastaBlocks;

		if (pFastaFile->GetDecontaminant() == true)
			u64NegativeMatched = u64NegativeMatched + (uint64_t)sizeFastaBlocks;
		else
			u64PositiveMatched = u64PositiveMatched + (uint64_t)sizeFastaBlocks;


	}

	//Parse the individual lines
	for (size_t idx = 0; idx < sizeFastaVector; idx++)
	{
		pFastaFile = pvecFastaOutputFiles->at(idx);
		u64matched = pFastaFile->GetVectorSize();
		
		dPercentage = (long double)u64matched / (long double)u64TotalMatched;

		if (pFastaFile->GetDecontaminant() == false)
		{
			pPos += sprintf_s(pPos, sizeBufferLeft, "%s\t%zu\t+\t %.2Lf%c%s", pFastaFile->GetFastaFileName(), u64matched, dPercentage * 100, 37, EOL);
			sizeBufferLeft = SIZE_BUFFER_ONE_GIG - (pPos - pOutputBuffer);
		}
		else
		{
			pPos += sprintf_s(pPos, sizeBufferLeft, "%s\t%zu\t-\t %.2Lf%c%s", pFastaFile->GetFastaFileName(), u64matched, dPercentage * 100, 37, EOL);
			sizeBufferLeft = SIZE_BUFFER_ONE_GIG - (pPos - pOutputBuffer);
		}


	}

	//now the summary line

	/*
	Total Matched Genes: 9455 | Positive Matches: 6672/70.57% (Klebsormidiaceae:6672) | Decontamination matches: 2783/29.43% (Archea:1,Bacteria:946,Viridae:3,Fungi:1833,Plastid:0)
	
	Total Matches
	Positive set
	negative set
	*/

	pPos += sprintf_s(pPos, sizeBufferLeft, "Total Matched Genes: %zu | Positive Matches: %zu/%.2Lf%c (", u64TotalMatched, u64PositiveMatched, ((long double) u64PositiveMatched/(long double)u64TotalMatched) * 100, 37, EOL);
	sizeBufferLeft = SIZE_BUFFER_ONE_GIG - (pPos - pOutputBuffer);
	//Parse the positive items
	for (size_t idx = 0; idx < sizeFastaVector; idx++)
	{
		pFastaFile = pvecFastaOutputFiles->at(idx);
		
		
		if (pFastaFile->GetDecontaminant() == false)
		{
			// Positive set
			pPos += sprintf_s(pPos, sizeBufferLeft, "%s:%zu,", pFastaFile->GetFastaFileName(), pFastaFile->GetVectorSize());
			sizeBufferLeft = SIZE_BUFFER_ONE_GIG - (pPos - pOutputBuffer);
		}
		else
		{
			//Negative set
			continue;
		}


	}
	pPos[-1] = ')'; // remove the last , and replace it with a )

	//Now the negative set
	pPos += sprintf_s(pPos, sizeBufferLeft, " | Decontamination matches: %zu/%.2Lf%c (", u64NegativeMatched, ((long double) u64NegativeMatched/(long double)u64PositiveMatched) * 100, 37, EOL);
	sizeBufferLeft = SIZE_BUFFER_ONE_GIG - (pPos - pOutputBuffer);
	for (size_t idx = 0; idx < sizeFastaVector; idx++)
	{
		pFastaFile = pvecFastaOutputFiles->at(idx);
		
		
		if (pFastaFile->GetDecontaminant() == true)
		{
			//Negative set
			pPos += sprintf_s(pPos, sizeBufferLeft, "%s:%zu,", pFastaFile->GetFastaFileName(), pFastaFile->GetVectorSize());
			sizeBufferLeft = SIZE_BUFFER_ONE_GIG - (pPos - pOutputBuffer);
		}
		else
		{
			//Positive set
			continue;
		}


	}
	pPos[-1] = ')'; // remove the last , and replace it with a )



	//Write the output file
	std::string strStrainName;
	lReturn = glb.propertyBag.GetStrainName(&strStrainName);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	std::string strOutputFolder;
	lReturn = glb.propertyBag.GetOutputPath(&strOutputFolder);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	std::string strTimeStamp = std::to_string(std::time(0));
	std::string strSummaryFile = strOutputFolder + strStrainName + "_" + strTimeStamp + "_Summary_Decontamination.txt";

	printf_s("Create Summary Decontamination file: %s %s", strSummaryFile.c_str(), EOL);


	FILE* hFileFasta;

	errno_t err = fopen_s(&hFileFasta, strSummaryFile.c_str(), "wb");
	if (err)
	{
		char szError[SIZE_ERRORBUFFER];
		sprintf_s(szError, sizeof(szError), "Error ERR_FOPEN_SUMMARY_FILE_WRITE on fopen_s() with err: %i%s", err, EOL);
		printf_s("%s", szError);

		delete pOutputBuffer;
		return WRITE_OUTPUT_FILE_SUMMARY_DECONTAMINATION;

	}
	else
	{
		//Place the full content of strTotalFasta into the new file and close it
		fputs(pOutputBuffer, hFileFasta);
	}
	if (hFileFasta != 0)
		fclose(hFileFasta);



	return lReturn;
}




long WriteOutputFiles(std::vector<CFastaFile*>* pvecFastaOutputFiles)
{
	//Step 1 Get the strain name & Output Folder
	//Step 2 Get the file buffer size for both the summary file and fasta files
	//Step 3 loop over fasta file vector
	//Step 3 write the output file
	//Step 4 Write the summary file
	//Step 5 Cleanup
	//Step 6 Done

	long lReturn = ERR_NOERROR;

	//STEP ONE
	std::string strStrainName;
	lReturn = glb.propertyBag.GetStrainName(&strStrainName);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	std::string strOutputFolder;
	lReturn = glb.propertyBag.GetOutputPath(&strOutputFolder);
	if (lReturn != ERR_NOERROR)
		return lReturn;


	//STEP TWO
	std::string strFastaFile;
	lReturn = glb.propertyBag.GetFastaQuery(&strFastaFile);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	size_t sizeFileBuffer = 0;
	size_t sizeBufferLeft = 0;

	lReturn = DetermineFilebufferSize(&sizeFileBuffer, strFastaFile);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	char* pBuffer = new char[sizeFileBuffer + 100];
	char* pBufferPos = pBuffer;
	CSmartPtr<char> pAutoDelete = pBuffer;

	memset(pBuffer, '\0', sizeFileBuffer + 100);

	size_t sizeFastaFiles = pvecFastaOutputFiles->size();
	size_t sizeFastaBlocks = 0;

	CFastaFile* pCurrentFastaFileWriting = nullptr;
	CFastaBlock* pCurrentFastaBlock = nullptr;

	FILE* hFileOutputFasta = nullptr;

	std::string strFileName;
	std::string strFilePathFull;

	for (size_t idx = 0; idx < sizeFastaFiles; idx++)
	{
		//Reset the variables and reset the buffer
		sizeBufferLeft = sizeFileBuffer;
		memset(pBuffer, '\0', sizeFileBuffer + 100);
		pBufferPos = pBuffer;


		//Obtain variables
		pCurrentFastaFileWriting = pvecFastaOutputFiles->at(idx);
		sizeFastaBlocks = pCurrentFastaFileWriting->GetVectorSize();
		if (sizeFastaBlocks == 0)
			continue; //No Matches we do not have to continue with the rest

		//Obtain Fasta File Name
		strFileName = pCurrentFastaFileWriting->GetFastaFileName();
		strFilePathFull = strOutputFolder + strStrainName + "_" + strFileName + ".fa";

		printf_s("Writing output file %s_%s.fa %s",strFileName.c_str(), strStrainName.c_str(), EOL);

		//Fill the Buffer
		for (size_t idx2 = 0; idx2 < sizeFastaBlocks; idx2++)
		{

			sizeBufferLeft = sizeBufferLeft - (pBufferPos - pBuffer);
			pCurrentFastaBlock = pCurrentFastaFileWriting->GetFastaBlock(idx2);
			pBufferPos += sprintf_s(pBufferPos, sizeBufferLeft, ">%s%s%s%s", pCurrentFastaBlock->GetFastaHeader(), EOL, pCurrentFastaBlock->GetFastaSequence(),EOL);

		}

		//Remove last EOL
		pBufferPos--;
		*pBufferPos = '\0';
		//Write the Output File
		errno_t errFopen_s =  fopen_s(&hFileOutputFasta, strFilePathFull.c_str(), "wb");
		if (errFopen_s != ERR_NOERROR)
		{
			printf_s("Error in opening fasta file %s, on line %i in file %s %s", strFilePathFull.c_str(), __LINE__, __FILE__,EOL);
			return WRITE_OUTPUT_FILE_ERROR_FOPEN;
		}
		else
		{
			fputs(pBuffer, hFileOutputFasta);
		}
		fclose(hFileOutputFasta);
	}

	//Write the summary file
	lReturn = CreateSummaryFile(pvecFastaOutputFiles);

	return lReturn;
}