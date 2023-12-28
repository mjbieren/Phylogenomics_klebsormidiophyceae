#include "Table.h"

long SetFileBuffer(char** pPos, std::string strFileName);
long SetBufferFromPositions(char* pStart, char* pEnd, char** ppBuffer, bool bGetEndChar = false);
long GetRowBuffer(char* pStart, char* pEnd, char** ppBuffer, bool bGetEndChar = false);



long CompareColumnNames(std::string strColumnNames, std::string strColumnNamesNew, unsigned int * piColumnNames, std::vector<char*> * pvecColumn, std::vector<char*> * pvecColumnRenew)
{
	/*
	This function is to check if the Column Names and Rename have the same amount of input
	If they have they parse the individual columns into a vector.
	1) Count the columns
	2) Check if column amount is the same
	3) Parse the columns into the two vectors
	*/
	long lReturn = ERR_NOERROR;

	//1
	unsigned int uiColumnName = (int) std::count(strColumnNames.begin(), strColumnNames.end(), ',');
	unsigned int uiColumnNameRename = (int) std::count(strColumnNamesNew.begin(), strColumnNamesNew.end(), ',');

	//2
	if (uiColumnName != uiColumnNameRename)
		return TABLE_NOTMATCHING_COLUMNNUMBER;

	//3)
	char* pStartColumNames = &strColumnNames[0];
	char* pStartColumnNamesRenew = &strColumnNamesNew[0];
	char* pEndColumNames = &strColumnNames[0];
	char* pEndColumnNamesRenew = &strColumnNamesNew[0];

	//Create the buffers pointers
	char* pBufferColumnName = nullptr;
	char* pBufferColumnNameRenew = nullptr;

	for (unsigned int idx = 0; idx < uiColumnName; idx++)
	{
		//Move the pointers to a ,
		MOVE_PTR_TO_COMMA(pEndColumNames);
		MOVE_PTR_TO_COMMA(pEndColumnNamesRenew);

		pBufferColumnName = new char[pEndColumNames - pStartColumNames + 10];
		memset(pBufferColumnName, '\0', (pEndColumNames - pStartColumNames + 10));
		memcpy(pBufferColumnName, pStartColumNames, (pEndColumNames - pStartColumNames));
		pvecColumn->push_back(pBufferColumnName);

		pBufferColumnNameRenew = new char[pEndColumnNamesRenew - pStartColumnNamesRenew + 10];
		memset(pBufferColumnNameRenew, '\0', (pEndColumnNamesRenew - pStartColumnNamesRenew + 10));
		memcpy(pBufferColumnNameRenew, pStartColumnNamesRenew, (pEndColumnNamesRenew - pStartColumnNamesRenew));
		pvecColumnRenew->push_back(pBufferColumnNameRenew);

	}

	*piColumnNames = uiColumnName;

	return lReturn;
}

long ParseFileHeader(char* pHeaderBuffer, CTableRowHeader** ppHeaders, std::string strHeaderColumns, std::string strHeaderColumnsRenew)
{
	/*
	This function is meant to parse the table header into columns
	1) Count how many columns we have
	2) Create TableColumns
	3) Parse the pHeaderBuffer into the different columns
	4) Check if strHeaderColumns and strHeaderColumnsRenew have the same amount of columns
	5) Check which columns have te be passed by strHeaderColumns and add the name renew as well
	*/
	long lReturn = ERR_NOERROR;

	char* pStart = pHeaderBuffer;
	char* pEnd = pHeaderBuffer;
	
	//1) Count how  many Columns we have since it's tab delimated. Column 1 /t Column 2 /t Column 3 /t Column N
	size_t sizeColumnCount =0;
	while (pStart)
	{
		if (*pStart == '\t')
			sizeColumnCount++;

		pStart++;
	}
	//Reset it back to the original position.
	pStart = pHeaderBuffer;
	//You always have 1 more column than there are tabs.
	sizeColumnCount++;

	CTableColumnHeader* pHeaderColumn = nullptr;
	CTableRowHeader* pHeader = *ppHeaders;

	//Create the columns
	for (size_t idx = 0; idx < sizeColumnCount; idx++)
	{
		pHeaderColumn = new CTableColumnHeader;
		pHeader->PushBack(pHeaderColumn);
	}

	//3) Fill the columns
	for (size_t idx2 = 0; idx2 < sizeColumnCount; idx2++)
	{
		pHeaderColumn = pHeader->GetVectorItem(idx2);
		MOVE_PTR_TO_TAB(pEnd);
		lReturn = pHeaderColumn->SetHeaderName(pStart, pEnd);
		if (lReturn != ERR_NOERROR)
			return lReturn;

		pEnd++;
		pStart = pEnd;
	}

	//4 Check if strHeaderColumns and strHeaderColumnsRenew have the same amount of columns
	unsigned int uiColumnNames = 0;

	std::vector<char*> vecColumn;
	std::vector<char*> vecColumnRenew;
	lReturn = CompareColumnNames(strHeaderColumns, strHeaderColumnsRenew, &uiColumnNames, &vecColumn, &vecColumnRenew);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	size_t sizeColumnName = 0;
	bool bIsFound = false;
	//Now we know how much columns have to be added and now we can determine which columns have to be added.
	for (unsigned int idx = 0; idx < uiColumnNames; idx++)
	{
		sizeColumnName = strlen(vecColumn.at(idx));

		for (size_t idx2 = 0; idx2 < sizeColumnCount; idx2++)
		{
			pHeaderColumn = pHeader->GetVectorItem(idx2);
			if (memcmp(pHeaderColumn->GetHeaderName(), vecColumn.at(idx), sizeColumnName) == 0)
			{
				//Found the column Name now we have to add the renew name
				pHeaderColumn->SetHeaderNameNew(vecColumnRenew.at(idx));
				pHeaderColumn->SetBoolAdd();
				bIsFound = true;
			}
		}
		if (bIsFound == false)
		{
			printf("Could not find column '%s' within the original Table file %s", vecColumn.at(idx), EOL);
			return TABLE_NOMATCH_HEADERCOLUMN;
		}

		bIsFound = false;

	}

	return lReturn;
}

