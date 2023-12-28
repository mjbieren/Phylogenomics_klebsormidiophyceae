#include "GlobalHelper.h"
CGlobal glb;

long ParseCommandLine(int argc, char* argv[], char* envp[]);
long CheckReturnValue(long lReturn, std::vector<CTaxonomicGroup*>* pvecTaxonomicGroups);
long ParseTaxonomicGroups(std::vector<CTaxonomicGroup*>* pvecTaxonomicGroup, CTaxonomicGroup* pTaxonomicGroupsSorted);
long ReformatTreeFiles(CTaxonomicGroup* pTaxonomicGroupSorted);
long MoveMafftFiles();

/*This program changes the format of the species within the trees to the PhyloPyPruner format SAGxxx_1234->SAGxxx@1234.

STEP 1) Parse the Command Line
-Tree file format
-Directory with all the Trees
-TaxonomicGroup File Path
-Output Folder Path
-Move Mafft files (optional)

STEP 2) Parse the TaxonomicGroup File and make them easily accessible (to search).
STEP 3) Apply the PPP format to the tree files and write them to the output folder, be sure to rename them from .mafft.treefile to .treefile.
STEP 4) (Optional) Move the .mafft files to the output folder and rename it to .fa

*/



int main(int argc, char* argv[], char* envp[])
{
    long lReturn = ERR_NOERROR;

    //STEP ONE
    printf_s("Parsing the command line %s", EOL);
    lReturn = ParseCommandLine(argc, argv, envp);
    CHECK_RETURNVALUE(lReturn);
    printf_s("Done with parsing the command line%s", EOL);

    //Set the variables. 
    //we have a vector of taxonomic groups and an taxonomic group object (pTaxnomicGroupObject) for quick search for matching.
    std::vector<CTaxonomicGroup*> vecTaxonomicGroups; //Taxonomic Group file split in different groups. The amount of objects in the vectors = how many taxonomic group sthere is in total. Do not confuse that with how many there need to be.
    //Create an object for the taxonomic Group but then sorted, for easy search during the matching.
    CTaxonomicGroup* pTaxonomicGroupSorted = new CTaxonomicGroup;
    //Set the boolean to delete the vector objects to false, since the objects exist in the vecTaxonomicGroups vector and we can't delete an object twice!
    pTaxonomicGroupSorted->SetDeleteTaxonomicSpecies(false);
    //Set a Smart pointer for the object so that we do not have to keep trace of deleting it, it will be deleted automatically :)
    CSmartPtr<CTaxonomicGroup> pAutoDeleteTaxonomicGroup = pTaxonomicGroupSorted;

    //STEP TWO
    printf_s("Parsing the taxonomic group file into objects.%s", EOL);
    lReturn = ParseTaxonomicGroups(&vecTaxonomicGroups, pTaxonomicGroupSorted);
    CheckReturnValue(lReturn, &vecTaxonomicGroups);
    CHECK_RETURNVALUE(lReturn);
    printf_s("Done with parsing the Taxonomic Group File.%s", EOL);


    //STEP THREE
    printf_s("Reformatting the Tree files %s", EOL);
    lReturn = ReformatTreeFiles(pTaxonomicGroupSorted);
    CheckReturnValue(lReturn, &vecTaxonomicGroups);
    CHECK_RETURNVALUE(lReturn);
    printf_s("Done with reformatting Tree Files %s", EOL);

    //STEP FOUR
    //Check if the bool is set to true
    bool bMoveMafftFiles = false;
    lReturn = glb.propertyBag.GetMoveMafftFile(&bMoveMafftFiles);
    CheckReturnValue(lReturn, &vecTaxonomicGroups);
    CHECK_RETURNVALUE(lReturn);

    if (bMoveMafftFiles == true) //We have to move them.
    {
        printf_s("Moving the msa files to the output folder %s", EOL);
        lReturn = MoveMafftFiles();
        CheckReturnValue(lReturn, &vecTaxonomicGroups);
        CHECK_RETURNVALUE(lReturn);
        printf_s("Done with moving the msa files %s", EOL);

    }

    //Cleaning up
	 ClearVector(&vecTaxonomicGroups);

	 printf_s("Done with writing the output files %s Thank you for using 'Apply PhylopyPruner Format' :) %s", EOL, EOL);



    return 0;
}