#include "GlobalHelper.h"

long CheckReturnValue(long lReturn, std::vector<CFastaFile*>* pvecFastaFilesSource,std::vector<CFastaFile*>* pvecFastaFileOrthoGroups, std::vector<CFastaFile*> * pvecFastaFileOrthoGroupsFiltered, std::vector<CTable*> * pvecOrthoGroups, std::vector<std::string*> * pvecOrthoGroupNamesFromSetOneAndTwo, std::vector<std::string*>* pvecOrthoGroupNamesFromSetOneAndTwoFiltered)
{
    if (lReturn != ERR_NOERROR)
    {
		ClearVector(pvecOrthoGroups); //CTable object and rows get deleted       
        pvecFastaFileOrthoGroupsFiltered->clear(); //This one is just a pointer forwarding used to write out files.
        ClearVector(pvecFastaFileOrthoGroups); //Fasta File objects get deleted but not the fasta blocks       
		ClearVector(pvecFastaFilesSource); //Everything gets deleted including Fasta files and fasta Blocks.
        pvecOrthoGroupNamesFromSetOneAndTwoFiltered->clear(); //This one just points towards already existing objects in the vector below
        ClearVector(pvecOrthoGroupNamesFromSetOneAndTwo); //Everything Gets deleted Taxonomic groups and species
       
    }

    return lReturn;

}