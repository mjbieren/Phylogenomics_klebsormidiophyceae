#include "Fasta.h"

long SplitHeaderNameAndNumber(const char* szToMatch, char** ppHeader, unsigned long* ulHeaderNumber, size_t* psizeBuffer);

long SetFileBuffer(char** pPos, std::string strFileName);
//long ParseFastaFile(const char * szPath);


long CFastaBlock::SetHeaderNumber(unsigned long ulHeaderNumber)
{
	if (ulHeaderNumber == 0)
		return FASTABLOCK_SETHEADERNUMBER;

	m_ulHeaderNumber = ulHeaderNumber;

	return ERR_NOERROR;
}

long CFastaBlock::GetHeaderNumber(unsigned long* pulHeaderNumber)
{
	if (m_ulHeaderNumber == 0)
		return FASTABLOCK_GETHEADERNUMBER;

	*pulHeaderNumber = m_ulHeaderNumber;

	return ERR_NOERROR;
}

unsigned long CFastaBlock::GetHeaderNumber() { return m_ulHeaderNumber; }

long CFastaBlock::SetFastaHeader(const char* pFastaHeader)
{
	size_t sizeHeader = strlen(pFastaHeader);
	if (sizeHeader == 0)
		return FASTABLOCK_SETFASTAHEADER;


	if (m_szFastaHeaderComplete)
		delete m_szFastaHeaderComplete;

	m_szFastaHeaderComplete = new char[sizeHeader + 10];
	memset(m_szFastaHeaderComplete, '\0', sizeHeader + 10);

	memcpy(m_szFastaHeaderComplete, pFastaHeader, sizeHeader);


	return ERR_NOERROR;
}

long CFastaBlock::SetFastaHeader(const char* pStart, const char* pEnd, bool bSetHeaderNumber)
{
	long lSizeDifference = pEnd - pStart;

	if (lSizeDifference <= 0 || lSizeDifference > SIZE_BUFFER_ONE_GIG)
		return FASTABLOCK_SETFASTAHEADER_POINTERDIFFERENCE;

	const char* pPosHeader = nullptr;
	if (*pStart == '>')
		pPosHeader = pStart + 1;
	else
	{
		pPosHeader = pStart;
		lSizeDifference--;
	}

	if (m_szFastaHeaderComplete)
		delete m_szFastaHeaderComplete;

	m_szFastaHeaderComplete = new char[lSizeDifference + 10];
	memset(m_szFastaHeaderComplete, '\0', lSizeDifference + 10);

	memcpy(m_szFastaHeaderComplete, pPosHeader, lSizeDifference);

	if (bSetHeaderNumber == true)
		SetHeaderNumber();
	
	return ERR_NOERROR;

}

long CFastaBlock::SetHeaderNumber()
{
	long lReturn = ERR_NOERROR;
	char* pPos = m_szFastaHeaderComplete;
	
	size_t sizeHeader = strlen(m_szFastaHeaderComplete);
	if (sizeHeader == 0)
		return FASTABLOCK_SETHEADERNUMBER;

	pPos += sizeHeader-1;

	MOVE_PTR_TO_PREV_NONDIGIT(pPos);

	pPos++;
	m_ulHeaderNumber = strtoul(pPos,NULL,0);

	//Set FastaHeaderStrain
	MOVE_PTR_TO_PREV_NONDIGIT(pPos);
	/*if (*pPos == '-')
		pPos--;*/

	//Check if there is a "TRINITY in the complete header.
	std::string strCompleteHeader = m_szFastaHeaderComplete;
	size_t sizePos = strCompleteHeader.find("TRINITY");
	if (sizePos != std::string::npos) //Header is a trinity header.
	{
		char* pHeaderNumberPos = m_szFastaHeaderComplete + sizePos; //Looking TRINITY_
		pPos = pHeaderNumberPos;
		lReturn = SetHeaderStrain(m_szFastaHeaderComplete, m_szFastaHeaderComplete+sizePos-1);
		if (lReturn != ERR_NOERROR)
			return lReturn;
		//>14614.18.24@TRINITY_DN21458_c0_g1.@1
		//Header Number = 21458
		MOVE_PTR_TO_DIGIT(pPos);
		if (isdigit(*pPos))
		{
			m_ulHeaderNumber = strtoul(pPos, NULL, 0);	
			lReturn = SetHeaderRest(pHeaderNumberPos, pHeaderNumberPos + strlen(pHeaderNumberPos));
			if (lReturn != ERR_NOERROR)
				return lReturn;
		}
		
	}
	else
	{
		lReturn = SetHeaderStrain(m_szFastaHeaderComplete, pPos);
		if (lReturn != ERR_NOERROR)
		return lReturn;
		//Time to find the position for the rest of the header.
		pPos = m_szFastaHeaderComplete + strlen(m_szFastaHeaderStrain);
		char* pEnd = m_szFastaHeaderComplete + strlen(m_szFastaHeaderComplete);

		if (*pPos == '@' || *pPos == ' ' || *pPos == '-' || *pPos == '|') //Separators.
			pPos++;

		lReturn = SetHeaderRest(pPos, pEnd);
		if (lReturn != ERR_NOERROR)
		return lReturn;
	}
	
	
	if (lReturn != ERR_NOERROR)
		return lReturn;

	return ERR_NOERROR;
}

