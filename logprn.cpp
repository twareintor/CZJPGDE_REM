#include "logprn.h"


int CnnLogPrnW(LPCWSTR wszFNm, LPCWSTR wszWhat, double xWhat)
{
        int                     nOut = 0;
        LPWSTR          wszTmp = (LPWSTR)GlobalAlloc(GPTR, 0x400);

        swprintf(wszTmp, wszWhat, xWhat);
        // MessageBoxW(NULL, wszTmp, wszWhat, MB_OK);   // TEST
        CxxLogPrnW(wszFNm, wszTmp);
        GlobalFree(wszTmp);
        return nOut;
}

int CxxLogPrnW(LPCWSTR wszFNm, LPCWSTR wszWhat)
{
    // does nothing more than appending the string wszWhat at the end of the file wszFNm.....
    int            nOut = 0;
    HANDLE        hLogFile = CreateFileW(
                                        wszFNm,
                                        GENERIC_READ+GENERIC_WRITE,
                                        FILE_SHARE_DELETE+FILE_SHARE_READ+FILE_SHARE_WRITE,
                                        NULL,
                                        OPEN_ALWAYS,
                                        FILE_ATTRIBUTE_NORMAL,
                                        NULL
                                        );

    DWORD        dwBytesWritten = 0;

    if(hLogFile!=INVALID_HANDLE_VALUE)
    {
        SetFilePointer(hLogFile, 0, NULL, FILE_END);
        WriteFile(
                    hLogFile,
                    wszWhat,
                    wcslen(wszWhat)*2,
                    &dwBytesWritten,
                    NULL
                    );          // write what to log is...

    }
    else
    {
        nOut = -3209;
    }
    CloseHandle(hLogFile);
    return nOut;

}


int TEST_CastTypes()            // DEACTIVATED!!!! // DEACTIVATED!!!! // DEACTIVATED!!!! // DEACTIVATED!!!!
{
        /****************************************************************
        void*                   pInv = (void*)GlobalAlloc(GPTR, sizeof(DWORD));
        double*                 pxOut = (double*)GlobalAlloc(GPTR, sizeof(double));
        LPWSTR                  wszTmp;

        swprintf(wszTmp, L"Value = %d", *static_cast<double>(pInv));
        MessageBoxW(NULL, wszTmp, );

        GlobalFree(pxOut);
        GlobalFree(pInv);

        ****************************************************************/
        return 0;

}
