#include "GlobalHelper.h"


long WriteFastaFile(CFastaFile* pFastaFile, std::string strDirPath, char ** ppFileBuffer, bool bPhyloPyPrunerFormat, size_t sizeBuffer)
{
	//This function is to write a Fasta file
	/*
	1) Get the Fasta File Name from the pFastaFile Object
	2) Determine if we have a phylopypruner format. If so the order of executing will be = >StrainName@HeaderNumber EOL SequenceEOL
	*/

	long lReturn = ERR_NOERROR;

	char* pFastaFileName = nullptr;
	lReturn = pFastaFile->GetFastaFileName(&pFastaFileName);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	std::string strFullFilePath = strDirPath + pFastaFileName + ".fa";

	CFastaBlock* pCurrentFastaBlock = nullptr;
	size_t sizeFastaBlockVector = pFastaFile->GetVectorSize();

	char* pBufferPos = *ppFileBuffer;

	size_t sizeBufferLeft = sizeBuffer;

	//Fill The buffer
	if (bPhyloPyPrunerFormat == false)
	{
		for (size_t idx = 0; idx < sizeFastaBlockVector; idx++)
		{
			sizeBufferLeft = sizeBufferLeft - (pBufferPos - *ppFileBuffer);
			pCurrentFastaBlock = pFastaFile->GetFastaBlock(idx);
			pBufferPos += sprintf_s(pBufferPos, sizeBufferLeft, ">%s%s%s%s", pCurrentFastaBlock->GetFastaHeader(), EOL, pCurrentFastaBlock->GetFastaSequence(), EOL);

		}
	}
	else
	{
		for (size_t idx = 0; idx < sizeFastaBlockVector; idx++)
		{
			sizeBufferLeft = sizeBufferLeft - (pBufferPos - *ppFileBuffer);
			pCurrentFastaBlock = pFastaFile->GetFastaBlock(idx);
			//pBufferPos += sprintf_s(pBufferPos, sizeBufferLeft, ">%s%s%lu%s%s%s", pCurrentFastaBlock->GetFastaHeaderStrain(),"@",pCurrentFastaBlock->GetHeaderNumber() ,EOL, pCurrentFastaBlock->GetFastaSequence(), EOL);
			pBufferPos += sprintf_s(pBufferPos, sizeBufferLeft, ">%s%s%s%s%s%s", pCurrentFastaBlock->GetFastaHeaderStrain(),"@",pCurrentFastaBlock->GetFastaHeaderRest() ,EOL, pCurrentFastaBlock->GetFastaSequence(), EOL);

		}
	}

	//Remove last EOL
	pBufferPos--;
	*pBufferPos = '\0';

	FILE* hFileOutputFasta = nullptr;

	errno_t errFopen_s =  fopen_s(&hFileOutputFasta, strFullFilePath.c_str(), "wb");
		if (errFopen_s != ERR_NOERROR)
		{
			printf_s("Error in opening fasta file %s, on line %i in file %s %s", strFullFilePath.c_str(), __LINE__, __FILE__,EOL);
			return WRITE_OUTPUT_FILE_ERROR_FOPEN;
		}
		else
		{
			fputs(*ppFileBuffer, hFileOutputFasta);
		}
		fclose(hFileOutputFasta);

	return lReturn;
}

long WriteOutputFiles(std::vector<CFastaFile*>* pvecFastaFileOrthoGroupsFiltered)
{
	long lReturn = ERR_NOERROR;

	//This function writes all the fasta files with the Orthogroup names and a summary file
	//1) Check if the user want the PhylopyPruner Format or not
	//2) Loop over the vector Fasta file
	//3) Print out every 50 fasta files how for we are.
	//4) Write the Fasta File
	//5) Write the Summary File

	//Code is being reused from the OSG program. with slight adjustments.

	size_t sizeFilteredAmount = pvecFastaFileOrthoGroupsFiltered->size();
		
	std::string strFastaDirOutput;
	lReturn = glb.propertyBag.GetOutputDir(&strFastaDirOutput);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETFASTADIR_FPPPR;

	//Set the File Buffer for all the files.
	char* pBuffer = new char[SIZE_BUFFER_ONE_GIG];
	CSmartPtr<char> pAutoDelete = pBuffer; //Create smart pointer so that we do not have to keep track of it.
	memset(pBuffer, '\0', SIZE_BUFFER_BIG); //Set all characters as zero terminated.


	//STEP ONE
	bool bPhyloPyPrunerFormat = false;
	lReturn = glb.propertyBag.GetBoolPhyloPyPruner(&bPhyloPyPrunerFormat);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_PHYLOPYPRUNERFORMAT_OSG;


	CFastaFile* pFasta = nullptr;

	//STEP TWO
	for (size_t idx = 0; idx < sizeFilteredAmount; idx++)
	{
		pFasta = pvecFastaFileOrthoGroupsFiltered->at(idx);
		if (!pFasta)
			return WRITE_OUTPUT_ORTHOGROUP_FASTAOBJECT;

		//Reset the buffer
		memset(pBuffer, '\0', SIZE_BUFFER_BIG);

		//STEP THREE
		if (idx % 50 == 0) //We do not want to print out that we are writing a file for each file only every 50.
			printf_s("Write filtered Fasta files to the output folder. File %zu out of %zu %s", idx+1, sizeFilteredAmount, EOL);
		else
		{
#ifdef _DEBUG
			printf_s("DEBUG MODE: Write filtered Fasta files to the output folder. File %zu out of %zu %s", idx+1, sizeFilteredAmount, EOL);
#endif
		}

		//STEP FOUR
		lReturn = WriteFastaFile(pFasta, strFastaDirOutput, &pBuffer, bPhyloPyPrunerFormat, SIZE_BUFFER_ONE_GIG);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}

	printf_s("Done with writing all the filtered PPP Result fasta files %s", EOL);


	


	return lReturn;
}