long CFastaBlock::SetHeaderStrain(const char* pStart, const char* pEnd)
{
	long lReturn = ERR_NOERROR;

	long lSizeDifference = pEnd - pStart;

	if (lSizeDifference <= 0 || lSizeDifference > SIZE_BUFFER_ONE_GIG)
		return FASTABLOCK_SETFASTAHEADERSTRAIN_POINTERDIFFERENCE;

	if (m_szFastaHeaderStrain)
		delete m_szFastaHeaderStrain;

	m_szFastaHeaderStrain = new char[lSizeDifference + 10];
	memset(m_szFastaHeaderStrain, '\0', lSizeDifference + 10);

	memcpy(m_szFastaHeaderStrain, pStart, lSizeDifference);
		
	return lReturn;
}

long CFastaBlock::SetHeaderStrain(char* szHeaderStrain)
{
	if (!szHeaderStrain)
		return FASTABLOCK_SETHEADERSTRAIN_NODATA;

	m_szFastaHeaderStrain = szHeaderStrain;

	return ERR_NOERROR;
}

long CFastaBlock::SetHeaderRest(const char* pStart, const char* pEnd)
{
	long lReturn = ERR_NOERROR;

	long lSizeDifference = pEnd - pStart;

	if (lSizeDifference <= 0 || lSizeDifference > SIZE_BUFFER_ONE_GIG)
		return FASTABLOCK_SETFASTAHREST_POITNERDIFFERENCE;

	if (m_szFastaHeaderRest)
		delete m_szFastaHeaderRest;

	m_szFastaHeaderRest = new char[lSizeDifference + 10];
	memset(m_szFastaHeaderRest, '\0', lSizeDifference + 10);

	memcpy(m_szFastaHeaderRest, pStart, lSizeDifference);
		
	return lReturn;
}
long CFastaBlock::SetHeaderRest(char* szHeaderRest)
{
	if (!szHeaderRest)
		return FASTABLOCK_SETHEADERREST_NODATA;

	m_szFastaHeaderRest = szHeaderRest;

	return ERR_NOERROR;
}


long CFastaBlock::SetFastaSequence(char* pFastaSequence)
{
	size_t sizeHeader = strlen(pFastaSequence);
	if (sizeHeader == 0)
		return FASTABLOCK_SETFASTAHEADER;


	if (m_szFastaSequence)
		delete m_szFastaSequence;

	m_szFastaSequence = new char[sizeHeader + 10];
	memset(m_szFastaSequence, '\0', sizeHeader + 10);

	memcpy(m_szFastaSequence, pFastaSequence, sizeHeader);

	return ERR_NOERROR;
}

long CFastaBlock::SetFastaSequence(const char* pStart, const char* pEnd)
{
	long lSizeDifference = pEnd - pStart;

	if (lSizeDifference <= 0 || lSizeDifference > SIZE_BUFFER_ONE_GIG)
		return FASTABLOCK_SETFASTAHEADER_POINTERDIFFERENCE;


	if (m_szFastaSequence)
		delete m_szFastaSequence;

	m_szFastaSequence = new char[lSizeDifference + 10];
	memset(m_szFastaSequence, '\0', lSizeDifference + 10);

	memcpy(m_szFastaSequence, pStart, lSizeDifference);

	return ERR_NOERROR;

}

char* CFastaBlock::GetFastaHeader()
{
	return m_szFastaHeaderComplete;
}

long CFastaBlock::GetFastaHeader(char** ppFastaHeader)
{
	if (!m_szFastaHeaderComplete)
		return FASTABLOCK_GETFASTAHEADER;

	*ppFastaHeader = m_szFastaHeaderComplete;

	return ERR_NOERROR;
}

long CFastaBlock::GetFastaHeaderStrain(char** ppFastaHeaderStrain)
{
	if (!m_szFastaHeaderStrain)
		return FASTABLOCK_GETHEADERSTRAIN;

	*ppFastaHeaderStrain = m_szFastaHeaderStrain;
	return ERR_NOERROR;
}
char* CFastaBlock::GetFastaHeaderStrain()
{
	return m_szFastaHeaderStrain;
}


long CFastaBlock::GetFastaHeaderRest(char** ppFastHeaderRest)
{
	if (!m_szFastaHeaderRest)
		return FASTABLOCK_GETHEADERREST;

	*ppFastHeaderRest = m_szFastaHeaderRest;
	return ERR_NOERROR;
}
char* CFastaBlock::GetFastaHeaderRest()
{
	return m_szFastaHeaderRest;
}

long CFastaBlock::GetFastaSequence(char** ppFastaSequence)
{
	if (!m_szFastaSequence)
		return FASTABLOCK_GETFASTASEQUENCE;

	*ppFastaSequence = m_szFastaSequence;

	return ERR_NOERROR;
}

char* CFastaBlock::GetFastaSequence() { return m_szFastaSequence; }

long CFastaBlock::SetFastaBlock(const char* pStart, const char* pEnd, bool bSetHeaderNumber)
{
	/*
	A fasta block looks as follows
	
	>FastaHeader-1
	Sequence line 1
	Sequence line 2
	


	the goal is to 
	*/
	long lReturn = ERR_NOERROR;


	//Check first if it's a FastaBlock
	if (*pStart != '>')
		return FASTAFILE_PARSEFASTABLOCK_WRONGFORMAT;

	const char* pPos = pStart;

	//Parse first the header
	MOVE_PTR_TO_EOL(pPos);

	lReturn = SetFastaHeader(pStart, pPos, bSetHeaderNumber);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	//Set the FastaSequence
	MOVE_PTR_TO_BOL(pPos);
	lReturn = SetFastaSequence(pPos, pEnd);

	return lReturn;
}

long CFastaBlock::SetBoolFastaBlockWrite(bool bFastaBlockWrite)
{
	if (bFastaBlockWrite != false && m_bWriteFastaBlock != true)
		return FASTABLOCK_SETWRITEFASTABLOCKBOOL;

	m_bWriteFastaBlock = bFastaBlockWrite;
	return ERR_NOERROR;
}

