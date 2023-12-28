
#include "GlobalHelper.h"

long SplitHeaderNameAndNumber(char* szToMatch, char** ppHeader, unsigned long* ulHeaderNumber, size_t* psizeBuffer);
long FindMatchingFastaFile(std::vector<CFastaFile*>* pvecGenesFastaFile, const char* szToMatch, CFastaFile** ppFastaFileMatched, bool bDecontaminant = false, bool bSplitHeader = false);

long ParseDecontaminationFile(std::vector<CFastaFile*>* pvecFastaFiles)
{
	//Step 1 Get File path for decontamination file
	//Step 2 Parse the file into a table
	//Step 3 loop over it to create Fasta Files
	//Step 4 Order the fasta files alphabetically.

	long lReturn = ERR_NOERROR;

	//STEP ONE
	std::string strFilePath;
	lReturn = glb.propertyBag.GetDecontaminationListPath(&strFilePath);

	CTable TableDecontamination;

	//STEP TWO
	lReturn = TableDecontamination.ParseTableWithPath(strFilePath, TAB_DELIMITED, false);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	//STEP THREE
	size_t sizeTableRows = TableDecontamination.GetAmountOfTableRows();
	CFastaFile* pFastaFile = nullptr;

	std::vector<char*>* pRow;

	char* pDecontaminant = nullptr;

	for (size_t idx = 0; idx < sizeTableRows; idx++)
	{
		pRow = TableDecontamination.GetRow(idx);
		pFastaFile = new CFastaFile;
		//Set the matching file where we compare our blast output file to
		lReturn = pFastaFile->SetMatchingName(pRow->at(0));
		if (lReturn != ERR_NOERROR)
		{
			delete pFastaFile;
			return lReturn;
		}

		//Set if the fasta file will be a decontaminant or not (e.g. a decontaminant is Bacteria when you want only a positive set for Klebso
		pDecontaminant = pRow->at(1);
		if (*pDecontaminant == '+')
			pFastaFile->SetDecontaminant(false);
		else
			pFastaFile->SetDecontaminant(true);


		//Set the Fasta file, the fasta file name we want to combine.
		lReturn = pFastaFile->SetFastaFileName(pRow->at(2));
		if (lReturn != ERR_NOERROR)
		{
			delete pFastaFile;
			return lReturn;
		}


		//Put it in the vector
		pvecFastaFiles->push_back(pFastaFile);
		pFastaFile->SortFastaFileAlphabetically();
		


	}

	//Sort our fasta files alphabetically
	std::sort(pvecFastaFiles->begin(), pvecFastaFiles->end(), [](CFastaFile* a, CFastaFile* b)
		{	
			//I know this is expensive but it otherwise not sorted correctly with pointers alone!
			std::string strFirstColumnA = a->GetMatchingName();
			std::string strFirstColumnB = b->GetMatchingName();

			std::string a_name = CTable::StringToUpper(strFirstColumnA);
			std::string b_name = CTable::StringToUpper(strFirstColumnB);
			
			return (a_name < b_name);
		});


	return lReturn;
}

