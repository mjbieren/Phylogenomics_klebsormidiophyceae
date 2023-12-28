#include "GlobalHelper.h"
CGlobal glb;

long CheckReturnValue(long lReturn, std::vector<CFastaFile*>* pvecFastaFilesSource, std::vector<CFastaFile*>* pvecFastaFileFiltered, std::vector<CTaxonomicGroup*>* pvecTaxonomicGroups);
long ParseCommandLine(int argc, char* argv[], char* envp[]);
long ParseInputFiles(std::vector<CFastaFile*>* pvecFastaFilesSource, std::vector<CTaxonomicGroup*>* pvecTaxonomicGroupSource, CTaxonomicGroup* pTaxonomicGroupsSorted);
long FilterPPPOutput(std::vector<CFastaFile*>* pvecFastaFileOrthoGroups, std::vector<CFastaFile*>* pvecFastaFileOrthoGroupsFiltered, CTaxonomicGroup* pvecTaxonomicGroupSorted);
long WriteOutputFiles(std::vector<CFastaFile*>* pvecFastaFileOrthoGroupsFiltered, size_t sizeFastaFilesSource, size_t sizeTaxonomicSpecies);
/*
This function is meant to filter the PhylopyPruner outpout based on how many taxonomic groups the user want to be present.
So if you have 80 species, which are divided by 15 different taxonomic groups and the user says we need 10 at least taxonomic groups

STEP 1 Parse the command line:
Needed arguments:
-f Fasta Files path (PhylopyPruner output)
-t Taxonomic Group file Path
-n Taxonomic group Threshold Value
-r Output folder path
-s Summary File Path
-h Remove Gene IDS
-a Remove Gaps/Alignments


STEP 2 Parse the input files (Taxonomic Group file, Fasta files)
STEP 3 Filter Based on taxonomic group and threshold
STEP 4 Write Output File (Fasta files filtered).
STEP 5 Write Summary File
STEP 6 Cleanup
*/


int main(int argc, char* argv[], char* envp[])
{
	long lReturn = ERR_NOERROR;

    //STEP ONE
    printf_s("Parsing the command line %s", EOL);
    lReturn = ParseCommandLine(argc, argv, envp);
    CHECK_RETURNVALUE(lReturn);
    printf_s("Done with parsing the command line %s", EOL);


    //Set the Variables
    std::vector<CFastaFile*> vecFastaFilesPPPResult; //PPP Result Fasta Files
    std::vector<CFastaFile*> vecFastaFilesFiltered; //Filtered Fasta Files, DO NOT delete the items
    std::vector<CTaxonomicGroup*> vecTaxonomicGroups; //TaxonimGroups where it's stored in.

    
    //Create an object for the taxonomic Group but then sorted, for easy search during the matching.
    CTaxonomicGroup* pTaxonomicGroupSorted = new CTaxonomicGroup;
    //Set the boolean to delete the vector objects to false, since the objects exist in the vecTaxonomicGroups vector and we can't delete an object twice!
    pTaxonomicGroupSorted->SetDeleteTaxonomicSpecies(false);
    //Set a Smart pointer for the object so that we do not have to keep trace of deleting it, it will be deleted automatically :)
    CSmartPtr<CTaxonomicGroup> pAutoDeleteTaxonomicGroup = pTaxonomicGroupSorted;

    //STEP TWO
    printf_s("Parsing the input files %s", EOL);
    lReturn = ParseInputFiles(&vecFastaFilesPPPResult, &vecTaxonomicGroups, pTaxonomicGroupSorted);
    //long CheckReturnValue(long lReturn, std::vector<CFastaFile*>* pvecFastaFilesSource, std::vector<CFastaFile*>* pvecFastaFileFiltered, std::vector<CTaxonomicGroup*>* pvecTaxonomicGroups);
    CheckReturnValue(lReturn, &vecFastaFilesPPPResult, &vecFastaFilesFiltered, &vecTaxonomicGroups);
    CHECK_RETURNVALUE(lReturn);
	printf_s("Done with printing the input files%s", EOL);

    //STEP THREE
    printf_s("Filter the fasta files in the PPP output folder based on the user input threshold %s", EOL);
    lReturn = FilterPPPOutput(&vecFastaFilesPPPResult, &vecFastaFilesFiltered, pTaxonomicGroupSorted);
    CheckReturnValue(lReturn, &vecFastaFilesPPPResult, &vecFastaFilesFiltered, &vecTaxonomicGroups);
    CHECK_RETURNVALUE(lReturn);
    printf("Done with Filtering %s", EOL);


    //STEP FOUR AND FIVE
    printf_s("Writing the output files %s", EOL);
    lReturn = WriteOutputFiles(&vecFastaFilesFiltered, vecFastaFilesPPPResult.size(), pTaxonomicGroupSorted->GetVectorSize());
    CheckReturnValue(lReturn, &vecFastaFilesPPPResult, &vecFastaFilesFiltered, &vecTaxonomicGroups);
    CHECK_RETURNVALUE(lReturn);

    //STEP SIX (Cleaning up
	vecFastaFilesFiltered.clear();
	ClearVector(&vecFastaFilesPPPResult); //Everything gets deleted including Fasta files and fasta Blocks.
    ClearVector(&vecTaxonomicGroups); //Everything Gets deleted Taxonomic groups and species

	printf_s("Done with writing the output files %s Thank you for using 'Filter Phylopypruner Result' :) %s", EOL, EOL);

    return 0;
}