long CFastaBlock::GetBoolFastaBlockWrite(bool* bpFastaBlockToWrite)
{
	if (m_bWriteFastaBlock != false && m_bWriteFastaBlock != true)
		return FASTABLOCK_GETWRITEFASTABLOCKBOOL;
	*bpFastaBlockToWrite = m_bWriteFastaBlock;
	return ERR_NOERROR;
}

bool CFastaBlock::GetBoolFastaBlockWrite()
{
	return m_bWriteFastaBlock;
}


long CFastaBlock::RemoveGapsFromSequence()
{
	//This function removes the gaps from the sequence
	long lReturn = ERR_NOERROR;

	//Check first if the fasta sequence exist (we can't remove characters from an empty zero terminated string!!!
	if (!m_szFastaSequence)
		return FASTABLOCK_GETFASTASEQUENCE;

	//Define the size for the new buffer
	size_t sizeBuffer = strlen(m_szFastaSequence); 
	
	//Create a buffer and zero temrinate it
	char* pBuffer = new char[sizeBuffer+10];//To have a bit of a filter.
	memset(pBuffer, '\0', sizeBuffer+10);

	//Fill the new buffer with the old data.
	char* pPosNewBuffer = pBuffer;

	for (size_t idx = 0; idx < sizeBuffer; idx++)
	{
		if (m_szFastaSequence[idx] != '-' && m_szFastaSequence[idx] != '\n' && m_szFastaSequence[idx] != '\r')
		{
			*pPosNewBuffer = m_szFastaSequence[idx];
			pPosNewBuffer++;
		}

	}

	//Delete the old one and replace FastaSequence
	delete m_szFastaSequence;

	m_szFastaSequence = pBuffer;



	return lReturn;
}

char* CFastaFile::GetFastaFileName()
{
	return m_szFastaFileName;
}

long CFastaFile::GetFastaFileName(char** ppFastaFileName)
{
	if (!m_szFastaFileName)
		return FASTAFILE_GETFASTAFILENAME;

	*ppFastaFileName = m_szFastaFileName;

	return ERR_NOERROR;
}

long CFastaFile::SetFastaFileName(std::string strFileName)
{
	long lReturn = ERR_NOERROR;

	size_t sizestrFileName = strFileName.size();

	if (sizestrFileName == 0)
		return FASTAFILE_EMPTY_STRING_FASTAFILENAME;

	if(m_szFastaFileName)
		delete m_szFastaFileName;

	m_szFastaFileName = new char[sizestrFileName + 10];
	memset(m_szFastaFileName, '\0', (sizestrFileName + 10));
	memcpy(m_szFastaFileName, strFileName.c_str(), sizestrFileName);

	return lReturn;
}




long CFastaFile::SetFastaFileName(const char* pStart, const char* pEnd)
{
	long lSizeDifference = pEnd - pStart;

	if (lSizeDifference <= 0 || lSizeDifference > SIZE_BUFFER_ONE_GIG)
		return FASTAFILE_SETFASTAFILENAME_POINTERDIFFERENCE;
	if (m_szFastaFileName)
		delete m_szFastaFileName;

	m_szFastaFileName = new char[lSizeDifference + 10];
	memset(m_szFastaFileName, '\0', lSizeDifference + 10);
	memcpy(m_szFastaFileName, pStart, lSizeDifference);

	return ERR_NOERROR;


}

long CFastaFile::SetMatchingName(const char* pStart, const char* pEnd)
{
	long lSizeDifference = pEnd - pStart;

	if (lSizeDifference <= 0 || lSizeDifference > SIZE_BUFFER_ONE_GIG)
		return FASTAFILE_SETMATCHINGNAME_POINTERDIFFERENCE;
	if (m_szMatchingName)
		delete m_szMatchingName;

	m_szMatchingName = new char[lSizeDifference + 10];
	memset(m_szMatchingName, '\0', lSizeDifference + 10);
	memcpy(m_szMatchingName, pStart, lSizeDifference);

	return ERR_NOERROR;

}
long CFastaFile::SetMatchingName(std::string strMatchingName)
{
	long lReturn = ERR_NOERROR;

	size_t sizestrFileName = strMatchingName.size();

	if (sizestrFileName == 0)
		return FASTAFILE_EMPTY_STRING_MATCHINGNAME;

	if(m_szMatchingName)
		delete m_szMatchingName;

	m_szMatchingName = new char[sizestrFileName + 10];
	memset(m_szMatchingName, '\0', (sizestrFileName + 10));
	memcpy(m_szMatchingName, strMatchingName.c_str(), sizestrFileName);

	return lReturn;
}
long CFastaFile::GetMatchingName(char* pMatchingName)
{
	if (!m_szMatchingName)
		return FASTAFILE_GETMATCHINGNAME;

	pMatchingName = m_szMatchingName;

	return ERR_NOERROR;
}
char* CFastaFile::GetMatchingName()
{
	return m_szMatchingName;
}


long CFastaFile::SortFastasFilesOnHeaderNumbers()
{
	long lReturn = ERR_NOERROR;

	std::sort(m_vecFastaBlock.begin(), m_vecFastaBlock.end(), [](CFastaBlock* a, CFastaBlock* b)
		{
			/*std::string strFirstColumnA = a->GetFastaHeader();
			std::string strFirstColumnB = b->GetFastaHeader();

			std::string a_name = CTable::StringToUpper(strFirstColumnA);
			std::string b_name = CTable::StringToUpper(strFirstColumnB);*/
			return ( a->GetHeaderNumber() < b->GetHeaderNumber());
		});

	m_bFastaBlockSorted = true;

	return lReturn;
}

