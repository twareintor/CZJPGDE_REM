#ifndef LOGPRN_H_892382838223892384839
#define LOGPRN_H_892382838223892384839



#include <windows.h>
#include <stdio.h>

#define LOGPRN0 L"outlog000.htm"
#define LOGPRN1 L"outlog001.htm"
#define LOGPRN2 L"outlog002.htm"

#define HEADER1 L"<html><head><title></title><body>"
#define FOOTER1 L"\n</body></html>"


int CnnLogPrnW(LPCWSTR wszFNm, LPCWSTR wszWhat, double xWhat);
int CxxLogPrnW(LPCWSTR wszFNm, LPCWSTR wszWhat);

int TEST_CastTypes();           // DEACTIVATED!!!! // DEACTIVATED!!!! // DEACTIVATED!!!! // DEACTIVATED!!!!

int CxxLogPrnW(LPCWSTR wszFNm, LPCWSTR wszWhat);
int CnnLogPrnW(LPCWSTR wszFNm, LPCWSTR wszWhat, void* pWhat);


























#endif//LOGPRN_H_892382838223892384839
