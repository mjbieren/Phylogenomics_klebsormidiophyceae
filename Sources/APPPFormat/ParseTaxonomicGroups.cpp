#include "GlobalHelper.h"

long SetFileBuffer(char** pPos, std::string strFileName);

long ParseTaxonomicGroup(CTaxonomicGroup* pTaxonomicGroup, const char* pStart, const char* pEnd)
{
	//SpeciesName(HeaderName),SpeciesName2(HeaderName2),etc, EOL			#group 1
	
	long lReturn = ERR_NOERROR;

	const char* pPosStart = pStart;
	const char* pPos = pStart;
	const char* pPosEnd = pStart;

	CTaxonomicSpecies* pSpecies = nullptr;

	while (pPosEnd != pEnd)
	{
		while (*pPosEnd != ',' && pPosEnd != pEnd)		
			pPosEnd++;

		if (pPosEnd == pEnd) //Found the last one
			{
				//To make sure we don't end with a , at the end aka "SpeciesName(HeaderName),SpeciesName2(HeaderName2),"
				while (*pPos != '(' && pPos != pEnd)
					pPos++;

				if(pPos == pEnd) //Ended with a , at the end and not a header/species name.
					break;

				//Didn't end with a , instead of a ( meaning a headername start.
				pSpecies = new CTaxonomicSpecies;
				lReturn = pSpecies->SetSpeciesName(pPosStart, pPos - pPosStart);
				if (lReturn != ERR_NOERROR)
				{
					delete pSpecies;
					return lReturn;
				}


				pPosStart = pPos + 1; //we want to end it with 1 further than the (.
				
				while (*pPos != ')' && pPos != pEnd)
					pPos++;

				if (pPos == pEnd)
					{
						delete pSpecies;
						return PARSE_INPUT_FILE_TAXONOMICGROUPFILE_WRONG_FORMAT;//Error cuz we didn't end it at the right one.
					}

				lReturn = pSpecies->SetHeaderName(pPosStart, pPos - pPosStart);
				if (lReturn != ERR_NOERROR)
				{
					delete pSpecies;
					return lReturn;
				}
					
				//Push Back the taxonomic group
				pTaxonomicGroup->PushBack(pSpecies);

			}
		else //It's not the end
		{
			while (*pPos != '(' && pPos != pPosEnd)
				pPos++;
			
			if (pPos == pPosEnd)
				return PARSE_INPUT_FILE_TAXONOMICGROUPFILE_WRONG_FORMAT;
			
			pSpecies = new CTaxonomicSpecies;
			lReturn = pSpecies->SetSpeciesName(pPosStart, pPos - pPosStart);
			if (lReturn != ERR_NOERROR)
				{
					delete pSpecies;
					return lReturn;
				}

			pPosStart = pPos + 1; //we want to end it with 1 further than the (.

			while (*pPos != ')' && pPos != pPosEnd)
				pPos++;

			if (pPos == pPosEnd)
				return PARSE_INPUT_FILE_TAXONOMICGROUPFILE_WRONG_FORMAT;

			lReturn = pSpecies->SetHeaderName(pPosStart, pPos - pPosStart);
			if (lReturn != ERR_NOERROR)
				{
					delete pSpecies;
					return lReturn;
				}

			//Set them all time to push it back in the taxonomic group
			
			pTaxonomicGroup->PushBack(pSpecies);


			//move the pointers
			pPosEnd++;
			pPosStart = pPosEnd;
			pPos = pPosEnd;

		}



	}

	return lReturn;
}


long ParseTaxonomicGroups(std::vector<CTaxonomicGroup*> * pvecTaxonomicGroup, CTaxonomicGroup* pTaxonomicGroupsSorted)
{
	//This function parses the taxonomic group file into different taxonomic group objects and then add all the species into one taxonomic group sorted alphabetically.
	/*
	1) Parse the taxonomic group file into different taxonomic groups. 1 line = 1 group and push it back into one taxonomic group vector
	2) Set the idx of all taxanomic species to the same value in one taxonomic group.
	3) Parse all taxonomic species of each group into one taxanomic group object (everything in one object).
	4) Sort the taxonomic species vector within the taxonomic group sorted object alphabetically

	SpeciesName(HeaderName),SpeciesName2(HeaderName2),etc, EOL			#group 1
	SpeciesName3(HeaderName3),SpeciesName4(HeaderName4), etc, EOL		#group 2

	Every line is 1 HeadersFilters belonging to 1 group, every species within it belongs to 1 headerFilter.
	
	*/
	long lReturn = ERR_NOERROR;

	std::string strTaxonomicGroupFile;

	lReturn = glb.propertyBag.GetTaxonomicGroupFilePath(&strTaxonomicGroupFile);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETTAXONOMICGROUPFILE_APPPF;

	//Get the file buffer;
	char* pBuffer = nullptr;
	lReturn = SetFileBuffer(&pBuffer, strTaxonomicGroupFile);
	CSmartPtr<char> pAutoDelete = pBuffer;
	if (lReturn != ERR_NOERROR)
		return lReturn;

	char* pPos = pBuffer;
	char* pEnd = pBuffer;

	CTaxonomicGroup* pTaxonomicGroup = nullptr;


	//STEP ONE
	while (*pPos)
	{
		MOVE_PTR_TO_EOL_REAL(pEnd); //Looking at EOL or EOF. But not the one before
		pTaxonomicGroup = new CTaxonomicGroup;
		lReturn = ParseTaxonomicGroup(pTaxonomicGroup, pPos, pEnd);
		if (lReturn != ERR_NOERROR)
		{
			delete pTaxonomicGroup;
			return lReturn;
		}

		pvecTaxonomicGroup->push_back(pTaxonomicGroup);
		MOVE_PTR_TO_BOL(pEnd);
		pPos = pEnd;

	}

	//STEP TWO & STEP THREE
	size_t sizeTaxonomicGroupVector = pvecTaxonomicGroup->size();

	size_t sizeTaxonomicSpecies = 0;
	CTaxonomicSpecies* pSpecies = nullptr;

	for (size_t idx = 0; idx < sizeTaxonomicGroupVector; idx++)
	{
		pTaxonomicGroup = pvecTaxonomicGroup->at(idx);
		pTaxonomicGroup->SetDeleteTaxonomicSpecies(); //Also set that we delete the items, since this is the original.
		sizeTaxonomicSpecies = pTaxonomicGroup->GetVectorSize();
		for (size_t idx2 = 0; idx2 < sizeTaxonomicSpecies; idx2++)
		{
			pSpecies = pTaxonomicGroup->GetSpeciesVectorItem(idx2);
			pSpecies->SetFilterNumber(idx);
			pTaxonomicGroupsSorted->PushBack(pSpecies);
		}
	}



	//STEP FOUR
	lReturn = pTaxonomicGroupsSorted->SortTaxonomicGroupAlphabeticallyHeaders();
	return lReturn;
}