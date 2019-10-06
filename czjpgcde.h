//////////////////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <winbase.h>			// for file size, is not already included into the "windows.h"???
#include <sys/stat.h>
#include <commctrl.h>
#include <dirent.h>
#include <ctime>
#include <math.h>
#include <mbstring.h>
// #include <string.h>
#define	PI					3.1415926535
#define JPG_MAXFSIZE		0x1000000
// resource header // environment specific????? La henta esta muy loca!
#include "resource.h"

// third-party component headers:
#include "jpge.h"			// from Rich Geldreich <richgel99@gmail.com>
#include "jpgd.h" 			// from Rich Geldreich <richgel99@gmail.com>
#include "exif.h"			// from Mayank Lahiri <mlahiri@gmail.com>
// Aplication specific headers:
#include "cxbars.h"				// custom title bar, custom toolbars
#include "czjpg_if.h"			// custom interface - abbandoned!!!
#include "cxdisclaimer.h"		// disclaimer
#include "logprn.h"

#define CX_HTITLE			0			// height of the title bar		// VER.A48 NO MORE IN USE
#define CX_DIRECT_FROM_FILE	TRUE		// loads direct from file		// VER.A48 NO MORE IN USE

HBITMAP						g_hbm;					// saves the form image in order to be restored on repaint...
LPWSTR						g_szwFNX;				// the name of the current file shown in the form with OPEN command
BOOL						g_bIni = FALSE;			// becomes TRUE at the very start, allow drawing interface then switches off again.

BOOL 						g_bFullBlue = FALSE;	// practically a constant for fancy stuff
BOOL						g_bDblClkTO = FALSE;	// detects a double-click, as the main window does not receive double-clicks

int 						g_nGutter	= 24;		// practically a constant


int CXDoCreateMenu(HMENU hmenu);		// to create the menu, only when a custom title bar is not used...
void CatPath(char * BasePath, const char * FilePath);	//
int CXModifyJpgImage(HWND hwnd, double xNewWidth, double yNewHeight);	//
int CXFormatGeoLocOutput(EXIFInfo*	pexifRes, LPWSTR wszOuu, LPCWSTR wszFNm);	// formats the geolocation exif info into an understandable format;
int CXAddFilePrefix(LPCWSTR wszFNI, LPCWSTR wszPre, LPCWSTR wszSuf, LPWSTR wszFNO); // 
int compress_to_jpg(HDC hdc, LPRECT lprc, LPCWSTR szFNm);	//	
int decompress_to_dc(HDC hdc, LPRECT lprc, LPCWSTR szwFNm);
//int decompress_to_dc(HDC hdc, LPRECT lprc, const char* szFNm);	// replaced with the previous, having szFNm LPCSTR instead of const char
void test__HWND2JPG(HWND hwnd);											// 
void test__HWND2JPG(HWND hwnd, LPCWSTR wszFNm);							// 
void CXHwnd2Jpeg(HWND hwnd, LPCWSTR wszFNm);			// temporary solution // the function must take what in memory is...
void test__JPG2HWND(HWND hwnd);											// 
int CXTextWithShadow(
						HDC hdc, 
						LPRECT lprcBmp, 
						LPCWSTR szText, 
						UINT nMLine, 
						UINT nSize, 
						UINT nRow, 
						UINT nCol, 
						UINT nShadow, 
						UINT nMode
					); // prints text on output image: TODO: Documentation for this function
void SaveFormImageOnMem(HWND hwnd, BOOL bUpd);
DWORD ProvideTheFileName(HWND hwnd, LPWSTR szwFNm);		// sets the file name by opening the common control for open files (wide-char)
int CXGetExifInfoEx(LPCWSTR szwFNm, EXIFInfo* pexifRes);	// get exif information from the file

int CXFullBlue(BYTE* red, BYTE* green, BYTE* blue);		// not part of the target of the program, just a toy for fancy stuff!
int CXOpenFile(HWND hwnd, LPRECT lprcOut, LPWSTR szwFNm);		// TEST function to open a JPEG file in the dc designated by.......
int CXGetExifInfoText(EXIFInfo* pExifRes, LPWSTR szwTxt); // some exif data extracted from pExifRes and transposed into widechar-text
int CXOrientation(unsigned short iOrientation, LPWSTR szwOut, unsigned short *pAngle); // sets the szwOut to the description of the iOrientation
int CXNextFileInDirectory(LPWSTR szwFNm, int iStep);		// next file iStep in the directory - defined by szFNm
int CXGetJpgBytes(LPCWSTR szwFNm, DWORD* pdwFSize, unsigned char* szJpgBytes);	// receives the raw bytes from the jpg file





















//////////////////////////////////////////////////////////////////////////////////////////////
