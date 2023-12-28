#include "GlobalHelper.h"
CGlobal glb;


long ParseCommandLine(int argc, char* argv[], char* envp[]);
long CheckReturnValue(long lReturn, std::vector<CFastaFile*>* pvecFastaFilesSource, std::vector<CFastaFile*>* pvecFastaFileOrthoGroups, std::vector<CFastaFile*>* pvecFastaFileOrthoGroupsFiltered, std::vector<CTable*>* pvecOrthoGroups, std::vector<CTaxonomicGroup*>* pvecTaxonomicGroups);
long ParseInputFiles(std::vector<CFastaFile*>* pvecFastaFilesSource, std::vector<CTable*>* pvecOrthoGroups, size_t* psizeOrthoGroups, std::vector<CTaxonomicGroup*>* pvecTaxonomicGroups, CTaxonomicGroup* pTaxonomicGroupsSorted);
long MatchFiles(std::vector<CFastaFile*>* pvecFastaFilesSource, std::vector<CFastaFile*>* pvecFastaFileOrthoGroups, std::vector<CTable*>* pvecOrthoGroups);
long FilterOrthoGroupsOutput(std::vector<CFastaFile*>* pvecFastaFileOrthoGroups, std::vector<CFastaFile*>* pvecFastaFileOrthoGroupsFiltered, CTaxonomicGroup* pvecTaxonomicGroupSorted);
long WriteOutputFiles(std::vector<CFastaFile*>* pvecFastaFileOrthoGroupsFiltered, size_t sizeTotalOrthoGroups, size_t sizeFastaFilesSource);

/*
This program is meant to obtain all orthogroups that contain at least a number of taxonomic groups that the user specifies
So if you have 80 species, which are divided by 15 different taxonomic groups and the user says we need 10 at least taxonomic groups
only the orthogroups with at least 10 different taxonomic groups (NOT! species) have to be present.
which can mean 10 dfiferent species but also 50 different species or more!.


STEP 1 Parse the command line
Needed arguments:
-f Fasta Files Path
-g OrthoGroups Path
-r Output Folder Path
-t Taxonomic Group Path
-n Taxonomic Group Threshold Value
-p PhyloPyPruner Filter

STEP 2 Parse the input files (Taxonomic group file, fasta files, orthogroup files
STEP 3 Match Fasta blocks with the orthogroup members
STEP 4 Filter based on User Input (Basically set Write fasta bool to true or false
STEP 5 Write the Orthogroup fasta files. Be sure to replace the . in the orthogroup files to NO.00001 to NO_000001. Since A lot of programs do not like a .
STEP 6 Write a Summary file. (How many orthogroups there were, how much was actually filtered and what the taxonomic group threshold was.
STEP 7 Cleanup





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
    std::vector<CFastaFile*> vecFastaFilesSource; //The original fasta files, you use during orthofinder
    std::vector<CFastaFile*> vecFastaFileOrthoGroups; //The actual output files, will be filled in step 3
    std::vector<CFastaFile*> vecFastaFileOrthoGroupsFiltered; //vector only containing fasta files that will be written. will be filled in step 4
    std::vector<CTable*> vecOrthogroups; //Multiple files = multiple table files.
    std::vector<CTaxonomicGroup*> vecTaxonomicGroups; //Taxonomic Group file split in different groups. The amount of objects in the vectors = how many taxonomic group sthere is in total. Do not confuse that with how many there need to be.
    
    size_t sizeTotalOrthoGroups = 0; //Needed for the summary File
    
    //Create an object for the taxonomic Group but then sorted, for easy search during the matching.
    CTaxonomicGroup* pTaxonomicGroupSorted = new CTaxonomicGroup;
    //Set the boolean to delete the vector objects to false, since the objects exist in the vecTaxonomicGroups vector and we can't delete an object twice!
    pTaxonomicGroupSorted->SetDeleteTaxonomicSpecies(false);
    //Set a Smart pointer for the object so that we do not have to keep trace of deleting it, it will be deleted automatically :)
    CSmartPtr<CTaxonomicGroup> pAutoDeleteTaxonomicGroup = pTaxonomicGroupSorted;


    //STEP TWO
    printf_s("Parsing the input files %s", EOL);
    lReturn = ParseInputFiles(&vecFastaFilesSource, &vecOrthogroups, &sizeTotalOrthoGroups, &vecTaxonomicGroups, pTaxonomicGroupSorted);
    CheckReturnValue(lReturn, &vecFastaFilesSource, &vecFastaFileOrthoGroups, &vecFastaFileOrthoGroupsFiltered, &vecOrthogroups, &vecTaxonomicGroups);
	CHECK_RETURNVALUE(lReturn);

    printf_s("Done with printing the input files%s", EOL);

    //STEP THREE
    printf_s("Matching the Fasta blocks to the different orthogroups.%s", EOL);
    lReturn = MatchFiles(&vecFastaFilesSource, &vecFastaFileOrthoGroups, &vecOrthogroups);
    CheckReturnValue(lReturn, &vecFastaFilesSource, &vecFastaFileOrthoGroups, &vecFastaFileOrthoGroupsFiltered, &vecOrthogroups, &vecTaxonomicGroups); 
    CHECK_RETURNVALUE(lReturn);
    printf_s("Done with Matching.%s", EOL);


    //STEP FOUR
    printf_s("Filter the Orthogroup fasta file based on the user input threshold %s", EOL);
    lReturn = FilterOrthoGroupsOutput(&vecFastaFileOrthoGroups, &vecFastaFileOrthoGroupsFiltered, pTaxonomicGroupSorted);
    CheckReturnValue(lReturn, &vecFastaFilesSource, &vecFastaFileOrthoGroups, &vecFastaFileOrthoGroupsFiltered, &vecOrthogroups, &vecTaxonomicGroups); 
    printf("Done with Filtering %s", EOL);

    //STEP FIVE AND SIX
    printf_s("Writing the output files %s", EOL);
    lReturn = WriteOutputFiles(&vecFastaFileOrthoGroupsFiltered, sizeTotalOrthoGroups, vecFastaFilesSource.size());
	CheckReturnValue(lReturn, &vecFastaFilesSource, &vecFastaFileOrthoGroups, &vecFastaFileOrthoGroupsFiltered, &vecOrthogroups, &vecTaxonomicGroups); 
    CHECK_RETURNVALUE(lReturn);
    printf_s("Done with Writing the output files %s Thank you for using OrthoGroup Sequence Grabber :) %s", EOL, EOL);

	//Cleaning up Time
	ClearVector(&vecOrthogroups); //CTable object and rows get deleted       
    vecFastaFileOrthoGroupsFiltered.clear(); //objects are existing in the vecFastaFileOrthoGroups but we have to clear the vector.
    ClearVector(&vecFastaFileOrthoGroups); //Fasta File objects get deleted but not the fasta blocks
   	ClearVector(&vecFastaFilesSource); //Everything gets deleted including Fasta files and fasta Blocks.
    ClearVector(&vecTaxonomicGroups); //Everything Gets deleted Taxonomic groups and species

    



    return 0;
}