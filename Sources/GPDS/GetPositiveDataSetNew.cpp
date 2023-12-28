#include "GlobalHelper.h"
CGlobal glb;


long ParseCommandLine(int argc, char* argv[], char* envp[]);
long CheckReturnValue(long lReturn, std::vector<CFastaFile*>* pvecFastaFilesOutputFiles, CFastaFile* pFastaFileQuery, CTable* pBlastFile, CTable* pBlastFileFiltered);
long ParseInputFiles(std::vector<CFastaFile*>* pvecFastaOutputFiles, CFastaFile* pFastaQuery, CTable* pBlastFile, CTable* pBlastFileFiltered);
long MatchingFiles(std::vector<CFastaFile*>* pvecFastaOutputFiles, CTable* pBlastFiltered, CFastaFile* pFastaQuery);
long WriteOutputFiles(std::vector<CFastaFile*>* pvecFastaOutputFiles);


/*
This program is used to obtain multiple fasta files out of a blast output file, Where the 2nd row is matched to a decontamination database.
Input:
-Fasta File
-Bast output file
-Decontamination list ( a list of which prefixs are positive or negative)

-i Input Decontamination file liste>
-f Fasta file
-b BlastOutputFiles
-c Blast ColumnNumber
-s StrainName
-r OutputPath

At the end for every item in the decontamination list a fasta file is created. E.g. Bacteria.fa,, Fungi.fa etc.

1) Parse Command Line
2) Parse Input files (Decontamination file, Blast output file, Fasta file
3) Match blast output to fasta file and place them in the decontamination fasta vector
4) Write Fasta output files
5) Clean up.


*/
int main(int argc, char* argv[], char* envp[])
{
    long lReturn = ERR_NOERROR;

    //STEP ONE PARSE COMMAND LINE
	lReturn = ParseCommandLine(argc, argv,envp);
    CHECK_RETURNVALUE(lReturn);


   //Initiate the different vectors needed for the program
    std::vector<CFastaFile*> vecFastaOutputFiles;
    CFastaFile* pFastaFile = new CFastaFile;
    CTable* pBlastFile = new CTable;
    CTable* pBlastFileFiltered = new CTable;


    //STEP TWO PARSE THE INPUT FILES
    printf_s("Parsing the input files %s", EOL);
    lReturn = ParseInputFiles(&vecFastaOutputFiles, pFastaFile, pBlastFile, pBlastFileFiltered);
    CheckReturnValue(lReturn, &vecFastaOutputFiles, pFastaFile, pBlastFile, pBlastFileFiltered);
    CHECK_RETURNVALUE(lReturn);
    printf_s("Done with Parsing the input files %s", EOL);

    //STEP THREE MATCH THE FILES
    printf_s("Match the decontamination headers with the fasta headers %s", EOL);
    lReturn = MatchingFiles(&vecFastaOutputFiles, pBlastFileFiltered, pFastaFile);
	CheckReturnValue(lReturn, &vecFastaOutputFiles, pFastaFile, pBlastFile, pBlastFileFiltered);
    CHECK_RETURNVALUE(lReturn);
    printf_s("Done with Matching %s", EOL);

    //STEP FOUR WRITE THE OUTPUT FILES
    printf_s("Creating the different fasta files%s", EOL);
    lReturn = WriteOutputFiles(&vecFastaOutputFiles);
	CheckReturnValue(lReturn, &vecFastaOutputFiles, pFastaFile, pBlastFile, pBlastFileFiltered);
    CHECK_RETURNVALUE(lReturn);
    printf_s("Done with creating the different fasta files%", EOL);

    //STEP 5
    ClearVector<CFastaFile>(&vecFastaOutputFiles);
    delete pFastaFile;
    delete pBlastFile;
    delete pBlastFileFiltered;

    return 0;
}