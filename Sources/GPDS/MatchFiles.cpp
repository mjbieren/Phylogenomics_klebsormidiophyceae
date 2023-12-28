#include "GlobalHelper.h"

long SplitHeaderNameAndNumber(char* szToMatch, char** ppHeader, unsigned long* ulHeaderNumber, size_t* psizeBuffer);
long FindMatchingFastaFile(std::vector<CFastaFile*>* pvecGenesFastaFile, const char* szToMatch, CFastaFile** ppFastaFileMatched, bool bDecontaminant = false, bool bSplitHeader = false);

long MatchingFiles(std::vector<CFastaFile*>* pvecFastaOutputFiles, CTable* pBlastFiltered, CFastaFile* pFastaQuery)
{
	long lReturn = ERR_NOERROR;

	//Step 1 loop over the blast file, all items have to be matched to a specific Fasta Output File
	//Step 2 Find which Fasta block
	//Step 3 Find which fasta output file
	//Step 4 Place fasta block in fasta output file



	size_t sizeBlastFileRows = pBlastFiltered->GetAmountOfTableRows();

	//rows look like: Strain, decontaminant, rest
	std::vector<char*>* pRow = nullptr;

	char* pFastaHeaderToFind = nullptr;
	CFastaBlock* pFastaBlock = nullptr;
	CFastaFile* pFastaFileToFind = nullptr;

	size_t sizeHeaderBuffer = SIZE_BUFFER_BIG;
	char* pHeaderBufferWithoutNumber = new char[sizeHeaderBuffer + 10];
	memset(pHeaderBufferWithoutNumber, '\0', sizeHeaderBuffer + 10);
	
#ifdef _DEBUG
	size_t sizeFastaVector = pFastaQuery->GetVectorSize();
	for (size_t idx = 0; idx < sizeFastaVector; idx++)
	{
		if (idx == 1150)
			printf("Found you");
		pFastaHeaderToFind = pFastaQuery->GetFastaBlock(idx)->GetFastaHeader();
		printf_s("Header %zu/ %zu : %s %s", idx, sizeFastaVector, pFastaHeaderToFind, EOL);
	}
#endif 

	//STEP ONE
	for (size_t idx = 0; idx < sizeBlastFileRows; idx++)
	{
		pRow = pBlastFiltered->GetRow(idx);
		pFastaHeaderToFind = pRow->at(0);

		//STEP TWO
		lReturn = pFastaQuery->GetMatchingFastaBlock(pFastaHeaderToFind, &pFastaBlock);
		if (lReturn != ERR_NOERROR)
		{
			delete pHeaderBufferWithoutNumber;
			return lReturn;
		}

		//STEP THREE
		lReturn = FindMatchingFastaFile(pvecFastaOutputFiles, pRow->at(1), &pFastaFileToFind, true, true);
		if (lReturn != ERR_NOERROR)
		{
			delete pHeaderBufferWithoutNumber;
			return lReturn;
		}

		//STEP FOUR
		lReturn = pFastaFileToFind->PushBackRowSafe(pFastaBlock);
		if (lReturn != ERR_NOERROR)
		{
			delete pHeaderBufferWithoutNumber;
			return lReturn;
		}

	}

	//Set all fasta output files as delete blocks false
	size_t sizeFastaFiles = pvecFastaOutputFiles->size();
	for (size_t idx = 0; idx < sizeFastaFiles; idx++)
	{
		pFastaFileToFind = pvecFastaOutputFiles->at(idx);
		pFastaFileToFind->SetDeleteRows(false);
	}

	delete pHeaderBufferWithoutNumber;

	return lReturn;
}