std::string CFastaFile::StringToUpper(std::string strInput)
{
	std::transform(strInput.begin(), strInput.end(), strInput.begin(), ::toupper);
	return strInput;
}

long CFastaFile::SortFastaFileAlphabetically()
{
	long lReturn = ERR_NOERROR;
	std::sort(m_vecFastaBlock.begin(), m_vecFastaBlock.end(), [](CFastaBlock* a, CFastaBlock* b)
		{
			int iRes = strcasecmp(a->GetFastaHeader(), b->GetFastaHeader()); // Sort numbers as string (just in case).
			return iRes < 0;

		});

	m_bFastaBlockSorted = true;
	return lReturn;
}

long CFastaFile::ParseFastaFile(const char* pPath, bool bSetHeaderNumber)
{

	/*
	This function parses a fasta file into fasta blocks with only the file
	1) Get the File Buffer
	2) Parse Fasta blocks
	3) Get the Last Fasta block
	4) Clean up.
	
	*/
	long lReturn = ERR_NOERROR;
	std::string strFilePath = pPath;
	char* pBuffer = nullptr;

	//STEP 1, get the file buffer
	lReturn = SetFileBuffer(&pBuffer, strFilePath);
	if (lReturn != ERR_NOERROR)
	{
		printf_s("Could not set the file buffer for the Fasta file with file path %s %s", pPath, EOL);
		return FASTAFILE_GETFASTAFILEBUFFER;
	}

	//Set the smart pointer so that I do not have to add delete every time :)
	CSmartPtr<char> pSmartFastaFileBuffer = pBuffer;


	//Check if the fasta file has the right format.
	if (*pBuffer != '>')
	{
		printf_s("%s is not a Fasta file.%s", pPath, EOL);
		return FASTAFILE_WRONGFORMAT;
	}

	//Create and initate the variables needed for the parsing
	const char* pStart = pBuffer;
	const char* pEnd = pBuffer;
	CFastaBlock* pBlock = nullptr;

	while (pEnd)
	{
		pEnd++;
		MOVE_PTR_TO_NEXT_SEQUENCE(pEnd);
		if (*pEnd == '>')
			pEnd--;

		pBlock = new CFastaBlock;
		if (*pStart == '\0')
			break;
		lReturn = pBlock->SetFastaBlock(pStart, pEnd, bSetHeaderNumber);
		if (lReturn != ERR_NOERROR)
		{
			delete pBlock;
			return lReturn;
		}
		lReturn = this->PushBackRowSafe(pBlock);
		if (lReturn != ERR_NOERROR)
		{
			delete pBlock;
			return lReturn;
		}
		pEnd++;
		pStart = pEnd;

	}

#ifdef _DEBUG
	size_t sizeVector = m_vecFastaBlock.size();
	printf_s("There are %zu different FastaBlocks within this fasta file %s", sizeVector, EOL);
	CFastaBlock* pFastaToCheck = m_vecFastaBlock.at(sizeVector - 1);
	printf_s("The last one is %s %s", pFastaToCheck->GetFastaHeader(), EOL);
#endif

	return lReturn;
}

long CFastaFile::GetMatchingFastaBlockNumber(unsigned long ulHeaderNumber, CFastaBlock** ppFastaBlock)
{
	long lReturn = ERR_NOERROR;

	std::vector<CFastaBlock*>::iterator iterLowPos;
	std::vector<CFastaBlock*>::iterator iterHighPos;
	std::vector<CFastaBlock*>::iterator iterBegin = m_vecFastaBlock.begin();
	std::vector<CFastaBlock*>::iterator iterEnd = m_vecFastaBlock.end()-1;

	bool bFoundFastaBlock = false;

	CFastaBlock FastaBlockToSearch;
	FastaBlockToSearch.SetHeaderNumber(ulHeaderNumber);

	iterLowPos = std::lower_bound(iterBegin, iterEnd, &FastaBlockToSearch, [](CFastaBlock* a, CFastaBlock* b)
	{
			
		return (a->GetHeaderNumber() < b->GetHeaderNumber());
	});
	iterHighPos = std::upper_bound(iterBegin, iterEnd, &FastaBlockToSearch, [](CFastaBlock* a, CFastaBlock* b)
	{
			
		return (a->GetHeaderNumber() > b->GetHeaderNumber());
	});
	CFastaBlock* pFastaBlockToTest = nullptr;

	if (*iterLowPos)
	{
		if (iterLowPos != iterHighPos)
		{
			while (iterLowPos <= iterHighPos)
			{
				pFastaBlockToTest = *iterLowPos;
				if (pFastaBlockToTest->GetHeaderNumber() == ulHeaderNumber)
				{
					*ppFastaBlock = pFastaBlockToTest;
					bFoundFastaBlock = true;
					break;
				}
				iterLowPos++;
				pFastaBlockToTest = *iterLowPos;
				if (pFastaBlockToTest->GetHeaderNumber() == ulHeaderNumber)
				{
					*ppFastaBlock = pFastaBlockToTest;
					bFoundFastaBlock = true;
				}
			}
		}
		else
		{
			pFastaBlockToTest = *iterLowPos;
			if (pFastaBlockToTest->GetHeaderNumber() == ulHeaderNumber)
			{
				*ppFastaBlock = pFastaBlockToTest;
				bFoundFastaBlock = true;
				
			}
		}
	}
	if (bFoundFastaBlock == false)
		return FASTAFILE_MATCHING_FASTABLOCK_NOT_FOUND_BASED_ON_HEADERNUMBER;


	return lReturn;
}

