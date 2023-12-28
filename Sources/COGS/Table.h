#pragma once
#include "stdafx.h"
#include "ClearVector.h"
#include "Management.h"

#define TAB_DELIMITED 9
#define COMMA_DELIMITED 44

class CTableColumnHeader
{
private:
	char* m_pHeaderName = nullptr;
	char* m_pHeaderNameNew = nullptr;
	bool m_bAdd = false;
public:
	CTableColumnHeader() {};
	CTableColumnHeader(char* pHeaderName)
	{
		m_pHeaderName = pHeaderName;
	}
	~CTableColumnHeader()
	{
		if(m_pHeaderName)
			delete m_pHeaderName;

		if(m_pHeaderNameNew)
			delete m_pHeaderNameNew;
	};

	long SetHeaderName(char* pStart, char* pEnd)
	{
		long lReturn = ERR_NOERROR;
		if (m_pHeaderName)
			delete m_pHeaderName; //Make Sure that the memory is deleted, we don't want memory leaks

		if (pEnd - pStart <= 0)
			return TABLEROW_SETCOLUMNNAME_HEADER; //The size of the string must be at least 1 or bigger;

		m_pHeaderName = new char[pEnd - pStart + 10]; //Create new buffer
		memset(m_pHeaderName, 0, pEnd - pStart + 10); //Zero terminate everything
		memcpy(m_pHeaderName, pStart, pEnd - pStart); //Copy the HeaderName;

		return lReturn;
	}
	long SetHeaderName(char* pHeaderName)
	{
		if (pHeaderName == nullptr)
			return 1; //Todo Errormessage

		m_pHeaderName = pHeaderName;
	}

	long SetHeaderNameNew(char* pStart, char* pEnd)
	{
		long lReturn = ERR_NOERROR;
		if (m_pHeaderNameNew)
			delete m_pHeaderNameNew; //Make Sure that the memory is deleted, we don't want memory leaks

		if (pEnd - pStart <= 0)
			return TABLEROW_SETCOLUMNNAMENEW_HEADER; //The size of the string must be at least 1 or bigger;

		m_pHeaderNameNew = new char[pEnd - pStart + 10]; //Create new buffer
		memset(m_pHeaderNameNew, 0, pEnd - pStart + 10); //Zero terminate everything
		memcpy(m_pHeaderNameNew, pStart, pEnd - pStart); //Copy the HeaderName;

		return lReturn;
	}

	long SetHeaderNameNew(char* pHeaderNameNew)
	{
		if (!pHeaderNameNew)
			return TABLEROW_SETCOLUMNNAMENEW_HEADER_POINTER;

		m_pHeaderNameNew = pHeaderNameNew;
		return ERR_NOERROR;
	}

	char* GetHeaderName()
	{
		return m_pHeaderName;
	}

	long GetHeaderName(char** ppHeaderName)
	{
		if (!m_pHeaderName)
			return TABLEROW_GETCOLUMNNAME_HEADER;

		*ppHeaderName = m_pHeaderName;
		return ERR_NOERROR;
	}

	char* GetHeaderNameNew()
	{
		return m_pHeaderNameNew;
	}

	long GetHeaderNameNEW(char** ppHeaderNameNew)
	{
		if (m_pHeaderName == nullptr)
			return TABLEROW_GETCOLUMNNAMENEW_HEADER;

		*ppHeaderNameNew = m_pHeaderNameNew;
		return ERR_NOERROR;
	}

	void SetBoolAdd(bool bAdd = true)
	{
		m_bAdd = bAdd;
	}

	bool GetBoolAdd()
	{
		return m_bAdd;
	}

};

class CTableRowHeader
{
private:
	std::vector<CTableColumnHeader*> m_vecTableColumns;
public:

	CTableRowHeader() {};
	~CTableRowHeader() 
	{
		ClearVector<CTableColumnHeader>(&m_vecTableColumns);
	};

	void PushBack(CTableColumnHeader* pHeaderColumn)
	{
		m_vecTableColumns.push_back(pHeaderColumn);
	}

	CTableColumnHeader* GetVectorItem(size_t idx)
	{
		return m_vecTableColumns.at(idx);
	}

	size_t GetVectorSize()
	{
		return m_vecTableColumns.size();
	}

