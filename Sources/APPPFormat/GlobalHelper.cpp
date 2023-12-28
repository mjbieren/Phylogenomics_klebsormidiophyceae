#include "GlobalHelper.h"

long CheckReturnValue(long lReturn, std::vector<CTaxonomicGroup*> *pvecTaxonomicGroups)
{
    if (lReturn != ERR_NOERROR)
    {
        ClearVector(pvecTaxonomicGroups); //Everything Gets deleted Taxonomic groups and species      
    }

    return lReturn;

}