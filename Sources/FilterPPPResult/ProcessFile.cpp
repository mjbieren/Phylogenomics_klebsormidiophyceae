#include "stdafx.h"
#include "Management.h"

#define ERROR_PARSE_ROW_SIZE_TOO_SMALL 10000
#define ERROR_PARSE_ROW_SIZE_TOO_BIG 10001

long SetFileBuffer(char** ppPos, std::string strFileName)
{
	//stat the file
	//Open the file and fread into the buffer
	long lReturn = ERR_NOERROR;

	struct stat statInfo; //stat info about our file of interest;

	if (stat(strFileName.c_str(), &statInfo) || !(statInfo.st_mode & S_IFMT))
	{
		printf_s("Error ERR_FILE_OF_INTEREST_DOES_NOT_EXIST File: %s%s", strFileName.c_str(), EOL);
		return ERR_FILE_OF_INTEREST_DOES_NOT_EXIST_SET_FILE_BUFFER;
	}

	uint64_t sizeFile = statInfo.st_size;
	char* pBuffer = new char[sizeFile + 100];
	memset(pBuffer, '\0', sizeFile + 100); // just to be sure since the computer have a habit to overwrite old buffers :').

	FILE* hFileOfInterest;

	errno_t ernum = fopen_s(&hFileOfInterest, strFileName.c_str(), "rb");

	//If we got an error, we will do a proper error handling
	if (ernum)
	{
		char szError[SIZE_BUFFER_MEDIUM];
		sprintf_s(szError, sizeof(szError), "Error ERR_FOPEN_FILE_OF_INTEREST err: %i on line: %i in file %s while opening file %s%s", ernum, __LINE__, __FILE__, strFileName.c_str(), EOL);
		perror(szError);

		delete pBuffer;
		return ERR_FOPEN_SET_FILE_BUFFER_COULD_NOT_OPEN_FILE;
	}

	//Now we read the file into our buffer
	size_t sizeData = fread(pBuffer, 1, sizeFile, hFileOfInterest);
	sizeData = sizeData;
	ASSERT(sizeData == sizeFile);

	//we of course close our file again
	fclose(hFileOfInterest);

	//Zero terminates the file buffer
	pBuffer[sizeFile] = '\0';

	*ppPos = pBuffer; //will be cleaned up in the other file.

	return lReturn;
}

long SetFileBufferReuse(char** ppExistingBuffer, std::string strFileName, size_t* pSizeBuffer)
{
	//stat the file
	//Open the file and fread into the buffer
	long lReturn = ERR_NOERROR;

	//Check first if existing buffer actually looks at something. If not create a file buffer that's big enough. (1gb)
	if (!*ppExistingBuffer)
	{
		//Buffer doesn't actually exist.

		*ppExistingBuffer = new char[SIZE_BUFFER_HUNDRED_MB];
		*pSizeBuffer = SIZE_BUFFER_HUNDRED_MB;

		memset(*ppExistingBuffer, '\0', SIZE_BUFFER_HUNDRED_MB);
	
	}

	struct stat statInfo; //stat info about our file of interest;

	if (stat(strFileName.c_str(), &statInfo) || !(statInfo.st_mode & S_IFMT))
	{
		printf_s("Error ERR_FILE_OF_INTEREST_DOES_NOT_EXIST File: %s%s", strFileName.c_str(), EOL);
		return ERR_FILE_OF_INTEREST_DOES_NOT_EXIST_SET_FILE_BUFFER;
	}

	uint64_t sizeFile = statInfo.st_size;
	
	char* pBuffer = nullptr;

	//Now we have to check if the current buffer size is big enough for our file otherwise we replace the old buffer!.

	if (sizeFile >= *pSizeBuffer) //New file size is bigger than old buffer.
	{
		if (*ppExistingBuffer) //Check if file exist and delete, should actually exist but you never know!
			delete* ppExistingBuffer;

		*ppExistingBuffer = new char[sizeFile + 100];
		*pSizeBuffer = sizeFile + 100;
		memset(*ppExistingBuffer, '\0', *pSizeBuffer);
	}

	//Reset the memory with memset
	memset(*ppExistingBuffer, '\0', *pSizeBuffer);


	pBuffer = *ppExistingBuffer;

	FILE* hFileOfInterest;

	errno_t ernum = fopen_s(&hFileOfInterest, strFileName.c_str(), "rb");

	//If we got an error, we will do a proper error handling
	if (ernum)
	{
		char szError[SIZE_BUFFER_MEDIUM];
		sprintf_s(szError, sizeof(szError), "Error ERR_FOPEN_FILE_OF_INTEREST err: %i on line: %i in file %s while opening file %s%s", ernum, __LINE__, __FILE__, strFileName.c_str(), EOL);
		perror(szError);

		delete pBuffer;
		return ERR_FOPEN_SET_FILE_BUFFER_COULD_NOT_OPEN_FILE;
	}

	//Now we read the file into our buffer
	size_t sizeData = fread(pBuffer, 1, sizeFile, hFileOfInterest);
	sizeData = sizeData;
	ASSERT(sizeData == sizeFile);

	//we of course close our file again
	fclose(hFileOfInterest);

	//Zero terminates the file buffer
	pBuffer[sizeFile] = '\0';

	*ppExistingBuffer = pBuffer; //will be cleaned up in the other file.

	return lReturn;
}