long GetRowBuffer(char* pStart, char* pEnd, char** ppBuffer, bool bGetEndChar)
{
	long lReturn = ERR_NOERROR;

	if ((pEnd - pStart) <= 0)
		return TABLE_PARSE_ROW_SIZE_TOO_SMALL;

	if ((pEnd - pStart) > SIZE_BUFFER_BIG)//Just to be sure this file has EOLs
		return TABLE_PARSE_ROW_SIZE_TOO_BIG;


	char* pRowBuffer = new char[pEnd - pStart + 10];
	memset(pRowBuffer, '\0', pEnd - pStart + 10);

	if(bGetEndChar == false)
		memcpy(pRowBuffer, pStart, (pEnd - pStart));
	else
		memcpy(pRowBuffer, pStart, (pEnd - pStart+(strlen(EOL))));

	*ppBuffer = pRowBuffer;
	return lReturn;
}

long ParseFileRow(char * pStart, char * pEnd, size_t sizeColumn, CTableRowHeader ** ppTableRow)
{
	long lReturn = ERR_NOERROR;

	char* pBuffer = nullptr;
	lReturn = GetRowBuffer(pStart, pEnd, &pBuffer);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	CSmartPtr<char> psmrtBuffer = pBuffer;

	char* pStartPos = pBuffer;
	char* pEndPos = pBuffer;
	CTableRowHeader* pTableRow = *ppTableRow;

	CTableColumnHeader* pColumn = nullptr;

	for (size_t idx = 0; idx < sizeColumn; idx++)
	{
		MOVE_PTR_TO_TAB(pEndPos);
		pColumn = new CTableColumnHeader;

		pColumn->SetHeaderName(pStartPos, pEndPos);
		pTableRow->PushBack(pColumn);
		
		if(pEndPos)
		pEndPos++;

		pStartPos = pEndPos;
	}

	return lReturn;
}

long  CTableRowHeader::SetNewColumn(char* pStart, char* pEnd)
{
	long lReturn = ERR_NOERROR;

	//Check first if the pStart and pEnd are valid.
	if ((pEnd - pStart <= 0) ||( pEnd - pStart >= SIZE_BUFFER_ONE_GIG))
		return TABLE_NEWCOLUMN_SIZE_FORMAT_WRONG; //Todo error

	

	char* pColumnBuffer = nullptr;

	lReturn = GetRowBuffer(pStart, pEnd, &pColumnBuffer);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	CTableColumnHeader* pColumn = new CTableColumnHeader;
	pColumn->SetHeaderName(pStart, pEnd);


	return lReturn;
}

long CTable::SetTableName(const char* pStart, const char* pEnd)
{
	long lReturn = ERR_NOERROR;

	if (m_pTableName)
		delete m_pTableName; //Make Sure that the memory is deleted, we don't want memory leaks

	//Check first if the pStart and pEnd are valid.
	if ((pEnd - pStart <= 0) ||( pEnd - pStart >= SIZE_BUFFER_ONE_GIG))
		return TABLE_NEWCOLUMN_SIZE_FORMAT_WRONG; //Todo error

	m_pTableName = new char[pEnd - pStart + 10]; //Create new buffer
	memset(m_pTableName, 0, pEnd - pStart + 10); //Zero terminate everything
	memcpy(m_pTableName, pStart, pEnd - pStart); //Copy the HeaderName;

	return lReturn;
}

