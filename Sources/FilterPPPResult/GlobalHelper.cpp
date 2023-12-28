#include "GlobalHelper.h"

long CheckReturnValue(long lReturn, std::vector<CFastaFile*>* pvecFastaFilesSource, std::vector<CFastaFile*> * pvecFastaFileFiltered, std::vector<CTaxonomicGroup*> *pvecTaxonomicGroups)
{
    if (lReturn != ERR_NOERROR)
    {
  
        pvecFastaFileFiltered->clear();
		ClearVector(pvecFastaFilesSource); //Everything gets deleted including Fasta files and fasta Blocks.
        ClearVector(pvecTaxonomicGroups); //Everything Gets deleted Taxonomic groups and species

       
    }

    return lReturn;

}