long CFastaFile::GetMatchingFastaBlock(const char* szMatch, CFastaBlock** ppFastaBlock)
{
	long lReturn = ERR_NOERROR;

	size_t sizeToMatch = strlen(szMatch);
	

#ifdef _DEBUGX
	printf_s("Working on fasta block %s %s",szMatch, EOL);
#endif 

	std::vector<CFastaBlock*>::iterator iterLowPos;
	std::vector<CFastaBlock*>::iterator iterHighPos;
	std::vector<CFastaBlock*>::iterator iterBegin = m_vecFastaBlock.begin();
	std::vector<CFastaBlock*>::iterator iterEnd = m_vecFastaBlock.end()-1;

	bool bFoundFastaBlock = false;

	CFastaBlock FastaBlockToSearch;
	FastaBlockToSearch.SetFastaHeader(szMatch);

	iterLowPos = std::lower_bound(iterBegin, iterEnd, &FastaBlockToSearch, [sizeToMatch](CFastaBlock* a, CFastaBlock* b)
		{	
			int iRes = strcasecmp(a->GetFastaHeader(), b->GetFastaHeader()); // Sort numbers as string (just in case).
			return iRes < 0; 
	});
	iterHighPos = std::upper_bound(iterBegin, iterEnd, &FastaBlockToSearch, [sizeToMatch](CFastaBlock* a, CFastaBlock* b)
		{
			
			int iRes = strcasecmp(a->GetFastaHeader(), b->GetFastaHeader()); // Sort numbers as string (just in case).
			return iRes < 0; 
	});

	CFastaBlock* pFastaBlockToTest = nullptr;

	if (*iterLowPos)
	{
		if (iterLowPos != iterHighPos)
		{
			while (iterLowPos <= iterHighPos)
			{
				pFastaBlockToTest = *iterLowPos;				
				if (memcmp(szMatch, pFastaBlockToTest->GetFastaHeader(), sizeToMatch) == 0)
				{
					*ppFastaBlock = pFastaBlockToTest;
					bFoundFastaBlock = true;
					break;
				}
				iterLowPos++;
			}
		}
		else
		{
			pFastaBlockToTest = *iterLowPos;
			if (memcmp(szMatch, pFastaBlockToTest->GetFastaHeader(), sizeToMatch) == 0)
			{
				*ppFastaBlock = pFastaBlockToTest;
				bFoundFastaBlock = true;

			}
		}
	}
	if (bFoundFastaBlock == false)
		return FASTAFILE_MATCHING_FASTABLOCK_NOT_FOUND;


	return lReturn;
}

long CFastaFile::GetFirstAndLastMatchingBlockWithStrain(const char* szMatch, std::vector<CFastaBlock*>::iterator * pIterLowPos, std::vector<CFastaBlock*>::iterator* pIterHighPos)
{
	/*
	This function returns a range of strains of what matches the user input;
	For example when the user wants to retrieve all Fasta block from a specific species within a mixed fasta file (for example a decontamination file or an orthogroup).
	*/

	long lReturn = ERR_NOERROR;

	size_t sizeToMatch = strlen(szMatch);
	

#ifdef _DEBUGX
	printf_s("Working on fasta block %s %s",szMatch, EOL);
#endif 

	std::vector<CFastaBlock*>::iterator iterLowPos;
	std::vector<CFastaBlock*>::iterator iterHighPos;
	std::vector<CFastaBlock*>::iterator iterBegin = m_vecFastaBlock.begin();
	std::vector<CFastaBlock*>::iterator iterEnd = m_vecFastaBlock.end()-1;

	CFastaBlock FastaBlockToSearch;
	
	FastaBlockToSearch.SetHeaderStrain(szMatch, szMatch + strlen(szMatch));

	iterLowPos = std::lower_bound(iterBegin, iterEnd, &FastaBlockToSearch, [sizeToMatch](CFastaBlock* a, CFastaBlock* b)
		{	
			int iRes = strcasecmp(a->GetFastaHeaderStrain(), b->GetFastaHeaderStrain()); // Sort numbers as string (just in case).
			return iRes < 0; 
	});
	iterHighPos = std::upper_bound(iterBegin, iterEnd, &FastaBlockToSearch, [sizeToMatch](CFastaBlock* a, CFastaBlock* b)
		{
			
			int iRes = strcasecmp(a->GetFastaHeaderStrain(), b->GetFastaHeaderStrain()); // Sort numbers as string (just in case).
			return iRes < 0; 
	});


	*pIterLowPos = iterLowPos;
	*pIterHighPos= iterHighPos;

	return lReturn;

}

long CFastaFile::GetFirstAndLastMatchingBlockWithCompleteHeader(const char* szMatch, std::vector<CFastaBlock*>::iterator* pIterLowPos, std::vector<CFastaBlock*>::iterator* pIterHighPos)
{
	/*
	This function returns a range of strains of what matches the user input;
	For example when the user wants to retrieve all Fasta block from a specific species within a mixed fasta file (for example a decontamination file or an orthogroup).
	*/

	long lReturn = ERR_NOERROR;

	size_t sizeToMatch = strlen(szMatch);
	

#ifdef _DEBUGX
	printf_s("Working on fasta block %s %s",szMatch, EOL);
#endif 

	std::vector<CFastaBlock*>::iterator iterLowPos;
	std::vector<CFastaBlock*>::iterator iterHighPos;
	std::vector<CFastaBlock*>::iterator iterBegin = m_vecFastaBlock.begin();
	std::vector<CFastaBlock*>::iterator iterEnd = m_vecFastaBlock.end()-1;

	CFastaBlock FastaBlockToSearch;
	
	FastaBlockToSearch.SetFastaHeader(szMatch, szMatch + strlen(szMatch)+1);

	iterLowPos = std::lower_bound(iterBegin, iterEnd, &FastaBlockToSearch, [sizeToMatch](CFastaBlock* a, CFastaBlock* b)
		{	
			int iRes = strcasecmp(a->GetFastaHeader(), b->GetFastaHeader()); // Sort numbers as string (just in case).
			return iRes < 0; 
	});
	iterHighPos = std::upper_bound(iterBegin, iterEnd, &FastaBlockToSearch, [sizeToMatch](CFastaBlock* a, CFastaBlock* b)
		{
			
			int iRes = strcasecmp(a->GetFastaHeader(), b->GetFastaHeader()); // Sort numbers as string (just in case).
			return iRes < 0; 
	});


	*pIterLowPos = iterLowPos;
	*pIterHighPos= iterHighPos;

	return lReturn;
}