std::string CTable::StringToUpper(std::string strInput)
{
	std::transform(strInput.begin(), strInput.end(), strInput.begin(), ::toupper);
	return strInput;
}

bool CTable::CompareFirstColumnAlphabetically(std::vector<char*>* a, std::vector<char*>* b)
{

	int iRes = strcasecmp(a->at(0), b->at(0)); // Sort numbers as string (just in case).
	return iRes < 0;

}

void CTable::SortTableRowsAlphabetically(int iColumnToMatch)
{
	//First column is what the order should be based off
	m_iColumnToCheck = iColumnToMatch;
	std::vector<std::vector<char*>*> *pvecRows = &m_vecRows;
	std::sort(pvecRows->begin(), pvecRows->end(),CompareFirstColumnAlphabetically);

}

long CTable::ParseRow(char* pStart, char* pEnd, size_t sizeHeader, char CDelimeted)
{
	long lReturn = ERR_NOERROR;
	
	//We first check if we have a valid Row
	if ((pEnd - pStart <= 0) ||( pEnd - pStart >= SIZE_BUFFER_ONE_GIG))
		return TABLE_PARSE_ROW_SIZE_FORMAT_WRONG; //Todo error

	std::vector<char*>* pRow = new std::vector<char*>;
		
	char* pPosStart = pStart;
	char* pPos = pStart;
	
	char* pColumnBuffer = nullptr;

	unsigned int idx = 0;

	while (*pPosStart && (pPosStart != pEnd))
	{
		if ((idx + 1) == sizeHeader)
		{

			//we are at the last one
			if ((pPosStart - pEnd) == 0)
			{
				pColumnBuffer = new char[2];
				pColumnBuffer[0] = ' ';
				pColumnBuffer[1] = '\0';
			}
			else
			{
				lReturn = SetBufferFromPositions(pPosStart, pEnd, &pColumnBuffer);
				if (lReturn != ERR_NOERROR)
				{
					if (pColumnBuffer)
						delete pColumnBuffer;

					
					if (lReturn == 10000) //ERROR_PARSE_ROW_SIZE_TOO_SMALL
						return TABLE_PARSE_ROW_SIZE_TOO_SMALL;
					else if (lReturn == 10001)
						return TABLE_PARSE_ROW_SIZE_TOO_BIG; //ERROR_PARSE_ROW_SIZE_TOO_BIG
					

					return lReturn;
				}
				
			}
			pRow->push_back(pColumnBuffer);
			idx++;
			break;
		}

		MOVE_PTR_TO_CHAR(pPos,CDelimeted);
		
		if ((pPos - pPosStart) == 0)
		{
			pColumnBuffer = new char[2];
			pColumnBuffer[0] = ' ';
			pColumnBuffer[1] = '\0';
		}
		else
		{
			lReturn = SetBufferFromPositions(pPosStart, pPos, &pColumnBuffer);
			if (lReturn != ERR_NOERROR)
			{
				if (pColumnBuffer)
					delete pColumnBuffer;
				return lReturn;
			}
		}
		pRow->push_back(pColumnBuffer);
		idx++;

		pPos++;
		pPosStart = pPos;
	}

	if ((idx + 1) == sizeHeader) //Meaning the last column is not filled. Double check by using -1
	{
		if (pPosStart[-1] == '\t') //Last column not filled
		{
			pColumnBuffer = new char[2];
			pColumnBuffer[0] = ' ';
			pColumnBuffer[1] = '\0';

			pRow->push_back(pColumnBuffer);
			idx++;


		}
		else //This means that the rows have uneven columns!
			return TABLE_SIZE_HEADER_COLUMN_WRONG;
		
	}

	lReturn = PushBackRowSafe(pRow);

	if (sizeHeader != ((size_t)idx))
		return TABLE_SIZE_HEADER_COLUMN_WRONG; //ToDo Error Message.

	//Now we are done with the parsing of the rows
	return lReturn;
}

