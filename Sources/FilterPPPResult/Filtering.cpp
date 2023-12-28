#include "GlobalHelper.h"


long FilterPPPOutput(std::vector<CFastaFile*>* pvecFastaFileOrthoGroups, std::vector<CFastaFile*>* pvecFastaFileOrthoGroupsFiltered, CTaxonomicGroup * pvecTaxonomicGroupSorted)
{
	/*
	This function is meant to filter out all the fasta files that does not meet the Taxonomic group threshold.
	All the Orthogroups (fasta file) that meet the requirement are pushed back in the pvecFastaFileOrthoGroupsFiltered.

	1) Define how many taxonomic groups we have and create a size_t vector with hte size of sizeTaxonomicGroups and all are set to 0.
	2) Obtain the user threshold value
	3) Loop over all the fasta files
	4) First check how big the fasta block vector is, if it's smaller than the user threshold instantly go to the next vector item (Fasta file). Since we the amount of fasta blocks is equal or more than the user threshold
	5) If it's equal or bigger than the threshold match the fastablocks to the taxonomic groups. And fill the value of the group ID to 1 within the vector created in step 1.
	6) Sum up all the values in the vector from step 1 and see if it's equal or above the user threshold. If yes than add the fasta file to the filtered vector. if not continue with the next item in the vector
	7) Clean up
	*/
	long lReturn = ERR_NOERROR;

	//STEP ONE
	size_t sizeTaxonomicGroups = 0;
	lReturn = glb.propertyBag.GetTaxonomicGroupVectorSize(&sizeTaxonomicGroups); //Amount of groups withint he taxonomic group
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETTAXONOMICGROUPSIZE_VECTOR_FPPPR;

	//Create the vector and set all the values to 0;
	size_t arrTaxonomicGroupFilter[sizeTaxonomicGroups];
	memset(arrTaxonomicGroupFilter, 0, sizeof(arrTaxonomicGroupFilter));


	//STEP TWO
	size_t sizeThreshold = 0;
	lReturn = glb.propertyBag.GetTaxonomicGroupThreshold(&sizeThreshold); //The User input as a threshold
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_TAXONOMICTHRESHOLD_FPPPR;


	//Init the values needed for this function.
	CTaxonomicSpecies* pTaxonomicSpeciesToFind = nullptr;
	char* szTaxonomicGroupname = nullptr;
	CFastaFile* pOrthoGroup = nullptr;
	CFastaBlock* pFastaBlock = nullptr;
	size_t sizeFastaBlockVector = 0;
	size_t sizeFastaFileOrthoGroups = pvecFastaFileOrthoGroups->size();
	size_t sizeFilteridx = 0;
	size_t sizeSumFilterArr = 0;


	//STEP THREE
	for (size_t idx = 0; idx < sizeFastaFileOrthoGroups; idx++)
	{
#ifdef _DEBUG
		printf_s("Filtering file %zu/%zu%s", idx + 1, sizeFastaFileOrthoGroups, EOL);

#endif
		memset(arrTaxonomicGroupFilter, 0, sizeof(arrTaxonomicGroupFilter)); //Reset the values

		pOrthoGroup = pvecFastaFileOrthoGroups->at(idx); //Set the fasta file

		//Time to check if the fasta file matches
		sizeFastaBlockVector = pOrthoGroup->GetVectorSize();
		//STEP FOUR
		if (sizeFastaBlockVector >= sizeThreshold)
		{
			//Now we know we have the same (or more) amount of headers than the threshold defined by the user.
			//However we now have to determine if we have enough taxonomic group present to be equal or higher than the threshold defined by the user.
			
			//STEP FIVE
			for (size_t idx2 = 0; idx2 < sizeFastaBlockVector; idx2++) //Loop over the fasta block vector
			{
				sizeFilteridx = 0; //Reset it
				pFastaBlock = pOrthoGroup->GetFastaBlock(idx2);
				//Get the strain name from the fasta block
				lReturn = pFastaBlock->GetFastaHeaderStrain(&szTaxonomicGroupname);
				if (lReturn != ERR_NOERROR)
					return lReturn;

				/*
				1) Find the Taxonomic Species based on either Species Name or Header Name.
				2) Set the Taxonomic species you found to ppTaxonomicSpeciesToFind.
				3) Done
				*/
				//STEP ONE and STEP TWO
				lReturn = pvecTaxonomicGroupSorted->FindTaxonomicSpeciesFromHeaderName(szTaxonomicGroupname, &pTaxonomicSpeciesToFind);
				if (lReturn != ERR_NOERROR)
					return lReturn;
	
				//STEP THREE
					
				
				
				lReturn = pTaxonomicSpeciesToFind->GetFilterNumber(&sizeFilteridx);
				if (lReturn != ERR_NOERROR)
					return lReturn;

				arrTaxonomicGroupFilter[sizeFilteridx] = 1;
				


			}

			//STEP SIX
			sizeSumFilterArr = 0; //Reset it
			for (size_t idx3 = 0; idx3 < sizeTaxonomicGroups; idx3++)
				sizeSumFilterArr += arrTaxonomicGroupFilter[idx3];
			

			if (sizeSumFilterArr >= sizeThreshold) //The amount of taxanomic group present is the same or higher for the investigated Fasta file, we can now push back the Fasta file into the Filtered list.
			{
				pvecFastaFileOrthoGroupsFiltered->push_back(pOrthoGroup);
			}

		}
		else
			continue; //The amount of Fasta blocks is lower than the threshold, meaning we have fewer headers than needed so automatically pass.

	}


	//STEP SEVEN




	return lReturn;
}