long FilterBlastOutput(CTable* pBlastFile, CTable* pBlastFiltered, std::vector<CFastaFile*> * pvecFastaOutputFiles)
{
	//This function filters the blast file, so only the best results remains.
	//Blast output should already be ordered.

	//Step 1 get the size of how many rows
	//Step 2 Parse the first one
	//	Check if there it's a strict or non strict decontamination.
	//Step 3 Compare old vs new row if old == new, ignore new
	//Step 4 If it is not equal parse the old one into the filtered table and make from now -> old.


	long lReturn = ERR_NOERROR;

	//STEP ONE
	size_t sizeTableFull = pBlastFile->GetAmountOfTableRows();

	std::vector<char*>* pRowOld = nullptr;
	std::vector<char*>* pRowNew = nullptr;


	//STEP TWO
	if (sizeTableFull > 0)
	{
		pRowOld = pBlastFile->GetRow(0);
		pRowNew = pBlastFile->GetRow(1);
		lReturn = pBlastFiltered->PushBackRowSafe(pRowNew);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	else //Nothing in the vector
		return TABLE_PUSHBACK_NO_ROWS;

	bool bDecontaminationStrict = false;
	lReturn = glb.propertyBag.GetBoolStrictDecontamination(&bDecontaminationStrict);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETSTRICTDECONTAMINATION;

	CFastaFile* pFastaFileOld = nullptr;
	CFastaFile* pFastaFileNew = nullptr;

	if (sizeTableFull > 1)
	{
		for (size_t idx = 1; idx < sizeTableFull; idx++)
		{
			pRowNew = pBlastFile->GetRow(idx);
			//STEP THREE
			if (memcmp(pRowNew->at(0),pRowOld->at(0), strlen(pRowOld->at(0))) == 0)
			{
				//Same one
				//Check if a it's matched to a positive set if yes then we have to do a pop out.
				if (bDecontaminationStrict == false)
				{
					lReturn = FindMatchingFastaFile(pvecFastaOutputFiles, pRowOld->at(1), &pFastaFileOld, true, true);
					if (lReturn != ERR_NOERROR)
						return lReturn;

					lReturn = FindMatchingFastaFile(pvecFastaOutputFiles, pRowNew->at(1), &pFastaFileNew, true, true);
					if (lReturn != ERR_NOERROR)
						return lReturn;

					//Check first if they are the same!
					if (pFastaFileOld == pFastaFileNew)
						continue; //We do not have to add the same protein twice in the same file.
					else
					{
						//If they are different there are four scenarios. 
						//1) both decontaminant nothing happens
						//2) They are both positive sets, nothing will happens either (best matching is the most likely)
						//3) Old is a decontaminant but the new one is a positive set. This means old get kicked out in the filtered table and replaced
						//4) Old is positive, but new is decontaminat. Nothing happens.

						if (pFastaFileOld->GetDecontaminant() == false) //Scenario 1 & 4.
							continue;
						else
						{
							if (pFastaFileOld->GetDecontaminant() == false) //Scenario 3
							{
								lReturn = pBlastFiltered->PopBack();
								if (lReturn != ERR_NOERROR)
									return lReturn;
								lReturn = pBlastFiltered->PushBackRowSafe(pRowNew);
								if (lReturn != ERR_NOERROR)
									return lReturn;
								pRowOld = pRowNew;
							}
							else //Scenario 2
								continue;
						}
					}
					//If they are not

					
				}
				else
				{
					continue;
				}
			}
			else
			{
				//Different one
				//STEP FOUR
				lReturn = pBlastFiltered->PushBackRowSafe(pRowNew);
				if (lReturn != ERR_NOERROR)
					return lReturn;
				pRowOld = pRowNew;

			}
		}	
	}

	//Since we use pointers, we have to make sure that the filtered table doesn't try to clean up the objects again!
	pBlastFiltered->SetDeleteRow(false);
	 
	//Done with filtering
	return lReturn;
}

long ParseBlastFileAndFilter(CTable* pBlastFile, CTable* pBlastFileFiltered, std::vector<CFastaFile*>* pvecFastaOutputFiles)
{
	//Step 1 Get the file path
	//Step 2 Parse the table into the pBlastFile object
	//Step 3 filter the pBlastFile object and place the filtered object into pBlastFileFiltered
	//Step 4 Done
	long lReturn = ERR_NOERROR;

	//Step 1
	std::string strFilePath;
	lReturn = glb.propertyBag.GetBlastFilePath(&strFilePath);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	//Step 2
	lReturn = pBlastFile->ParseTableWithPath(strFilePath, TAB_DELIMITED, false);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	//Step 3
	lReturn = FilterBlastOutput(pBlastFile, pBlastFileFiltered, pvecFastaOutputFiles);

	return lReturn;
}

long ParseInputFiles(std::vector<CFastaFile*>* pvecFastaOutputFiles, CFastaFile* pFastaQuery, CTable* pBlastFile, CTable* pBlastFileFiltered)
{
	//Step 1 Parse FastaFile
	//Step 3 Parse BlastFile and filter it
	//Step 2 parse Decontamination file

	long lReturn = ERR_NOERROR;
	std::string strFilePath;

	//STEP ONE PARSE FASTA QUERY FILE
	printf_s("Parsing the Fasta Query File %s", EOL);
	//Get the File Path
	lReturn = glb.propertyBag.GetFastaQuery(&strFilePath);
	if (lReturn != ERR_NOERROR)
		return lReturn;
	//Parse the fasta file
	lReturn = pFastaQuery->ParseFastaFile(strFilePath.c_str());
	if (lReturn != ERR_NOERROR)
		return lReturn;
	//Order the fasta file alphabetically
	lReturn = pFastaQuery->SortFastaFileAlphabetically();
#ifdef _DEBUG
	pFastaQuery->PrintHeadersToScreen();
#endif
	if (lReturn != ERR_NOERROR)
		return lReturn;

	
	printf_s("Done with parsing the Fasta Query File%s", EOL);

	//STEP TWO PARSE DECONTAMINATION FILE
	printf_s("Parsing the decontamination file and sort it alphabetically %s", EOL);
	lReturn = ParseDecontaminationFile(pvecFastaOutputFiles);
	if (lReturn != ERR_NOERROR)
		return lReturn;
	printf_s("Done with parsing the decontamination file %s", EOL);



	//STEP THREE PARSE BLAST FILE AND FILTER IT
	printf_s("Parsing the blast output files and filter it %s", EOL);
	lReturn = ParseBlastFileAndFilter(pBlastFile, pBlastFileFiltered,pvecFastaOutputFiles);
	if (lReturn != ERR_NOERROR)
		return lReturn;
	printf_s("Done with parsing the blast output file.");


	





	return lReturn;
}