const long CTable::FindMatchingRow(char* szMatchingName, std::vector<char*>** ppRow, size_t sizeRowToMatch)
{
	long lReturn = ERR_NOERROR;

	if (!szMatchingName)
		return TABLE_FIND_MATCHING_ROW_NAME_DOES_NOT_EXIST; //ToDo error Message

	//std::vector<std::vector<char*>*>

	std::vector<std::vector<char*>*>::iterator iterLowPos;
	std::vector<std::vector<char*>*>::iterator iterHighPos;

	std::vector<std::vector<char*>*>::iterator iterBegin = m_vecRows.begin();
	std::vector<std::vector<char*>*>::iterator iterEnd = m_vecRows.end()-1;


	std::vector<char*> vecSearchRow;

	std::vector<char*>* pRow = nullptr;
	char* pszToTest = nullptr;

	for (size_t idx = 0; idx < sizeRowToMatch; idx++)
	{
		vecSearchRow.push_back(szMatchingName);
	}

	size_t sizeMatchingRow = sizeRowToMatch - 1; //We are looking at 0,1,2,3,4. Not 1,2,3,4,5

	bool bFoundRow = false;
	//We have to find the lowest and highest possible match.
	iterLowPos = std::lower_bound(iterBegin, iterEnd, &vecSearchRow, [sizeMatchingRow](std::vector<char*>* a, std::vector<char*>* b)
		{
			int iRes = strcasecmp(a->at(sizeMatchingRow), b->at(sizeMatchingRow)); // Sort numbers as string (just in case).
			return iRes < 0; 

		});
	iterHighPos = std::upper_bound(iterBegin, iterEnd, &vecSearchRow, [sizeMatchingRow](std::vector<char*>* a, std::vector<char*>* b)
		{
			int iRes = strcasecmp(a->at(sizeMatchingRow), b->at(sizeMatchingRow)); // Sort numbers as string (just in case).
			return iRes < 0;
		});

	if (*iterLowPos)
	{
		if (iterLowPos != iterHighPos) //if we have more than one match
		{
			while (iterLowPos <= iterHighPos)
			{
				pRow = *iterLowPos;
				pszToTest = pRow->at(sizeMatchingRow);

				if (memcmp(szMatchingName, pszToTest, strlen(szMatchingName)) == 0)
				{
					*ppRow = pRow;
					bFoundRow = true;
					break;
				}
				iterLowPos++;
			}

		}
		else
		{
			pRow = *iterLowPos;
			pszToTest = pRow->at(sizeMatchingRow);

			if (memcmp(szMatchingName, pszToTest, strlen(szMatchingName)) == 0)
			{
				*ppRow = pRow;
				bFoundRow = true;

			}

		}
	}

	if (bFoundRow == false)
		return TABLE_ROW_MATCHING_NOT_FOUND; //Todo Error Message;

	vecSearchRow.clear();

	return lReturn;
}

long CTable::ParseHeader(char* pStart, char* pEnd, CTable* Table, char cDelimeted)
{
	long lReturn = ERR_NOERROR;

	//We first check if we have a valid header
	if ((pEnd - pStart <= 0) ||( pEnd - pStart >= SIZE_BUFFER_ONE_GIG))
		return TABLE_PARSE_HEADER_FORMAT_WRONG; //Todo error

	int iCountHeaderColumns = 0;
	char* pPos = pStart;

	//Now we are going to count how many columns we actually have.
	while (pPos != pEnd)
	{
		if (*pPos == cDelimeted)
			iCountHeaderColumns++; //We found a column seperation :)

		pPos++;
	}

	// pPos has now reached pEnd, but we have to add one more column because the amount of columns is +1 in comparison to the amount separation characters
	iCountHeaderColumns++;

	//Header should look similar to:
	//query	seed_ortholog	evalue	score	eggNOG_OGs	max_annot_lvl	COG_category	Description	Preferred_name	GOs	EC	KEGG_ko	KEGG_Pathway	KEGG_Module	KEGG_Reaction	KEGG_rclass	BRITE	KEGG_TC	CAZy	BiGG_Reaction	PFAMs

	//Reset to the beginning again.
	pPos = pStart;

	char* pPosStart = pPos;

	int idx = 0;


	CTableRowHeader* pHeader = new CTableRowHeader;
	lReturn = Table->SetTableRowHeader(pHeader);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	
	CTableColumnHeader* pHeaderColumn = nullptr;

	char* pColumnBuffer = nullptr;
	while (pPosStart != pEnd)
	{
		if ((idx + 1) == iCountHeaderColumns)
		{
			//we are at the last one
			lReturn = SetBufferFromPositions(pPosStart, pEnd, &pColumnBuffer);
			if (lReturn != ERR_NOERROR)
				return lReturn;

			//Create a column header object
			pHeaderColumn = new CTableColumnHeader(pColumnBuffer);

			//Add the column header object into the Header Row object.
			pHeader->PushBack(pHeaderColumn);
			break; //Go out of the loop
		}
		while (*pPos && *pPos != cDelimeted && pPos != pEnd) //Move pointer to the delimeted character
			pPos++;

		//Set the buffer for the column name
		lReturn = SetBufferFromPositions(pPosStart, pPos, &pColumnBuffer);
		if (lReturn != ERR_NOERROR)
			return lReturn;

		//Create a column header object
		pHeaderColumn = new CTableColumnHeader(pColumnBuffer);

		//Add the column header object into the Header Row object.
		pHeader->PushBack(pHeaderColumn);
		idx++; //Increase the column count

		//We have to set the pPos 1 higher than the column delimited character.
		pPos++;

		//We have to set the pPosStart to the start position which is the current pPos
		pPosStart = pPos;

	}

	//Now that we have parsed the header we have to make sure that the amount of columns is the same as the amount that were added.
	if (pHeader->GetVectorSize() != (size_t)iCountHeaderColumns)
		return TABLE_HEADER_VECTOR_AND_COLUMN_COUNT_NOT_MATCHING; //ToDo Error Message.

	//Now we are done with the parsing and can continue with the rows
	return lReturn;
}