	std::vector<CTableColumnHeader*>* GetHeaderVector()
	{
		return &m_vecTableColumns;
	}

	long SetNewColumn(char* pStart, char* pEnd);
};

class CTable
{
private:
	CTableRowHeader* m_pHeader = nullptr;
	std::vector<std::vector<char*>*> m_vecRows; //Columns are zero terminated!!!
	int m_iColumnToCheck = 0;
	char* m_pTableName = nullptr;
	bool m_bDeleteRows = true;
public:
	CTable() {};
	~CTable()
	{
		if (m_pHeader)
			delete m_pHeader;

		if (m_pTableName)
			delete m_pTableName;

		size_t sizeVec = m_vecRows.size();
		std::vector<char*>* pVecChars = nullptr;
		if (m_bDeleteRows == true)
		{
		//Delete the vector of char vectors
		

		

			for (size_t idx = 0; idx < sizeVec; idx++)
			{
				pVecChars = m_vecRows.at(idx);
				ClearVector<char>(pVecChars);
				delete pVecChars;
			}
			m_vecRows.clear();
		}
		else
		{
			for (size_t idx = 0; idx < sizeVec; idx++)
			{
				pVecChars = m_vecRows.at(idx);
				pVecChars->clear();
				
			}
			m_vecRows.clear();
		}
		
	};

	void SetDeleteRow(bool bDelete = true) { m_bDeleteRows = bDelete; }

	long SetTableName(const char* pStart, const char* pEnd);

	char* GetTableName()
	{
		return m_pTableName;
	}

	long GetTableName(char** ppTableName)
	{
		if (m_pTableName == nullptr)
			return TABLE_GET_TABLE_NAME;

		*ppTableName = m_pTableName;

		return ERR_NOERROR;

	}



	long SetTableRowHeader(CTableRowHeader* pHeader)
	{
		if (pHeader == nullptr)
			return TABLE_SET_TABLE_ROW_HEADER; //Todo Erromessage

		m_pHeader = pHeader;
		return ERR_NOERROR;
	}

	long GetHeaderRowTable(CTableRowHeader** ppHeader)
	{
		if (m_pHeader == nullptr)
			return TABLE_GET_TABLE_ROW_HEADER; //Todo Erromessage

		*ppHeader = m_pHeader;

		return ERR_NOERROR;
	}

	long PushBackRowSafe(std::vector<char*>* pRow)
	{
		if (!pRow)
		{
			return TABLE_PUSHBACK_ROW_SAFE; //Todo ErrorMessage
		}
		m_vecRows.push_back(pRow);

		return ERR_NOERROR;
	}

	void PushBackRow(std::vector<char*>* pRow)
	{
		m_vecRows.push_back(pRow);
	}

	size_t GetAmountOfTableRows() { return m_vecRows.size(); }

	std::vector<char*>* GetRow(size_t idx)
	{
		return m_vecRows.at(idx);
	}

	long PopBack()
	{
		long lReturn = ERR_NOERROR;
		if (m_vecRows.size() == 0)
			return TABLE_POPBACK_NO_ROWS;

		m_vecRows.pop_back();
		return lReturn;
	}

	static std::string StringToUpper(std::string strInput); //To make everything uppercase

	static bool CompareFirstColumnAlphabetically(std::vector<char*>* a, std::vector<char*>* b);

	//This means that you can use this function for multiple type of delimited files. by default it use the tab delimated format (tsv) and searches automatically for /t (TAB = ASCII 9)
	long ParseRow(char* pStart, char* pEnd, size_t sizeHeader, char CDelimeted = TAB_DELIMITED);
	long ParseTable(char* pFileBuffer, char CDelimeted = TAB_DELIMITED, bool bHeader = true);
	long ParseTableWithPath(std::string strTablePath, char Delimeted = TAB_DELIMITED, bool bHeader = true);
	static long ParseHeader(char* pStart, char* pEnd, CTable* Table, char cDelimeted = TAB_DELIMITED);

	void SortTableRowsAlphabetically(int iColumnToMatch = 0);

	const long FindMatchingRow(char* szMatchingName, std::vector<char*>** ppRow, size_t sizeRowToMatch = 1);

};