void CFastaFile::PrintHeadersToScreen()
{
	size_t sizeFastaBlocks = m_vecFastaBlock.size();
	CFastaBlock* pFastaBlock = nullptr;

	for (size_t idx = 0; idx < sizeFastaBlocks; idx++)
	{
		pFastaBlock = m_vecFastaBlock.at(idx);
		printf_s("Fasta Header %zu/%zu : %s %s", idx + 1, sizeFastaBlocks, pFastaBlock->GetFastaHeader(), EOL);
	}
}

long CFastaFile::RemoveGapsFromFastaBlocks()
{
	long lReturn = ERR_NOERROR;

	size_t sizeVector = m_vecFastaBlock.size();
	
	for (size_t idx = 0; idx < sizeVector; idx++)
	{
		lReturn = m_vecFastaBlock.at(idx)->RemoveGapsFromSequence();
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}

	return lReturn;
}


long FindMatchingFastaFile(std::vector<CFastaFile*>* pvecGenesFastaFile, const char* szToMatch, CFastaFile** ppFastaFileMatched, bool bDecontaminant = false, bool bSplitHeader = false)
{
	long lReturn = ERR_NOERROR;

	size_t sizeToMatch = strlen(szToMatch);

	std::vector<CFastaFile*>::iterator iterLowPos;
	std::vector<CFastaFile*>::iterator iterHighPos;
	std::vector<CFastaFile*>::iterator iterBegin = pvecGenesFastaFile->begin();
	std::vector<CFastaFile*>::iterator iterEnd = pvecGenesFastaFile->end()-1;

#ifdef _DEBUG
	CFastaFile* pBeginFastaFileBegin = *iterBegin;
	pBeginFastaFileBegin = pBeginFastaFileBegin;
	CFastaFile* pBeginFastaFileEnd =  *iterEnd;
	pBeginFastaFileEnd = pBeginFastaFileEnd;
#endif

	bool bFoundFastaBlock = false;
	std::string strFastaFileNameToFind;
	
	CFastaFile FastaFileToSearch;


	if (bSplitHeader == true)
	{
		//SplitHeaderNameAndNumber

		size_t sizeHeaderBuffer = SIZE_BUFFER_BIG;
		char* pHeaderWithoutNumber = new char[sizeHeaderBuffer];
		memset(pHeaderWithoutNumber, '\0', sizeHeaderBuffer);

		CSmartPtr<char> pAutoDelete = pHeaderWithoutNumber;
		unsigned long ulHeaderNumber = 0;

		lReturn = SplitHeaderNameAndNumber(szToMatch, &pHeaderWithoutNumber, &ulHeaderNumber, &sizeHeaderBuffer);
		if (lReturn != ERR_NOERROR)
			return lReturn;

		strFastaFileNameToFind = pHeaderWithoutNumber;
		

		if (bDecontaminant == false)
		{
			FastaFileToSearch.SetFastaFileName(strFastaFileNameToFind);
			iterLowPos = std::lower_bound(iterBegin, iterEnd, &FastaFileToSearch, [](CFastaFile* a, CFastaFile* b)
				{
					int iRes = strcasecmp(a->GetFastaFileName(), b->GetFastaFileName()); // Sort numbers as string (just in case).	
					return iRes < 0;
				});
			iterHighPos = std::upper_bound(iterBegin, iterEnd, &FastaFileToSearch, [](CFastaFile* a, CFastaFile* b)
				{
					int iRes = strcasecmp(a->GetFastaFileName(), b->GetFastaFileName());	// Sort numbers as string (just in case).
					return iRes < 0;
				});
		}
		else
		{
			FastaFileToSearch.SetMatchingName(strFastaFileNameToFind);
			iterLowPos = std::lower_bound(iterBegin, iterEnd, &FastaFileToSearch, [](CFastaFile* a, CFastaFile* b)
				{
					int iRes = strcasecmp(a->GetMatchingName(), b->GetMatchingName());	// Sort numbers as string (just in case).
					return iRes < 0;
				});
			iterHighPos = std::upper_bound(iterBegin, iterEnd, &FastaFileToSearch, [](CFastaFile* a, CFastaFile* b)
				{
					int iRes = strcasecmp(a->GetMatchingName(), b->GetMatchingName());	// Sort numbers as string (just in case).
					return iRes < 0;
				});

		}

	}
	else
	{
		strFastaFileNameToFind = szToMatch;
		
		if (bDecontaminant == false)
		{
			FastaFileToSearch.SetFastaFileName(strFastaFileNameToFind);
			iterLowPos = std::lower_bound(iterBegin, iterEnd, &FastaFileToSearch, [](CFastaFile* a, CFastaFile* b)
				{
					

					int iRes = strcasecmp(a->GetFastaFileName(), b->GetFastaFileName()); // Sort numbers as string (just in case).	
					return iRes < 0;
				});
			iterHighPos = std::upper_bound(iterBegin, iterEnd, &FastaFileToSearch, [](CFastaFile* a, CFastaFile* b)
				{
					int iRes = strcasecmp(a->GetFastaFileName(), b->GetFastaFileName());	 // Sort numbers as string (just in case).
					return iRes < 0;
				});
		}
		else
		{
			FastaFileToSearch.SetMatchingName(strFastaFileNameToFind);
			iterLowPos = std::lower_bound(iterBegin, iterEnd, &FastaFileToSearch, [](CFastaFile* a, CFastaFile* b)
				{
					int iRes = strcasecmp(a->GetMatchingName(), b->GetMatchingName());	// Sort numbers as string (just in case).
					return iRes < 0;
					
				});
			iterHighPos = std::upper_bound(iterBegin, iterEnd, &FastaFileToSearch, [](CFastaFile* a, CFastaFile* b)
				{
					int iRes = strcasecmp(a->GetMatchingName(), b->GetMatchingName());	// Sort numbers as string (just in case).
					return iRes < 0;
				});

		}
	}
	
	CFastaFile* pFastaFileToTest = nullptr;

	if (*iterLowPos)
	{
		sizeToMatch = strFastaFileNameToFind.length();
		if (iterLowPos != iterHighPos)
		{			
			while (iterLowPos <= iterHighPos)
			{
				pFastaFileToTest = *iterLowPos;
				if (bDecontaminant == false)
				{
					if (memcmp(strFastaFileNameToFind.c_str(),pFastaFileToTest->GetFastaFileName(), sizeToMatch) == 0)
					{
						*ppFastaFileMatched = pFastaFileToTest;
						bFoundFastaBlock = true;
						break;
					}
				}
				else
				{
					if (memcmp(strFastaFileNameToFind.c_str(), pFastaFileToTest->GetMatchingName(), sizeToMatch) == 0)
					{
						*ppFastaFileMatched = pFastaFileToTest;
						bFoundFastaBlock = true;
						break;
					}
				}
				iterLowPos++;
			}
		}
		else
		{
			pFastaFileToTest = *iterLowPos; //Fasta File import goes wrong, iterEnd is empty!!
						
			if (bDecontaminant == false)
			{
				if (memcmp(strFastaFileNameToFind.c_str(), pFastaFileToTest->GetFastaFileName(), sizeToMatch) == 0)
				{
					*ppFastaFileMatched = pFastaFileToTest;
					bFoundFastaBlock = true;

				}
			}
			else
			{
				if (memcmp(strFastaFileNameToFind.c_str(), pFastaFileToTest->GetMatchingName(), sizeToMatch) == 0)
				{
					*ppFastaFileMatched = pFastaFileToTest;
					bFoundFastaBlock = true;

				}
			
			}

		}
	}
	if (bFoundFastaBlock == false)
		return MATCHING_FASTAFILE_NOT_FOUND;


	return lReturn;
}