long CTable::ParseTable(char* pFileBuffer, char CDelimeted, bool bHeader)
{
	long lReturn = ERR_NOERROR;

	//So we are going to parse a whole table which is delimeted by the character CDelimited. Default is that it's tab delimited.
	//based on bHeader the table has headers or not. As a default the function interpret the first row as Headers.

	//We first check  if the pointer looks at a valid memory address

	if (!pFileBuffer)
		return TABLE_CREATE_FILE_BUFFER; //Todo Error Message

	char* pStart = pFileBuffer;
	char* pPos = pFileBuffer;

	size_t sizeColumns = 0;
	//Okay it exist. So now we are going to check if the table has a headers or not
	if (bHeader == true)
	{
		//We move the pointer of pPos to EOL (end of line)
		MOVE_PTR_TO_EOL_REAL(pPos);
		if (*pPos && *pStart)
		{
			lReturn = CTable::ParseHeader(pStart, pPos, this, CDelimeted);
			if (lReturn != ERR_NOERROR)
				return lReturn;

		}

		MOVE_PTR_TO_BOL(pStart);
		pPos = pStart;

		if (!pStart)
			return TABLE_NO_TABLE_CONTENT;
		sizeColumns = m_pHeader->GetVectorSize();
	}
	else
	{
		//Even if we do not have an header we have to determine how much columns we have!
		MOVE_PTR_TO_EOL_REAL(pPos);
		if (*pPos)
		{
			while (pStart != pPos)
			{
				if (*pStart == CDelimeted)
					sizeColumns++;
				pStart++;
			}
			//We need to add 1 more column because we have 1 extra column for every delimited amount
			sizeColumns++;

			//We have to set the pointers back to the beginning of the file!
			pStart = pFileBuffer;
			pPos = pStart;
		}
		else
			return TABLE_PARSE_TABLE_FORMAT_WRONG_NO_EOL; //ToDo because then we do not have a complete table
	}

	//Get the size of the header so we can double check the amount of columns added for the rows.

	size_t sizeRows = 0;
	sizeRows = sizeRows; //Getting rid of warning


	//Parsed the header (or not). Time to parse all the rows.
	while (*pStart)
	{
		//Move the pointer to EOL
		MOVE_PTR_TO_EOL_REAL(pPos);
		if (*pStart && pStart[0] != '#' && pStart[1] != '#')
		{
			sizeRows = this->GetAmountOfTableRows();
			lReturn = ParseRow(pStart, pPos, sizeColumns, CDelimeted);
			if (lReturn != ERR_NOERROR)
				return lReturn; //ToDo Error Message;
		}
		MOVE_PTR_TO_BOL(pStart);
		pPos = pStart;
	}


#ifdef _DEBUG
	printf("We have %zu rows of Table %s", GetAmountOfTableRows(), EOL);
#endif

	return lReturn;
}

long CTable::ParseTableWithPath(std::string strTablePath, char CDelimeted, bool bHeader)
{
	long lReturn = ERR_NOERROR;
	
	//Set the FileBuffer
	char* pBuffer = nullptr;
	lReturn = SetFileBuffer(&pBuffer, strTablePath);
	if (lReturn != ERR_NOERROR)
		return lReturn;
	CSmartPtr<char> pAutoDelete = pBuffer;


	//Parse the actual table
	lReturn = this->ParseTable(pBuffer, CDelimeted, bHeader);
#ifdef _DEBUG
	printf_s("This table has %zu rows %s", m_vecRows.size(), EOL);
#endif
	if (lReturn != ERR_NOERROR)
		return lReturn;


	return lReturn;
}