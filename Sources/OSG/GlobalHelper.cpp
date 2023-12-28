#include "GlobalHelper.h"

long CheckReturnValue(long lReturn, std::vector<CFastaFile*>* pvecFastaFilesSource,std::vector<CFastaFile*>* pvecFastaFileOrthoGroups, std::vector<CFastaFile*> * pvecFastaFileOrthoGroupsFiltered, std::vector<CTable*> * pvecOrthoGroups, std::vector<CTaxonomicGroup*> *pvecTaxonomicGroups)
{
    if (lReturn != ERR_NOERROR)
    {
		ClearVector(pvecOrthoGroups); //CTable object and rows get deleted       
        pvecFastaFileOrthoGroupsFiltered->clear();
        ClearVector(pvecFastaFileOrthoGroups); //Fasta File objects get deleted but not the fasta blocks       
		ClearVector(pvecFastaFilesSource); //Everything gets deleted including Fasta files and fasta Blocks.
        ClearVector(pvecTaxonomicGroups); //Everything Gets deleted Taxonomic groups and species

       
    }

    return lReturn;

}