long SplitHeaderNameAndNumber(const char* szToMatch, char** ppHeader, unsigned long* ulHeaderNumber, size_t* psizeBuffer)
{
	size_t sizeToMatch = strlen(szToMatch);
	if (sizeToMatch < 3) //A-1 = valid
		return MATCHING_SPLIT_HEADERNAME_AND_NUMBER_TOO_SMALL;

	if (strlen(szToMatch) >= *psizeBuffer)
	{
		if (*ppHeader)
			delete* ppHeader;

		*psizeBuffer = sizeToMatch + 10;
		*ppHeader = new char[*psizeBuffer];

	}

	memset(*ppHeader, '\0', *psizeBuffer);
	const char* pPos = szToMatch;
	const char* pEnd = szToMatch + sizeToMatch;


	pPos = pEnd - 1;
	MOVE_PTR_TO_PREV_NONDIGIT(pPos);
	pPos++;
	*ulHeaderNumber = strtoul(pPos, NULL, 0);
	MOVE_PTR_TO_PREV_NONDIGIT(pPos);
	/*if (*pPos == '-')
		pPos--;*/

	memcpy(*ppHeader, szToMatch, pPos - szToMatch);

	return ERR_NOERROR;
}

long SortFastaFilesAlphabeticallyFileName(std::vector<CFastaFile*> *pvecFastaFiles)
{
	long lReturn = ERR_NOERROR;
	std::sort(pvecFastaFiles->begin(), pvecFastaFiles->end(), [](CFastaFile* a, CFastaFile* b)
		{
			int iRes = strcasecmp(a->GetFastaFileName(), b->GetFastaFileName()); // Sort numbers as string (just in case).
			return iRes < 0;
		});
	

	return lReturn;
}

long SortFastaFilesAlphabeticallyMatchingName(std::vector<CFastaFile*> *pvecFastaFiles)
{
	long lReturn = ERR_NOERROR;
	std::sort(pvecFastaFiles->begin(), pvecFastaFiles->end(), [](CFastaFile* a, CFastaFile* b)
	{
		int iRes = strcasecmp(a->GetMatchingName(), b->GetMatchingName());	// Sort numbers as string (just in case).
		return iRes < 0;

	});

	return lReturn;

}


