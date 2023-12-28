#include "GlobalHelper.h"

long CheckReturnValue(long lReturn, std::vector<CFastaFile*>* pvecFastaFilesOutputFiles, CFastaFile* pFastaFileQuery, CTable* pBlastFile, CTable * pBlastFileFiltered)
{
    if (lReturn != ERR_NOERROR)
    {
        ClearVector(pvecFastaFilesOutputFiles);
        delete pFastaFileQuery;
        delete pBlastFile;
        delete pBlastFileFiltered;
        CHECK_RETURNVALUE(lReturn);
    }

    return lReturn;

}