long DetermineFilebufferSize(size_t* psizeBuffer, std::string strFileName)
{
	long lReturn =  ERR_NOERROR;

	struct stat statInfo; //stat info about our file of interest;

	if (stat(strFileName.c_str(), &statInfo) || !(statInfo.st_mode & S_IFMT))
	{
		printf_s("Error ERR_FILE_OF_INTEREST_DOES_NOT_EXIST File: %s%s", strFileName.c_str(), EOL);
		return ERR_FILE_OF_INTEREST_DOES_NOT_EXIST_GET_BUFFER_SIZE;
	}

	if (statInfo.st_size == 0)
		return ERR_FILE_OF_INTEREST_BUFFER_SIZE_BUFFER_TOO_SMALL;


	*psizeBuffer = statInfo.st_size;
	

	return lReturn;
}


long SetBufferFromPositions(char* pStart, char* pEnd, char** ppBuffer, bool bGetEndChar = false)
{
	/*
	Basically a function to set a buffer based on two file positions (a start and end positions)
	the bGetEndChar is basically made to have an option to also obtain the character that pEnd is looking at or not.
	e.g. you would do an EOL and you do not want to copy the EOL then you wouldn't copy the EOL aka the bGetEndChar = false. "ThisIsAStringToCopy/r/n" (pEnd is looking at /r or /n)
	But if you would have your pEnd looking at EOL but move it back to before the EOL aka the bGetEndChar = true "ThisIsAStringToCopy" (pEnd is looking at y"

	*/
#ifdef _DEBUG
	size_t sizeBuffer = pEnd - pStart;
	sizeBuffer = sizeBuffer; //Prevent warning, SizeBuffer variable not used.
#endif

	long lReturn = ERR_NOERROR;

	if ((pEnd - pStart) <= 0)
		return ERROR_PARSE_ROW_SIZE_TOO_SMALL;

	if ((pEnd - pStart) > SIZE_BUFFER_ONE_GIG)//Just to be sure this file has EOLs, keep in mind that certain orthogroup files have rows that are longer than 15k characters. Hence why SIZE_BUFFER_BIG will be too small!
		return ERROR_PARSE_ROW_SIZE_TOO_BIG;


	char* pBuffer = new char[pEnd - pStart + 10];
	memset(pBuffer, '\0', pEnd - pStart + 10);

	if(bGetEndChar == false)
		memcpy(pBuffer, pStart, (pEnd - pStart));
	else
		memcpy(pBuffer, pStart, (pEnd - pStart+ (strlen(EOL))));

	*ppBuffer = pBuffer;
	return lReturn;
}

long SplitStringByDelimitedCharacter(std::vector<char*>* pvecStringSplitted, std::string strStringToBeSplit, char cDelimitedChar)
{
	//This function split the string into little zero terminated strings basd on the delimited character (cDelimitedChar). E.g. Tab, comma, space etc.
	//First determine how big the string is.
	//Second determine how many columsn there are
	//Third
	size_t sizeStringToBeSplit = strStringToBeSplit.length();
	if (sizeStringToBeSplit <= 0)
		return 1; //Todo Error Message

	const char* pStart = strStringToBeSplit.c_str();
	const char* pPos = pStart;
	const char* pEnd = pStart + sizeStringToBeSplit;

	std::string::difference_type n = std::count(strStringToBeSplit.begin(), strStringToBeSplit.end(), cDelimitedChar);

	char* pStringBuffer = nullptr;
	size_t sizeStringBuffer = 0;

	for (size_t idx = 0; idx < (size_t) n; idx++)
	{
		//Move pPos to the delimited character.
		MOVE_PTR_TO_CHAR(pPos, cDelimitedChar);
		
		sizeStringBuffer = pPos - pStart;
		pStringBuffer = new char[sizeStringBuffer + 10]; //we want to zero terminate the string
		memset(pStringBuffer, '\0', sizeStringBuffer + 10);
		memcpy(pStringBuffer, pStart, sizeStringBuffer);


		pvecStringSplitted->push_back(pStringBuffer);

		pPos++;
		pStart = pPos;


	}

	//Get the last one
	sizeStringBuffer = pEnd - pPos;
	pStringBuffer = new char[sizeStringBuffer + 10]; //we want to zero terminate the string
	memset(pStringBuffer, '\0', sizeStringBuffer + 10);
	memcpy(pStringBuffer, pStart, sizeStringBuffer);


	pvecStringSplitted->push_back(pStringBuffer);


	return ERR_NOERROR;

}