long FindMatchingFastaFileFileName(std::vector<CFastaFile*> *pvecFastaFiles, char* pszFileName, CFastaFile ** ppFoundFastaFile)
{
	long lReturn = ERR_NOERROR;

	size_t sizeToMatch = strlen(pszFileName);

	std::vector<CFastaFile*>::iterator iterLowPos;
	std::vector<CFastaFile*>::iterator iterHighPos;
	std::vector<CFastaFile*>::iterator iterBegin = pvecFastaFiles->begin();
	std::vector<CFastaFile*>::iterator iterEnd = pvecFastaFiles->end()-1;

#ifdef _DEBUG
	CFastaFile* pBeginFastaFileBegin = *iterBegin;
	pBeginFastaFileBegin = pBeginFastaFileBegin;
	CFastaFile* pBeginFastaFileEnd = *iterEnd;;
	pBeginFastaFileEnd = pBeginFastaFileEnd;
#endif

	bool bFoundFastaFile = false;
	std::string strFastaFileNameToFind = pszFileName;
	
	CFastaFile FastaFileToSearch;

	FastaFileToSearch.SetFastaFileName(strFastaFileNameToFind);
	iterLowPos = std::lower_bound(iterBegin, iterEnd, &FastaFileToSearch, [](CFastaFile* a, CFastaFile* b)
	{
		
		int iRes = strcasecmp(a->GetFastaFileName(), b->GetFastaFileName());	// Sort numbers as string (just in case).
		return iRes < 0;
		
	});
	iterHighPos = std::upper_bound(iterBegin, iterEnd, &FastaFileToSearch, [](CFastaFile* a, CFastaFile* b)
	{
		int iRes = strcasecmp(a->GetFastaFileName(), b->GetFastaFileName()); // Sort numbers as string (just in case).
		return iRes < 0;
	});

#ifdef _DEBUG //For Debugging purposes :)
	CFastaFile* pFastaFileLow = *iterLowPos;
	pFastaFileLow = pFastaFileLow;
	CFastaFile* pFastaFileHigh = *iterHighPos;
	pFastaFileHigh = pFastaFileHigh;
#endif

	CFastaFile* pFastaFileToTest = nullptr;

	if (*iterLowPos)
	{
		sizeToMatch = strFastaFileNameToFind.length();
		if (iterLowPos != iterHighPos)
		{
			while (iterLowPos <= iterHighPos)
			{
				pFastaFileToTest = *iterLowPos;

				if (memcmp(strFastaFileNameToFind.c_str(), pFastaFileToTest->GetFastaFileName(), sizeToMatch) == 0)
				{
					*ppFoundFastaFile = pFastaFileToTest;
					bFoundFastaFile = true;
					break;
				}

				iterLowPos++;
			}
		}
		else
		{
			//iterLowPos is equal to iterHighPos
			pFastaFileToTest = *iterLowPos;
			if (memcmp(strFastaFileNameToFind.c_str(), pFastaFileToTest->GetFastaFileName(), sizeToMatch) == 0)
			{
				*ppFoundFastaFile = pFastaFileToTest;
				bFoundFastaFile = true;
			}
		}
	}
	if (bFoundFastaFile == false)
		return MATCHING_FASTAFILE_FILENAME_NOT_FOUND;

	return lReturn;
}

long FindMatchingFastaFileMatchingName(std::vector<CFastaFile*> *pvecFastaFiles, char* pszMatchingName, CFastaFile** ppFoundFastaFile)
{
	long lReturn = ERR_NOERROR;

	size_t sizeToMatch = strlen(pszMatchingName);

	std::vector<CFastaFile*>::iterator iterLowPos;
	std::vector<CFastaFile*>::iterator iterHighPos;
	std::vector<CFastaFile*>::iterator iterBegin = pvecFastaFiles->begin();
	std::vector<CFastaFile*>::iterator iterEnd = pvecFastaFiles->end()-1;

#ifdef _DEBUG
	CFastaFile* pBeginFastaFileBegin = *iterBegin;
	pBeginFastaFileBegin = pBeginFastaFileBegin;
	CFastaFile* pBeginFastaFileEnd = *iterEnd;
	pBeginFastaFileEnd = pBeginFastaFileEnd;
#endif

	bool bFoundFastaFile = false;
	std::string strFastaFileNameToFind = pszMatchingName;
	
	CFastaFile FastaFileToSearch;

	FastaFileToSearch.SetMatchingName(strFastaFileNameToFind);
	iterLowPos = std::lower_bound(iterBegin, iterEnd, &FastaFileToSearch, [](CFastaFile* a, CFastaFile* b)
	{
		int iRes = strcasecmp(a->GetMatchingName(), b->GetMatchingName());	// Sort numbers as string (just in case).
		return iRes < 0;

	});
	iterHighPos = std::upper_bound(iterBegin, iterEnd, &FastaFileToSearch, [](CFastaFile* a, CFastaFile* b)
	{
		int iRes = strcasecmp(a->GetMatchingName(), b->GetMatchingName());	 // Sort numbers as string (just in case).
		return iRes < 0;
	});

	CFastaFile* pFastaFileToTest = nullptr;

	if (*iterLowPos)
	{
		sizeToMatch = strFastaFileNameToFind.length();
		if (iterLowPos != iterHighPos)
		{
			while (iterLowPos <= iterHighPos)
			{
				pFastaFileToTest = *iterLowPos;

				if (memcmp(strFastaFileNameToFind.c_str(), pFastaFileToTest->GetMatchingName(), sizeToMatch) == 0)
				{
					*ppFoundFastaFile = pFastaFileToTest;
					bFoundFastaFile = true;
					break;
				}

				iterLowPos++;
			}

		}
		else
		{
			//iterLowPos is equal to iterHighPos
			pFastaFileToTest = *iterLowPos;
			if (memcmp(strFastaFileNameToFind.c_str(), pFastaFileToTest->GetMatchingName(), sizeToMatch) == 0)
			{
				*ppFoundFastaFile = pFastaFileToTest;
				bFoundFastaFile = true;
			}
		}
	}
	if (bFoundFastaFile == false)
		return 	MATCHING_FASTAFILE_MATCHINGNAME_NOT_FOUND;

	return lReturn;
}