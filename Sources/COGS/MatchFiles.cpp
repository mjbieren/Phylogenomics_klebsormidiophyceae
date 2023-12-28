#include "GlobalHelper.h"

long FindMatchingFastaFileFileName(std::vector<CFastaFile*>* pvecFastaFiles, char* pszFileName, CFastaFile** ppFoundFastaFile);
long FindMatchingFastaFileMatchingName(std::vector<CFastaFile*>* pvecFastaFiles, char* pszMatchingName, CFastaFile** ppFoundFastaFile);
long SortFastaFilesAlphabeticallyFileName(std::vector<CFastaFile*>* pvecFastaFiles);

long MatchFiles(std::vector<CFastaFile*>* pvecFastaFilesSource, std::vector<CFastaFile*>* pvecFastaFileOrthoGroups, std::vector<CTable*>* pvecOrthoGroups)
{
	/*
	This function matches the Orthogroup columns with the right fasta file/blocks and create new fasta file objects It does not filter them (yet) that's another step

	1) Go Over Each individual CTable Object
	2) Each row represent one OrthoGroup
	3) Create a Fasta File per OrthoGroup with the name of the first column, but replace the . to _ because certain tools can not handle the naem otherwise. N0.0000001 into N0_0000001
	4) Find the corresponding Fasta file and Fasta blocks from the OrthoGroup Columns
	5) Push back the FastaBlock pointer in the new CFasta File per Orthogroup
	6) Set the Fasta file delete as false (we do not want to delete the Fasta blocks in these new Fasta files, just clear the vectors)
	7) Push back fasta file in the fasta file vector.
	8) Done
	*/

	long lReturn = ERR_NOERROR;

	size_t sizeOrthoGroupTables = pvecOrthoGroups->size();
	size_t sizeOrthoGroupRows = 0;
	size_t sizeColumns = 0;
	CTable* pTable = nullptr;
	CTableRowHeader* pTableHeader = nullptr;
	CFastaFile* pFastaOrthoGroup = nullptr;

	CFastaFile * pFastaFileToFind = nullptr;
	CFastaBlock * pFastaBlockToFind = nullptr;
	std::string strFastaFileNameOrthoGroup;
	
	size_t sizePosOfCharacterToBeReplaced = 0;

	std::vector<char*>* pvecOrthoGroupRow = nullptr;

	char* pFastaFileName = nullptr;

	char* szFastaHeaderToFind = new char[SIZE_BUFFER_MEDIUM];
	CSmartPtr<char> pAutoDelete = szFastaHeaderToFind;
	memset(szFastaHeaderToFind, '\0', SIZE_BUFFER_MEDIUM);
	const char* pColumn = nullptr;
	const char* pPos = nullptr;
	const char* pStart = nullptr;


	//STEP ONE
	for (size_t idx = 0; idx < sizeOrthoGroupTables; idx++) //Loop over every table
	{
		pTable = pvecOrthoGroups->at(idx);
		sizeOrthoGroupRows = pTable->GetAmountOfTableRows();
		printf_s("Matching OrthoGroup file %zu/%zu: %s with Fasta Blocks.%s", idx+1, sizeOrthoGroupTables, pTable->GetTableName(), EOL);

		lReturn = pTable->GetHeaderRowTable(&pTableHeader);
		if (lReturn != ERR_NOERROR)
			return lReturn;

		//STEP TWO
		for (size_t idx2 = 0; idx2 < sizeOrthoGroupRows; idx2++) //Loop over every row
		{
			pvecOrthoGroupRow = pTable->GetRow(idx2);
			sizeColumns = pvecOrthoGroupRow->size();

			//STEP THREE
			pFastaOrthoGroup = new CFastaFile;
			//STEP SIX
			pFastaOrthoGroup->SetDeleteRows(false); //To make sure that it is set to false, because the items it contains are stored somewhere else.
			strFastaFileNameOrthoGroup = pvecOrthoGroupRow->at(0); //Set the Name
			sizePosOfCharacterToBeReplaced = strFastaFileNameOrthoGroup.find('.', 0);
			strFastaFileNameOrthoGroup.replace(sizePosOfCharacterToBeReplaced, 1, "_"); //Replacing N0.000000001 for N0_000000001
			pFastaOrthoGroup->SetFastaFileName(strFastaFileNameOrthoGroup);

			for (size_t idx3 = 3; idx3 < sizeColumns; idx3++) //Loop over every column but we skip the first 3, since we do not need them.
			{
				//STEP FOUR
				//Corresponding columns matches header of the table!

				//Find the corresponding fasta file we need.

				//Check if the column is empty (can happen)
				if (memcmp(" ", pvecOrthoGroupRow->at(idx3), 1) == 0)
					continue; //It's empty so we skip it!

				pFastaFileName = pTableHeader->GetVectorItem(idx3)->GetHeaderName(); //They are just the species names.

				lReturn = FindMatchingFastaFileFileName(pvecFastaFilesSource, pFastaFileName, &pFastaFileToFind);
				if (lReturn != ERR_NOERROR)
				{
					delete pFastaOrthoGroup;
					return lReturn;
				}

				//Find the corresponding fasta blocks from the corresponding fasta file
				//It looks like "A1-1, A1-2, A1-3. And they all have to be found.
				pColumn = pvecOrthoGroupRow->at(idx3);
				pStart = pColumn;
				pPos = pColumn;

				while (*pPos)
				{
					while (*pPos != ',' && *pPos)
						pPos++;
					
					
					memcpy(szFastaHeaderToFind, pStart, pPos - pStart);
					lReturn = pFastaFileToFind->GetMatchingFastaBlock(szFastaHeaderToFind,&pFastaBlockToFind); //Find the fasta block
					if (lReturn != ERR_NOERROR)
					{
						delete pFastaOrthoGroup;
						return lReturn;
					}
					//We have found it. time to push it back into the new Fasta File
					//STEP FIVE
					lReturn = pFastaOrthoGroup->PushBackRowSafe(pFastaBlockToFind);
					if (lReturn != ERR_NOERROR)
					{
						delete pFastaOrthoGroup;
						return lReturn;
					}

					//Reset the pFastaHeaderToFind
					memset(szFastaHeaderToFind, '\0', SIZE_BUFFER_MEDIUM);

					if (*pPos == ',') //A1-1, 
					{	//We have pushed it back so now we have to move the items
						pStart = pPos + 2; //comma + space = +2
						pPos = pStart; //Set the rest properly.

					}	
				}
			}	
			//Pushed back all the fasta blocks, time to push back the fasta file into the fasta file vector that will be written later.
			//STEP SEVEN
			pvecFastaFileOrthoGroups->push_back(pFastaOrthoGroup);

		}

		printf_s("Found %zu Orthogroups in file: %s.%s", sizeOrthoGroupRows, pTable->GetTableName(), EOL);

	}

	lReturn = SortFastaFilesAlphabeticallyFileName(pvecFastaFileOrthoGroups);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	//STEP EIGHT
	return lReturn;

}
