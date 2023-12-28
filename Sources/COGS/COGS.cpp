#include "GlobalHelper.h"
CGlobal glb;


long ParseCommandLine(int argc, char* argv[], char* envp[]);
long CheckReturnValue(long lReturn, std::vector<CFastaFile*>* pvecFastaFilesSource, std::vector<CFastaFile*>* pvecFastaFileOrthoGroups, std::vector<CFastaFile*>* pvecFastaFileOrthoGroupsFiltered, std::vector<CTable*>* pvecOrthoGroups, std::vector<std::string*>* pvecOrthoGroupNamesFromSetOneAndTwo, std::vector<std::string*>* pvecOrthoGroupNamesFromSetOneAndTwoFiltered);

long ParseInputFiles(std::vector<CFastaFile*>* pvecFastaFilesSource, std::vector<CTable*>* pvecOrthoGroups, size_t* psizeOrthoGroups, std::vector<std::string*>* pvecOrthoGroupNamesFromSetOneAndTwo, std::vector<std::string*>* pvecOrthoGroupNamesFromSetOneAndTwoFiltered);
long MatchFiles(std::vector<CFastaFile*>* pvecFastaFilesSource, std::vector<CFastaFile*>* pvecFastaFileOrthoGroups, std::vector<CTable*>* pvecOrthoGroups);
long Filtering(std::vector<CFastaFile*>* pvecOrthoGroups, std::vector<CFastaFile*>* pvecOrthoGroupsFiltered, std::vector<std::string*>* pvecOrthoGroupNamesFromSetOneAndTwo);
long WriteOutputFiles(std::vector<CFastaFile*>* pvecFastaFileOrthoGroupsFiltered);
/*
This program combine two orthogorup output sets (based on filtering) and remove the duplicates.
It takes the fasta file names and combine the two sets and remove the duplicate. 
After that it searches the fasta files made based on the orthofinder output (names)

1) ParseCommandLine
2) Parse Input Files (Names only from the fasta files set one and two, parse the fasta files, Parse Orthogroups)
3)  A) Matching. Match the Orthogroups with the base data files
    B) Match the names from set one and two to the orthogroup file names. Those are placed in the output fasta vector
4) Write output file.

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
    std::vector<std::string*> vecOrthoGroupNamesFromSetOneAndTwo;
	std::vector<std::string*> vecOrthoGroupNamesFromSetOneAndTwoFiltered;
    std::vector<CFastaFile*> vecFastaFileSource;
    std::vector<CFastaFile*> vecFastaFileOrthoGroups; //The actual output files, will be filled in step 3
    std::vector<CFastaFile*> vecFastaFileOrthoGroupsFiltered; //vector only containing fasta files that will be written. will be filled in step 4
    std::vector<CTable*> vecOrthogroups; //Multiple files = multiple table files.

    size_t sizeOrthoGroups = 0;

    //STEP TWO
    //Parse Input Files
    lReturn = ParseInputFiles(&vecFastaFileSource, &vecOrthogroups, &sizeOrthoGroups, &vecOrthoGroupNamesFromSetOneAndTwo, &vecOrthoGroupNamesFromSetOneAndTwoFiltered);
    CheckReturnValue(lReturn, &vecFastaFileSource, &vecFastaFileOrthoGroups, &vecFastaFileOrthoGroupsFiltered, &vecOrthogroups, &vecOrthoGroupNamesFromSetOneAndTwo, &vecOrthoGroupNamesFromSetOneAndTwoFiltered);
    CHECK_RETURNVALUE(lReturn);
    
    //STEP THREE
    printf_s("Matching the Fasta blocks to the different orthogroups.%s", EOL);
    lReturn = MatchFiles(&vecFastaFileSource, &vecFastaFileOrthoGroups, &vecOrthogroups);
    CheckReturnValue(lReturn, &vecFastaFileSource, &vecFastaFileOrthoGroups, &vecFastaFileOrthoGroupsFiltered, &vecOrthogroups, &vecOrthoGroupNamesFromSetOneAndTwo, &vecOrthoGroupNamesFromSetOneAndTwoFiltered);
    CHECK_RETURNVALUE(lReturn);
    printf_s("Done with Matching.%s", EOL);

    //STEP FOUR
    printf_s("Filtering the Orthogroups so that only the OrthoGroups matching the fasta File sets are written %s", EOL);
    lReturn = Filtering(&vecFastaFileOrthoGroups, &vecFastaFileOrthoGroupsFiltered, &vecOrthoGroupNamesFromSetOneAndTwoFiltered);
    CheckReturnValue(lReturn, &vecFastaFileSource, &vecFastaFileOrthoGroups, &vecFastaFileOrthoGroupsFiltered, &vecOrthogroups, &vecOrthoGroupNamesFromSetOneAndTwo, &vecOrthoGroupNamesFromSetOneAndTwoFiltered);
    CHECK_RETURNVALUE(lReturn);
    printf_s("Done with Filtering.%s", EOL);


    //STEP FIVE
    printf_s("Write the output files %s", EOL);
    lReturn = WriteOutputFiles(&vecFastaFileOrthoGroupsFiltered);
	CheckReturnValue(lReturn, &vecFastaFileSource, &vecFastaFileOrthoGroups, &vecFastaFileOrthoGroupsFiltered, &vecOrthogroups, &vecOrthoGroupNamesFromSetOneAndTwo, &vecOrthoGroupNamesFromSetOneAndTwoFiltered);
    CHECK_RETURNVALUE(lReturn);
    printf_s("Done with writing the output files%s", EOL);


    //STEP SIX
    //Cleanup
	ClearVector(&vecOrthogroups); //CTable object and rows get deleted       
    vecFastaFileOrthoGroupsFiltered.clear(); //This one is just a pointer forwarding used to write out files.
    ClearVector(&vecFastaFileOrthoGroups); //Fasta File objects get deleted but not the fasta blocks       
	ClearVector(&vecFastaFileSource); //Everything gets deleted including Fasta files and fasta Blocks.
    vecOrthoGroupNamesFromSetOneAndTwoFiltered.clear(); //This one just points towards already existing objects in the vector below
    ClearVector(&vecOrthoGroupNamesFromSetOneAndTwo); //Everything Gets deleted Taxonomic groups and species

    return 0;
}