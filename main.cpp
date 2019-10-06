/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Progetto Originale: "CZJPGDE.DEV" // 
// nome del file: "main.cpp"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "czjpgcde.h"

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_CREATE:
		{
			// General_Disclaimer();
			g_szwFNX = (LPWSTR)GlobalAlloc(GPTR, MAX_PATH*4);	// make room for the current file name loaded into the form
			// CXLogPrint("d:\\tmp\\logprn.txt", 10, "Program start...", TRUE);	// WARNING!!! THIS FUNCTION IS DEFECT!!!
			CxxLogPrnW(LOGPRN0, L"<p>Application started here:\r\n");
			break;
		}
		case WM_LBUTTONDOWN:
		{
			if(CX_HTITLE>=1)
			{
				POINT		Mouse = {LOWORD(lParam), HIWORD(lParam)};
				LPRECT		lprcAreas;
				LPRECT		lprcOpen;
				
				lprcOpen = (LPRECT)GlobalAlloc(GPTR, sizeof(RECT));
				lprcAreas = (LPRECT)GlobalAlloc(GPTR, sizeof(RECT)*5);
				
				CZTitleBar		cztb(hwnd, 18, lprcAreas, TRUE);
				for(int i=0; i<=4; i++)
				{
					if(PtInRect(lprcAreas+i, Mouse))
					{
						switch(i){
							case 4:
							{
								int nMsgRes = 
									MessageBoxW(hwnd, L"Are you sure you want to close?", L"Application close", MB_YESNO|MB_ICONQUESTION); 
								if(nMsgRes==IDYES)
								{
									PostQuitMessage(0);
								}
								break;
							}
						}
					}
				}
				GlobalFree(lprcAreas);
			}
			break;
		}
		case WM_PAINT:
		{
			if(CX_HTITLE>=1)
			{
				CZTitleBar	cztitlebar(hwnd, 18, NULL, TRUE);	// this draws the custom title bar
				// place to draw toolbar(s)
			}
			SaveFormImageOnMem(hwnd, TRUE);
			break;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam)){
				case ID_FILE_OPEN:
				{
					LPWSTR			szwFNm = (LPWSTR)GlobalAlloc(GPTR, 0x200);
					if(ProvideTheFileName(hwnd, szwFNm)==3604)
					{
						CXOpenFile(hwnd, NULL, szwFNm);
						g_szwFNX = (LPWSTR)GlobalAlloc(GPTR, 0x400);
						wcscpy(g_szwFNX, szwFNm);
						SaveFormImageOnMem(hwnd, FALSE);
					}
					GlobalFree(szwFNm);
					break;
				}
				case ID_FILE_EXIT:
				{
					int nMsgRes = 
						MessageBoxW(hwnd, L"Are you sure you want to exit?", L"Application close", MB_YESNO|MB_ICONQUESTION); 
					if(nMsgRes==IDYES)
					{
						PostQuitMessage(0);
					}
					break;
				}
				case ID_FILE_SAVE:
				{
					MessageBoxW(hwnd, L"File not saved", L"Function in development", MB_OK|MB_ICONINFORMATION);
					break;
				}
				case ID_FILE_SAVEAS:
				{
					LPWSTR			szwFNm = (LPWSTR)GlobalAlloc(GPTR, 0x200);
					swprintf(szwFNm, L"info_%s", wcsrchr(g_szwFNX, L'\\')+1);		// this is the "filename" extracted from the whole "path\\filename"
					if(ProvideTheFileName(hwnd, szwFNm)==3603)
					{
						test__HWND2JPG(hwnd, szwFNm);
					}
					GlobalFree(szwFNm);
					break;
				}
				case ID_TRANSFORM_FILES:
				{
					int ncxmodRes = CXModifyJpgImage(hwnd, 800, 0);
					SaveFormImageOnMem(hwnd, FALSE);
					MessageBox(hwnd, "Done!", "File(s) transform result", MB_OK);
					break;
				}
			}
			break;
		}
		case WM_KEYDOWN:
		{
			switch(wParam)
			{
				case VK_LEFT:
				{
					CXNextFileInDirectory(g_szwFNX, -1);
					CXOpenFile(hwnd, NULL, g_szwFNX);
					SaveFormImageOnMem(hwnd, FALSE);
					break;
				}
				case VK_RIGHT:
				{
					CXNextFileInDirectory(g_szwFNX,  1);
					CXOpenFile(hwnd, NULL, g_szwFNX);
					SaveFormImageOnMem(hwnd, FALSE);
					break;
				}
			}
			break;
		}
		case WM_KILLFOCUS:
		{
			if(CX_HTITLE>=1)
				CZTitleBar		cztb(hwnd, 18, NULL, FALSE);
			break;
		}
		case WM_SETFOCUS:
		{
			if(CX_HTITLE>=1)
				CZTitleBar		cztb(hwnd, 18, NULL, TRUE);
			break;
		}
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: 
		{
			GlobalFree(g_szwFNX);
			CxxLogPrnW(LOGPRN0, L"<p>Application stops here.\r\n");
			PostQuitMessage(0);
			break;
		}
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG Msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON)); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICONSM)); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	HMENU hmenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MYMENU));
	if(CX_HTITLE>=1) hmenu = NULL;
	DWORD	dwStyle1 = (WS_EX_STATICEDGE*(CX_HTITLE>=1))|(WS_EX_CLIENTEDGE*(CX_HTITLE<=0));
	DWORD	dwStyle2 = (WS_POPUPWINDOW*(CX_HTITLE>=1))|(WS_OVERLAPPEDWINDOW*(CX_HTITLE<=0));
	hwnd = CreateWindowEx(dwStyle1,"WindowClass","CX Image Info - Version Alpha 000 - - - TEST VERSION", dwStyle2|WS_VISIBLE,
		0, /* x */
		0, /* y */
		GetSystemMetrics(SM_CXSCREEN)*3/4, /* width */
		GetSystemMetrics(SM_CXSCREEN)*3/4*9/16, /* height */
		NULL,
		hmenu,
		hInstance,
		NULL);
	// ShowWindow(hwnd, SW_MAXIMIZE);	
	// SetWindowLong(hwnd, NULL, WS_SIZEBOX);
	
	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&Msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&Msg); /* Translate key codes to chars if present */
		DispatchMessage(&Msg); /* Send it to WndProc */
	}
	return Msg.wParam;
}

int CXDoCreateMenu(HMENU hmenu)
{
	
	return 0;
}

int compress_to_jpg(HDC hdc, LPRECT lprc, LPCWSTR wszFNm)
{
	// device-context variables...
	HDC					hdcMem;
	HBITMAP				hbm = NULL;
	BITMAP				bm;
	HBITMAP				hbmOld = NULL;
	// variables for image storage and processing
	HGLOBAL				hImgBytes;
	unsigned char*		szImgBytes = new unsigned char;
	unsigned char		szTmp;
	int					iAlign = (lprc->right-lprc->left)%4;
	// jpge-namespace-specific params variables
	jpge::params		paraDef;
	
	hdcMem = CreateCompatibleDC(hdc);
	// TODO: Restrict the width of the exported image to be a multiple of 4!!!
	hbm = CreateCompatibleBitmap(hdc, (lprc->right-lprc->left-iAlign), lprc->bottom-lprc->top);
	hbmOld = (HBITMAP)SelectObject(hdcMem, hbm);
	SetStretchBltMode(hdc, HALFTONE);
	
	BitBlt(hdcMem, 0, 0, lprc->right-lprc->left, lprc->bottom-lprc->top, hdc, 0, 0, SRCCOPY);
	GetObject(hbm, sizeof(BITMAP), &bm);
	// Bitmap info:	
	BITMAPINFOHEADER   bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);    
    bi.biWidth = bm.bmWidth;    
    bi.biHeight = -bm.bmHeight;  // top-down bitmap, in order to be saved as top-down JPG
    bi.biPlanes = 1;    
    bi.biBitCount = 24;    
    bi.biCompression = BI_RGB;    
    bi.biSizeImage = 0;  
    bi.biXPelsPerMeter = 0;    
    bi.biYPelsPerMeter = 0;    
    bi.biClrUsed = 0;    
    bi.biClrImportant = 0;
    // BUG HERE // OUTPUT IMAGE MISALIGNMENT!!! OUTPUT IMAGE MISALIGNMENT!!! OUTPUT IMAGE MISALIGNMENT!!! OUTPUT IMAGE MISALIGNMENT!!!
	
	DWORD dwBmpSize = ((bm.bmWidth * bi.biBitCount + 31) / 32) * 4 * bm.bmHeight; // misalignment!!! // TO BE CORRECTED!!!!!
	// DWORD dwBmpSize = bm.bmWidth * bm.bmHeight*3; // another misalignment!!!
	hImgBytes = GlobalAlloc(GHND, dwBmpSize);
	szImgBytes = (unsigned char*)GlobalLock(hImgBytes);
	// get the image bits in order to be saved in JPEG
 	GetDIBits(hdc, hbm, 0, (UINT)bm.bmHeight, szImgBytes, (LPBITMAPINFO)&bi, DIB_RGB_COLORS);
	// now invert red with blue, as our function inverts them self during compression...
	// MessageBoxW(NULL, L"here", L"DEBUG_3209", MB_OK);
	/// HERE IS THE NEXT PROBLEM: SEGMENTATION FAULT SEGFAULT SEGFAULT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	for(DWORD i=0; i<dwBmpSize-3-1; i+=3) // this is for 3 bits-per-point; last point will be probably not changed!!!
	{
		szTmp = szImgBytes[i];
		szImgBytes[i] = szImgBytes[i+2];
		szImgBytes[i+2] = szTmp;
		
	}
	
	// now save the array of bytes from bitmap in jpeg:		// variables to be used by saving the JPEG file:
	HGLOBAL		hJpegBytes = GlobalAlloc(GHND, JPG_MAXFSIZE);	// 16 MBytes!!! actually more than enough!!! 
	LPBYTE		lpbytJpg = (LPBYTE)GlobalLock(hJpegBytes);
	int			nJpgSize = JPG_MAXFSIZE;						// required to be non-zero (by the "jpge" namespace functions) - take a look out there...
	
	jpge::compress_image_to_jpeg_file_in_memory(lpbytJpg, nJpgSize, bm.bmWidth, bm.bmHeight, 3, (const jpge::uint8*)szImgBytes, paraDef);
	// Code to save the JPEG Bytes in a new file with Unicode name... done!!!
	
	HANDLE		hFile = CreateFileW(wszFNm, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD		dwWritten = 0;
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		WriteFile(hFile, lpbytJpg, nJpgSize, &dwWritten, NULL);
	}
	// Release BITMAP allocated memory 
	GlobalUnlock(hImgBytes); 	GlobalFree(hImgBytes);	CloseHandle(hImgBytes);
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);
	// release JPEG allocated memory and handles
	GlobalUnlock(hJpegBytes);	GlobalFree(hJpegBytes);	CloseHandle(hJpegBytes);
	return 0;
}

//
int decompress_to_dc(HDC hdc, LPRECT lprc, LPCWSTR szwFNm)
{
	// variables to do with receiving the bytes from the image
	int*				pWidth;			// width get from the extracting function
	int*				pHeight;		// height get from the extracting function
	int* 				pActComps;		// actual compression returned from the extracting function
	int 				nReqComps;		// required compression sent to the extracting function
	DWORD				dwFSize;		// JPEG file size, in bytes
	HGLOBAL				hJpgBytes;		// receives the raw-jpg bytes from the file "szFNm" (or "szwFNm" after transforming) 
	HGLOBAL				hImgBytes;		// receives the image bytes
	unsigned char*		szJpgBytes; 	// the raw-JPG image stored in memory
	unsigned char*		szImgBytes;		// receives the image bytes
	char				szTmp;			// for By2By operations probably needed 
	DWORD				dwBmpSize;		// the size of the bitmap
	double				xResFact = 1;	// resize factor
	// variables to do with logging and debugging
	char*				szMsg;			//
	szMsg = (char*)GlobalAlloc(GPTR, 0xff);		// this will have to do with debugging...
	//****************************************
	// loading the JPG file from media (hFile from szFNm) to memory in memory (szJpgBytes)
	CXGetJpgBytes(szwFNm, &dwFSize, NULL);	// just get the file size first...
	hJpgBytes = GlobalAlloc(GPTR, dwFSize);			// in order to know how big is the szJpgBytes...
	szJpgBytes = (unsigned char*)GlobalLock(hJpgBytes);
	CXGetJpgBytes(szwFNm, &dwFSize, szJpgBytes);	// then read the file and get the raw-jpeg bytes
	GlobalUnlock(hJpgBytes);
	CloseHandle(hImgBytes);
	//****************************************/
	// now initializing the variables receiving the dimensions of the decompressed picture...
	pWidth = (int*)GlobalAlloc(GPTR, sizeof(int));
	pHeight = (int*)GlobalAlloc(GPTR, sizeof(int));
	pActComps = (int*)GlobalAlloc(GPTR, sizeof(int));
	 
	nReqComps = 3; 			// 1 = Grayscale; 3 = RGB, 4 = RGBA
	szImgBytes = (unsigned char*)jpgd::decompress_jpeg_image_from_memory(szJpgBytes, dwFSize, pWidth, pHeight, pActComps, nReqComps);	
	// if the output rectangle is a null rectangle, then set it new
	if(IsRectEmpty(lprc))	// in this case, set the "lprc" just equal with the image dimensions, with the upper-left corner at {0,0}
	{	// ... simultaneously return the image dimensions with it, when requested (by sending the function a pointer to an empty RECT)
		if((lprc->right-lprc->left==0)&&(lprc->bottom-lprc->top==0))	// if no width or height defined, display original size:
		{
			lprc->right = *pWidth+lprc->left;
			lprc->bottom = *pHeight+lprc->top;
		}
		else
		{
			lprc->bottom = lprc->top+(lprc->right-lprc->left)*(*pHeight)/(*pWidth);
			lprc->right = lprc->left+(lprc->bottom-lprc->top)*(*pWidth)/(*pHeight);
		}
	}			// something will be dispayed anyway !!!	
	// correct the colors: invert red with blue
	for(DWORD i=0; i<=(*pWidth)*(*pHeight)*3-3; i+=3)
	{
		unsigned char szTmp = szImgBytes[i];
		szImgBytes[i] = szImgBytes[i+2];
		szImgBytes[i+2] = szTmp;
	}		
	// set the dimension of the output rectangle, then invert red with blue and pad the lines to DWORD (HERE THE OUTPUT IMAGE FAILS!!!)
	dwBmpSize = (*pWidth)*(*pHeight)*3;	
	// now set the bitmap info header and bitmap info: (from "Capturing an image" from MSDN)
	HBITMAP				hbm;
	HBITMAP 			hbmOld;
	BITMAPINFO			bmi;
	// Bitmap properties:	
	ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);    
    bmi.bmiHeader.biWidth = *pWidth;    
    bmi.bmiHeader.biHeight = -*pHeight;  
    bmi.bmiHeader.biPlanes = 1;    
    bmi.bmiHeader.biBitCount = 8*(*pActComps);    
    bmi.bmiHeader.biCompression = BI_RGB;    
    bmi.bmiHeader.biSizeImage = 0;  
    bmi.bmiHeader.biXPelsPerMeter = 0;    
    bmi.bmiHeader.biYPelsPerMeter = 0;    
    bmi.bmiHeader.biClrUsed = 0;    
    bmi.bmiHeader.biClrImportant = 0;
	// load image into the hdc:
	HDC					hdcMem;
	hdcMem = CreateCompatibleDC(hdc);
	hbm = CreateCompatibleBitmap(hdc, *pWidth, *pHeight);
	hbmOld = (HBITMAP)SelectObject(hdcMem, hbm);
	// SetStretchBltMode(hdc, HALFTONE);
	SetDIBits(hdcMem, hbm, 0, *pHeight, szImgBytes, (LPBITMAPINFO)&bmi, DIB_RGB_COLORS);
	free(szImgBytes);
	// time to release the resources 
	SetStretchBltMode(hdc, HALFTONE);
	StretchBlt(hdc, lprc->left, lprc->top, lprc->right-lprc->left, lprc->bottom-lprc->top, hdcMem, 0, 0, *pWidth, *pHeight, SRCCOPY);
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);
	// release objects and memory, handles aso.
	GlobalFree(szMsg);
	GlobalFree(pWidth);
	GlobalFree(pHeight);
	GlobalFree(pActComps);
	return 0;
}

int CXGetJpgBytes(LPCWSTR szwFNm, DWORD* pdwFSize, unsigned char* szJpgBytes)
{	// gets the JPEG raw file, by flooding the szJpgBytes with the raw data existing into the szwFNm
	HANDLE				hFile;			// handle to the JPEG file to be opened...
	DWORD				dwBytesRead;	// file size

	// getting the file size first...
	// then reading the file and get the bytes into the szJpgBytes...
	hFile = CreateFileW(szwFNm, GENERIC_READ,0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		MessageBoxW(NULL, L"File does not exists", szwFNm, MB_OK);
		CloseHandle(hFile);
		return -11;
	}
	if(szJpgBytes==NULL)	// just read the file size, and fill the *pdwFSize with its size
	{
		*pdwFSize = GetFileSize(hFile, NULL);						// this will return the JPEG file size	
	}
	else	// read file itself, and fill the szJpgBytes array with the content of the file...
	{
		ReadFile(hFile, szJpgBytes, *pdwFSize, &dwBytesRead, NULL);
	}
	CloseHandle(hFile);
	return 0;
}

int CXOpenFile(HWND hwnd, LPRECT lprcOut, LPWSTR szwFNm)
{
	int*			pWidth = (int*)GlobalAlloc(GPTR, sizeof(int));
	int*			pHeight = (int*)GlobalAlloc(GPTR, sizeof(int));
	LPRECT			lprc = (LPRECT)GlobalAlloc(GPTR, sizeof(RECT));
	LPRECT			lprcTop = (LPRECT)GlobalAlloc(GPTR, sizeof(RECT));
	LPRECT			lprcIn = (LPRECT)GlobalAlloc(GPTR, sizeof(RECT));
	LPRECT			lprcImg = (LPRECT)GlobalAlloc(GPTR, sizeof(RECT));
	LPRECT			lprcTxt = (LPRECT)GlobalAlloc(GPTR, sizeof(RECT));
	BOOL			bExifData = TRUE;
	
	// Preparing operations: reading the file EXIF header info, for next steps:
	HGLOBAL		hexifRes = GlobalAlloc(GHND, sizeof(EXIFInfo));
	EXIFInfo*	pexifRes = (EXIFInfo*)GlobalLock(hexifRes);
	
	// Set the output rectanlgle
	GetClientRect(hwnd, lprc);
	// SUBJECT TO REVISION // 
	int nLat = lprc->bottom-lprc->top-g_nGutter;
	SetRect(lprcTop, lprc->left, lprc->top, lprc->right, lprc->top+g_nGutter);
	SetRect(lprcIn, lprc->left, g_nGutter, lprc->left+nLat, lprc->top+g_nGutter+nLat);
	SetRect(lprcTxt, lprcIn->right, lprcIn->top, lprc->right, lprc->bottom);
	
	if((CXGetExifInfoEx(szwFNm, pexifRes)==0)&&(TRUE))	// if we can parse the EXIF data, we will get it in a structure
	{
		*pWidth = pexifRes->ImageWidth;
		*pHeight = pexifRes->ImageHeight;
	}
	else	// else, no EXIF header, probably JFIF, but however we will get the dimensions with GetImageSize()
	{
		// GetImageSize(szFNm, pWidth, pHeight);		// REMOVED TILL A FUNCTION THAT SUPPORT WIDE CHARACTER FILE NAMES...
		bExifData = FALSE;	
	}
	// set now the text on the title / image topmost frame: file name, dimensions, orientation
	LPWSTR			szwTop = (LPWSTR)GlobalAlloc(GPTR, MAX_PATH*4);
	LPWSTR			szwTxt = (LPWSTR)GlobalAlloc(GPTR, MAX_PATH*8);
	LPWSTR			szwOri = (LPWSTR)GlobalAlloc(GPTR, MAX_PATH*1);
	
	if(bExifData)
	{
		unsigned short*	pAngle = (unsigned short*)GlobalAlloc(GPTR, sizeof(unsigned short));
		
		CXOrientation(pexifRes->Orientation, szwOri, pAngle);
		// DEBUG MARK##30214
		swprintf(szwTop, L"File: \"%s\", Dims: %dx%d, Orientation: %s", szwFNm, *pWidth, *pHeight, szwOri);
		GlobalFree(pAngle);
	}
	GlobalFree(szwOri);
	// info text from EXIF data, otherwise, print a message that there no Exif data is available
	if(bExifData)	// 
	{
		CXGetExifInfoText(pexifRes, szwTxt);	// Exif info in a format which can be printed
	}
	else
	{
		swprintf(szwTxt, L"\r\n - No EXIF data available - \r\n\r\nCopyright (c) 2011-2014 C.C.");
	}
	if(bExifData)
	{	
		if((*pWidth)/(*pHeight)>=(lprcIn->right-lprcIn->left)/(lprcIn->bottom-lprcIn->top))
		{
			SetRect(lprcImg, lprcIn->left, lprcIn->top, lprcIn->right, lprcIn->top+(lprcIn->right-lprcIn->left)*(*pHeight)/(*pWidth));
			OffsetRect(lprcImg, 0, ((lprcIn->bottom-lprcIn->top)-(lprcImg->bottom-lprcImg->top))/2);
		}
		else
		{
			SetRect(lprcImg, lprcIn->left, lprcIn->top, lprcIn->left+(lprcIn->bottom-lprcIn->top)*(*pWidth)/(*pHeight), lprcIn->bottom);
			OffsetRect(lprcImg, ((lprcIn->right-lprcIn->left)-(lprcImg->right-lprcImg->left))/2, 0);
		}
	}
	else
	{	// the dimensions will be retrieved from the decompressed image (after decomp) in source function "decompress_to_dc()"
		{	// just an empty rect, in order to let the "decompress_to_dc()" to choose the output rectangle...
			SetRect(lprcImg, lprcIn->left, lprcIn->top, lprcIn->right, lprcIn->top);
		}
	}
	// then do the graphic operations:		Declarations
	PAINTSTRUCT		ps;
	HDC				hdc;
	HBRUSH			hbr, hbrOld;
	// then do the graphic operations:		Program
	BeginPaint(hwnd, &ps);
	hdc = GetDC(hwnd);
	SetBkMode(hdc, TRANSPARENT);									// for the text not to have shadow
	SetTextColor(hdc, RGB(0xd0, 0xd0, 0x40));						// gold
	SelectObject(hdc, CreateSolidBrush(RGB(0x40, 0x20, 0x40)));		// dark magenta, for information
	Rectangle(hdc, lprcTop->left-lprc->left, lprcTop->top-lprc->top, lprcTop->right-lprc->left, lprcTop->bottom-lprc->top);
	DrawTextW(hdc, szwTop, -1, lprcTop, DT_LEFT);					// file name, dimensions, orientation in the "toolbar"
	Rectangle(hdc, lprcTxt->left-lprc->left, lprcTxt->top-lprc->top, lprcTxt->right-lprc->left, lprcTxt->bottom-lprc->top);
	DrawTextW(hdc, szwTxt, -1, lprcTxt, DT_LEFT);
	SelectObject(hdc, CreateSolidBrush(RGB(0x40, 0x20, 0x40)));		// no more full black, but also dark magenta for image padding
	Rectangle(hdc, lprcIn->left-lprc->left, lprcIn->top-lprc->top, lprcIn->right-lprc->left, lprcIn->bottom-lprc->top);
	// Function to extract the image from the file "szFNm" and to put it into the form DC:
	decompress_to_dc(hdc, lprcImg, szwFNm);		// argument 3 of the "decompress_to_dc()" replaced with a widechar argument // it was!
	// GlobalFree(szwFNm);
	// 
	ReleaseDC(hwnd, hdc);
	EndPaint(hwnd, &ps);
	
	GlobalFree(szwTop);
	GlobalFree(szwTxt);
	
	GlobalUnlock(hexifRes);
	CloseHandle(hexifRes);
	
	GlobalFree(lprc);
	GlobalFree(lprcTxt);
	GlobalFree(lprcImg);	
	GlobalFree(lprcIn);
	GlobalFree(lprcOut);
	GlobalFree(lprcTop);
	GlobalFree(pWidth);
	GlobalFree(pHeight);
	
	return 0;	
}


int CXOrientation(unsigned short iOrientation, LPWSTR szwOut, unsigned short* pAngle)
{
	// TODO: Define angle and write it in the "pAngle" address...
	if(iOrientation==0){	wcscpy(szwOut, L"Unspecified");	*pAngle = 0;	}		
	if(iOrientation==1){	wcscpy(szwOut, L"Upper Left");	*pAngle = 0;	}
	if(iOrientation==3){	wcscpy(szwOut, L"Lower Right");	*pAngle = 0;	}
	if(iOrientation==6){	wcscpy(szwOut, L"Upper Right");	*pAngle = 0;	}
	if(iOrientation==8){	wcscpy(szwOut, L"Lower Left");	*pAngle = 0;	}
	if(iOrientation==9){	wcscpy(szwOut, L"Undefined");	*pAngle = 0;	}
	
	return 0;
}

int CXNextFileInDirectory(LPWSTR szwFNm, int iStep)
{
	// function disabled... to be modified entirely...
	/************************************************************************************************************************
	LPTSTR		szDir = (LPTSTR)GlobalAlloc(GPTR, MAX_PATH*2);		// the directory-path of the szFNm current file name
	LPTSTR		szTmp = (LPTSTR)GlobalAlloc(GPTR, MAX_PATH*2);		// temporary stored file name
	LPTSTR		szFnT = (LPTSTR)GlobalAlloc(GPTR, MAX_PATH*2);		// stores the szFNm (that are modified already at the begin of the function / mistake!!)
	HANDLE		hFnY = GlobalAlloc(GHND, MAX_PATH*2*2048);
	LPTSTR		szFnY = (LPTSTR)GlobalLock(hFnY);	// all files in the directory get from Dir() function
	int 		nFound = 0;
	
	strcpy(szFnT, szFNm);
	
	int i = strlen(szFNm);					// WARNING!!!! THIS WILL NOT WORK ON UNICODE OR MBCS FILE NAMES!!!
	while((i>0)&&(szFNm[i---2]!='\\'));		// WARNING!!!! THIS WILL NOT WORK ON UNICODE OR MBCS FILE NAMES!!!
	szFNm[i] = 0;
	memcpy(szDir, szFNm, i); 
	memcpy(szTmp, szDir, strlen(szDir));			// set it too
	// strcat(szDir, "*.jpg"); 				// WARNING!!!! WARNING!!! WARNING!!!
	// MessageBox(NULL, szFNm, NULL, MB_OK);
	
	DIR			*dir;				// no need for description
	dirent		*pdir;				// 		anyway
	int			u = 0;				// counts files during dir call 
	int			nFiles = 0;			// stores the total number of files in the working directory
	
	dir = opendir(szDir);
	int	j = 0;
	while(pdir=readdir(dir))
	{
		if(memcmp(pdir->d_name, ".", 1)!=0)	
		{
			strcat(szTmp, pdir->d_name);	// make path, but exclude "." and ".."
			// MessageBox(NULL, szTmp, NULL, MB_OK);
			memcpy(szFnY+MAX_PATH*2*j, szTmp, strlen(szTmp));		
			// MessageBox(NULL, szFnT, szTmp, MB_OK);
			if(strcmp(szTmp, szFnT)==0)
			{
				nFound = j;
			}
			strcpy(szTmp, szDir);
			j++;
		}	
	}
	j--;
	closedir(dir);
	free(pdir);
	// prepare the szFNm as modified value to be returned to the program, as next or previous file, depending on iStep
	int nCrt = nFound+iStep;
	if(nCrt>j)	nCrt=nCrt-j;
	if(nCrt<0)	nCrt=j+nCrt;
	memcpy(szFNm, szFnY+MAX_PATH*2*nCrt, MAX_PATH*2+1);
	// MessageBox(NULL, szFnY+MAX_PATH*2*nCrt, NULL, MB_OK);
	// free resources
	GlobalFree(szFnT);
	GlobalFree(hFnY);
	CloseHandle(hFnY);
	GlobalFree(szTmp);
	GlobalFree(szDir);
	// then return to the system
	return 0;
	************************************************************************************************************************/
	return -1;
}

int CXGetExifInfoText(EXIFInfo* pexifRes, LPWSTR szwTxt)
{
	wchar_t		wszTmp[512];
	
	LPWSTR			szwOri = (LPWSTR)GlobalAlloc(GPTR, MAX_PATH*1);
	unsigned short*	pAngle = (unsigned short*)GlobalAlloc(GPTR, sizeof(unsigned short));
	
	CXOrientation(pexifRes->Orientation, szwOri, pAngle);
	// Printing the data:
	wcscat(szwTxt, L"\r\n EXIF Data Information:\r\n");
	wcscat(szwTxt, L"\r\n Description: ");
	mbstowcs(wszTmp, pexifRes->ImageDescription.c_str(), strlen(pexifRes->ImageDescription.c_str())+1);
	wcscat(szwTxt, wszTmp);
	wcscat(szwTxt, L"\r\n Copyright: ");
	mbstowcs(wszTmp, pexifRes->Copyright.c_str(), strlen(pexifRes->Copyright.c_str())+1);
	wcscat(szwTxt, wszTmp);
	wcscat(szwTxt, L"\r\n Camera: ");
	mbstowcs(wszTmp, pexifRes->Model.c_str(), strlen(pexifRes->Model.c_str())+1);
	wcscat(szwTxt, wszTmp);
	wcscat(szwTxt, L", Manufacturer: ");
	mbstowcs(wszTmp, pexifRes->Make.c_str(), strlen(pexifRes->Make.c_str())+1);
	wcscat(szwTxt, wszTmp);
	wcscat(szwTxt, L"\r\n Software used: ");
	mbstowcs(wszTmp, pexifRes->Software.c_str(), strlen(pexifRes->Software.c_str())+1);
	wcscat(szwTxt, wszTmp);
	wcscat(szwTxt, L"\r\n Date & Time Picture Taken: ");
	mbstowcs(wszTmp, pexifRes->DateTimeOriginal.c_str(), strlen(pexifRes->DateTimeOriginal.c_str())+1);
	wcscat(szwTxt, wszTmp);
	wcscat(szwTxt, L"\r\n Date & Time Picture Digitized: ");
	mbstowcs(wszTmp, pexifRes->DateTimeDigitized.c_str(), strlen(pexifRes->DateTimeDigitized.c_str())+1);
	wcscat(szwTxt, wszTmp);
	wcscat(szwTxt, L"\r\n Date & Time Sub-Second Original: ");
	mbstowcs(wszTmp, pexifRes->SubSecTimeOriginal.c_str(), strlen(pexifRes->SubSecTimeOriginal.c_str())+1);
	wcscat(szwTxt, wszTmp);
	wcscat(szwTxt, L"\r\n Dimensions: ");
	swprintf(wszTmp, L"%d x %d (as per ExIf data)", pexifRes->ImageHeight, pexifRes->ImageWidth);	
	wcscat(szwTxt, wszTmp);
	wcscat(szwTxt, L"\r\n Bits per sample: ");
	swprintf(wszTmp, L"%hd; Byte Align = %d", pexifRes->BitsPerSample, pexifRes->ByteAlign);
	wcscat(szwTxt, wszTmp);
	swprintf(wszTmp, L"\r\n Exposure Bias Value (EV): %g", pexifRes->ExposureBiasValue);
	wcscat(szwTxt, wszTmp);
	swprintf(wszTmp, L"\r\n Exposure Time: %g s", pexifRes->ExposureTime);
	wcscat(szwTxt, wszTmp);
	if(pexifRes->Flash==0){wcscat(szwTxt, L"\r\n Flash used: Yes");}else{wcscat(szwTxt, L"\r\n Flash used: No");}
	wcscat(szwTxt, L"\r\n F/Stop: ");
	swprintf(wszTmp, L"%g", pexifRes->FNumber);
	wcscat(szwTxt, wszTmp);
	wcscat(szwTxt, L"\r\n Focal Lenght: ");
	swprintf(wszTmp, L"%g mm", pexifRes->FocalLength);
	wcscat(szwTxt, wszTmp);
	wcscat(szwTxt, L"\r\n Focal Lenght in 35 mm: ");
	swprintf(wszTmp, L"%hd mm", pexifRes->FocalLengthIn35mm);
	wcscat(szwTxt, wszTmp);
	wcscat(szwTxt, L"\r\n ISO Speed Ratings: ");
	swprintf(wszTmp, L"%hi", pexifRes->ISOSpeedRatings);
	wcscat(szwTxt, wszTmp);
	swprintf(wszTmp, L"\r\n Orientation: %s", szwOri);
	wcscat(szwTxt, wszTmp);
	wcscat(szwTxt, L"\r\n Metering mode: ");
	swprintf(wszTmp, L"%hi", pexifRes->MeteringMode);
	wcscat(szwTxt, wszTmp);
	wcscat(szwTxt, L"\r\n (1 = Average, 2 = Center-spot average, 3 = Spot");
	wcscat(szwTxt, L"\r\n         4 = Multi-spot, 5 = Multi-segment)");
	wcscat(szwTxt, L"\r\n Shutter speed (reciprocal of exposure time): ");
	swprintf(wszTmp, L"%d", pexifRes->ShutterSpeedValue);
	wcscat(szwTxt, wszTmp);
	wcscat(szwTxt, L"\r\n Subject distance: ");
	swprintf(wszTmp, L"%g m", pexifRes->SubjectDistance);
	wcscat(szwTxt, wszTmp);
	swprintf(wszTmp, L"\r\n\r\n");
	wcscat(szwTxt, wszTmp);
	CXFormatGeoLocOutput(pexifRes, wszTmp, NULL);
	wcscat(szwTxt, wszTmp);
	/*
	*/
	GlobalFree(pAngle);
	GlobalFree(szwOri);

	return wcslen(szwTxt);
}

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
					)
{	// make it on another DC then SRCCOPY
	const double 	xGut = 0.05;		// gutter (left, right, top, bottom= the same, as well as 
										//  multiplication factor for the 
	// NOTE: nMode> 0: Writes direct into the "hdc" as argument, when FALSE, first in an a new HDC then combines the two
	
	LPRECT			lprcText;
	HFONT 			hf;
	HFONT			hfOld;
	long 			lfHeight;
	UINT			nTxtPara;

	HDC				hdcX;
	SIZE			sizBmp;
	
	sizBmp.cx = lprcBmp->right-lprcBmp->left;
	sizBmp.cy = lprcBmp->bottom-lprcBmp->top;
	nTxtPara = DT_SINGLELINE*(nMLine==0)|DT_WORDBREAK*(nMLine>0)|
				((nRow==1)*(DT_TOP))|((nRow==2)*(DT_VCENTER))|((nRow==3)*(DT_BOTTOM))|
				((nCol==1)*(DT_LEFT))|((nCol==2)*(DT_CENTER))|((nCol==3)*(DT_RIGHT));
	// a new full HDC is created, where the text will be initially printed, then copied into the destination DC:
	HDC				hdcMemTxt;
	HBITMAP			hbmMemTxt;
	HBITMAP			hbmMemOld;
	if(nMode)
	{
		hdcMemTxt = CreateCompatibleDC(hdc);
		hbmMemTxt = CreateCompatibleBitmap(hdc, sizBmp.cx, sizBmp.cy);
		hbmMemOld = (HBITMAP)SelectObject(hdcMemTxt, hbmMemTxt);
		hdcX = hdcMemTxt;
	}
	else
	{
		hdcX = hdc;
	}
	// getting the font parameters:
	// lfHeight = -MulDiv(24, GetDeviceCaps(hdc, LOGPIXELSY), 36);	// no really needed to be aligned with the pixels...
	lfHeight = nSize*xGut*(sizBmp.cy>(16/9)*sizBmp.cx?sizBmp.cy:sizBmp.cx)/10;
	hf = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Verdana");
	lprcText = (LPRECT)GlobalAlloc(GPTR, sizeof(RECT));
	hfOld = (HFONT)SelectObject(hdcX, hf);
	// drawing the text:
	SetBkMode(hdcX, TRANSPARENT);
	SetRect(lprcText, xGut*sizBmp.cy, xGut*sizBmp.cy, sizBmp.cx-xGut*sizBmp.cy, sizBmp.cy-xGut*sizBmp.cy);
	// // drawing the shadow first, then the text self
	switch(nShadow){
		case 1:
		{
			SetTextColor(hdcX, RGB(64, 64, 64));
			DrawTextW(hdcX, szText, -1, lprcText, nTxtPara);
			OffsetRect(lprcText, -lfHeight*0.1, -lfHeight*0.1);				// for the next...
			break;
		}
		case 2:		// TO BE REVISED - IT NOT WORKS CORRECTLY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		{
			float r = 2*lfHeight*0.1, th = 0;		// to rotate the text rectangle around.
			float x = 0, y = 0; 
			float x0 = r*cos(th), y0 = r*sin(th);
			for(th=0; th<=4*PI; th+=PI/4)
			{
				x = r*cos(th); 			
				y = r*sin(th); 
				if(th>2*PI){
					r+=lfHeight*0.1;				
					SetTextColor(hdcX, RGB(32, 32, 32));
				}
				OffsetRect(lprcText, x-x0, y-y0);		// offset the textout rect to create shadow around-and-around
				x0 = x; 	
				y0 = y; 
				DrawTextW(hdcX, szText, -1, lprcText, nTxtPara);
			}
			OffsetRect(lprcText, -r/2, 0);			// bring the rectangle in the center of the "aura"
			break;
		}
		
	}
	// then the text self
	SetTextColor(hdcX, RGB(204, 204, 80));
	if(nMode)
	{
		SetTextColor(hdcX, RGB(0x10, 0x10, 0x10));			// the text with mode==1: THE WATERMARK!!!!!!!
	}
	DrawTextW(hdcX, szText, -1, lprcText, nTxtPara);
	// when indirect, copy the text-image from memory in the output image:
	if(nMode)
	{
		SetStretchBltMode(hdc, HALFTONE);
		StretchBlt(hdc, 0, 0, sizBmp.cx, sizBmp.cy, hdcX, 0, 0, sizBmp.cx, sizBmp.cy, SRCPAINT);
		// AlphaBlend();		// ?????????????
	}
	// Closing operations
	SelectObject(hdcX, hfOld);
	GlobalFree(lprcText);
	if(nMode)
	{
		SelectObject(hdcMemTxt, hbmMemOld);
		DeleteObject(hbmMemTxt);
		DeleteDC(hdcMemTxt);
	}
	else
	{
		hdc = hdcX;
	}
	return 0;
}


int CXModifyJpgImage(HWND hwnd, double xNewWidth, double yNewHeight)
{
	// decompresses the JPG image from the "szFNmInp" file, 
	//	then modifies the size, colours, 
	// 	then shows a preview into the window handled by "hwnd", 
	//
	LPWSTR				szwFNm	= (LPWSTR)GlobalAlloc(GPTR, (MAX_PATH+1)*0x400+1);		// for many files...;			
	LPWSTR				szwFNI	= (LPWSTR)GlobalAlloc(GPTR, 0x200);			// file name = Input
	LPWSTR				szwFNO	= (LPWSTR)GlobalAlloc(GPTR, 0x200); 			// file name = Output
	// variables to display EXIF Info on form or output image:
	LPWSTR				wszOut 	= (LPWSTR)GlobalAlloc(GPTR, 0x200);		// -- displays some text
	LPWSTR				wszOuu 	= (LPWSTR)GlobalAlloc(GPTR, 0x200);		// -- displays geolocation
	LPWSTR				wszOuv 	= (LPWSTR)GlobalAlloc(GPTR, 0x800);		// -- display full available exif information
	
	wcscpy(szwFNm, L"MULTISELECT");		
	if(ProvideTheFileName(NULL, szwFNm)==3604)
	{
		// here code to get the FNI (as well as FNO) from FNm: 
		// swprintf(szwFNI, L"%s", szwFNm);				removed and replaced with the next line
		wcscpy(szwFNI, szwFNm);				// in this case, szwFNI must be the same as szwFNm // for debug only!!!!!!!
		// TODO TODO TODO: // code to extract individual files from the szwFNm
		// Preparing operations: reading the file EXIF header info, for next steps:
		HGLOBAL		hexifRes = GlobalAlloc(GHND, sizeof(EXIFInfo));
		EXIFInfo*	pexifRes = (EXIFInfo*)GlobalLock(hexifRes);
  		int			nexifRes = CXGetExifInfoEx(szwFNI, pexifRes);	// loads the structure pexifRes, also check if the szwFNI exists...
		    		
  		if(nexifRes!=-11)
		{
			// jpeg file dimensions: 
			int*				pWidth 		= (int*)GlobalAlloc(GPTR, sizeof(int));
			int*				pHeight 	= (int*)GlobalAlloc(GPTR, sizeof(int));
			double				xImgW = 0, yImgH = 0; 	// same as the above
			unsigned short		nOrientation;	// EXIF orientation tag

			if(nexifRes==0)
			{	
				xImgW = pexifRes->ImageWidth;
				yImgH = pexifRes->ImageHeight;
				nOrientation = pexifRes->Orientation;
				// prints the scan date...
				wchar_t		wszTmp[512];		// this will convert the character of bytes-text to wide-character text, necessary to display the right text!!!
				mbstowcs(wszTmp, pexifRes->DateTimeOriginal.c_str(), strlen(pexifRes->DateTimeOriginal.c_str())+1);
				swprintf(wszOut, L"%s", wszTmp);
				// prints the GPS Information:
				CXFormatGeoLocOutput(pexifRes, wszOuu, szwFNI);				
			}
			// here code to resize the image: setting the rectangle of the resized image:
			HDC					hdc;
			HDC					hdcMem;
			HBITMAP				hbm;
			HBITMAP				hbmOld;
			LPRECT				lprcX;				// not just for debug only
			
			if((xNewWidth==0)&&(yNewHeight==0))		// if resize dimensions not defined, show and prepare in its original size...
			{		
				xNewWidth = xImgW;
				yNewHeight = yImgH;
			}
			else			// otherwise keep the 
			{
				xNewWidth = 512;
				yNewHeight = 512;
			}
			lprcX = (LPRECT)GlobalAlloc(GPTR, sizeof(RECT));
			lprcX->left = 0;		
			lprcX->right = xNewWidth;				// this means: if both newWidht and newHeight are zero, then the original size will be used!
			lprcX->top = 0;												
			lprcX->bottom = yNewHeight;			
			// code to prepare the (memory- and display-) DC for the graphic operations
			hdc = GetDC(NULL);
			hdcMem = CreateCompatibleDC(hdc);
			hbm = CreateCompatibleBitmap(hdc, xNewWidth, yNewHeight); 	// here the new dimensions of the bitmap or the resize goes crazy - never known why!!!
			hbmOld = (HBITMAP)SelectObject(hdcMem, hbm);
	  		// now the operation self:
			decompress_to_dc(hdcMem, lprcX, szwFNI);	// here the function that reads the file and shows the image in the display DC
			// here code to show preview on the form...	// it cannot be prewiewed on form, why??? (see version A30??)	
			//////////////////////////////////////////////////////////////////////		MARK_3209
			
			HDC				hdcForm = GetDC(hwnd);
			SetStretchBltMode(hdc, HALFTONE);
			//////////////////////////////////////////////////////////////////////
			// write some text on the output image:		TODO: options for user input text 
			CXTextWithShadow(hdcMem, lprcX, wszOut, 0, 3, 3, 3, 1, 0);
			CXTextWithShadow(hdcMem, lprcX, L"SAMPLE", 0, 32, 2, 2, 0, 1);
			CXTextWithShadow(hdcMem, lprcX, wszOuu, 0, 3, 1, 3, 1, 0);		
			//////////////////////////////////////////////////////////////////////
			LPRECT			lprcShow = (LPRECT)GlobalAlloc(GPTR, sizeof(RECT));
			GetClientRect(hwnd, lprcShow);
			HBRUSH			hbrBgr = CreateSolidBrush(RGB(0x20, 0x20, 0xd0	));
			FillRect(hdcForm, lprcShow, hbrBgr);
			DeleteObject(hbrBgr);
			// center the preview image on the form, resize it and display on form-DC
			POINT			pCenter = {(lprcShow->right-lprcShow->left)/2, (lprcShow->bottom-lprcShow->top)/2};
			double 			wRatio = xNewWidth/yNewHeight;		// = width/height = dimX/dimY
			
			StretchBlt(hdcForm, pCenter.x-256, pCenter.y-256/wRatio, 512, 512/wRatio, hdcMem, 0, 0, xNewWidth, yNewHeight, SRCCOPY);
			GlobalFree(lprcShow);
			ReleaseDC(hwnd, hdcForm);
			// end of just for preview on form... is here, in fact!
			//////////////////////////////////////////////////////////////////////
			
			// then save the jpg file:
			CXAddFilePrefix(szwFNI, L"", L"_sam", szwFNO);
			compress_to_jpg(hdcMem, lprcX, szwFNO);	// compressing the modified picture from DC back into a JPG file...
			// and finally, closing operations:
			SelectObject(hdcMem, hbmOld);
			GlobalFree(lprcX);
			CloseHandle(hbm);
			CloseHandle(hbmOld);
			DeleteDC(hdcMem);
			ReleaseDC(NULL, hdc);
			//
			GlobalFree(pWidth);
			GlobalFree(pHeight);

		}
		GlobalUnlock(hexifRes);
		GlobalFree(hexifRes);
		
	}
	else
	{
		return -23;
	}
	GlobalFree(szwFNI);
	GlobalFree(szwFNO);
	GlobalFree(szwFNm);
	//
	GlobalFree(wszOuv);
	GlobalFree(wszOuu);
	GlobalFree(wszOut);
	return 0;
}

int CXFormatGeoLocOutput(EXIFInfo*	pexifRes, LPWSTR wszOuu, LPCWSTR wszFNm)
{
	if((pexifRes->GeoLocation.Latitude!=0)||(pexifRes->GeoLocation.Longitude!=0))	// if it exists, ...
	{	// no need for comments: this retrieves the GPS information recorded in the picture at shooting time
		swprintf(wszOuu, L"Location: %f%c (%02.0f%c %02.0f%c %02.3f%c) %c, %f%c (%02.0f%c %02.0f%c %02.3f%c) %c, %.0f m", 
	  					pexifRes->GeoLocation.Latitude,0x00b0, 
	          			pexifRes->GeoLocation.LatComponents.degrees, 0x00b0, 
	          			pexifRes->GeoLocation.LatComponents.minutes, '\'', 
	          			pexifRes->GeoLocation.LatComponents.seconds, '\"', 
	          			pexifRes->GeoLocation.LatComponents.direction, 
	  					pexifRes->GeoLocation.Longitude,0x00b0, 
	          			pexifRes->GeoLocation.LonComponents.degrees, 0x00b0, 
	          			pexifRes->GeoLocation.LonComponents.minutes, '\'', 
	          			pexifRes->GeoLocation.LonComponents.seconds, '\"', 
	          			pexifRes->GeoLocation.LonComponents.direction, 
	          			pexifRes->GeoLocation.Altitude);
	}
	else
	{	// just...
		if(wszFNm!=NULL)
			swprintf(wszOuu, L"File \"%s\" image ExIf header does not contain geolocation information", wszFNm);
	}
	return 0;	
}


int CXAddFilePrefix(LPCWSTR wszFNI, LPCWSTR wszPre, LPCWSTR wszSuf, LPWSTR wszFNO)
{	// this adds a prefix as well as a suffix to the filename contained in the complete path & filename wszFNI;
	// the output is wszFNO; an input like "c:\\tmp\\images\\testimage.jpeg" 
	//		produces the outut "c:\\tmp\\images\\tn_testimage_resized.jpeg"
	// particular variables
	LPWSTR			wszFNT = (LPWSTR)GlobalAlloc(GPTR, 0x200);	// Temporary string
	LPWSTR			wszFNU = (LPWSTR)GlobalAlloc(GPTR, 0x200);	// Temporary string
	LPWSTR			wszFND = (LPWSTR)GlobalAlloc(GPTR, 0x200);	// Temporary string

	int				nPos = 0;
	swprintf(wszFNT, L"%s%s", wszPre, wcsrchr(wszFNI, L'\\')+1);
	nPos = 	wcsrchr(wszFNI, L'\\')+1-wszFNI;	// warning!! conmpiler settings for WCHAR 
	wcsncpy(wszFND, wszFNI, nPos);
	swprintf(wszFNU, L"%s%s", wszFND, wszFNT);	
	if(wcsrchr(wszFNU, L'.')!=NULL)			// if there is a period in the complete filename...
	{
		if(wcsrchr(wszFNU, L'.')>(wcsrchr(wszFNU, L'\\')))	// if the file name have an extension...
		{
			swprintf(wszFNT, L"%s%s", wszSuf, wcsrchr(wszFNU, L'.'));	// reuse wszFNT
			nPos = wcsrchr(wszFNU, L'.')-wszFNU;	// the same warning for WCHAR
			wszFND = (LPWSTR)GlobalAlloc(GPTR, 0x200);	// reuse wszFND;
			wcsncpy(wszFND, wszFNU, nPos);
			swprintf(wszFNO, L"%s%s", wszFND, wszFNT);
		}
		else		// if the file name hat no extension, but is a period in the path name...
		{
			swprintf(wszFNO, L"%s%s", wszFNU, wszSuf);
		}
	}
	else	// the file name has no extension and no period ...
	{	// just add the suffix
		// wcscpy(wszFNO, wszFNU);
		swprintf(wszFNO, L"%s%s", wszFNU, wszSuf);
	}
	
	GlobalFree(wszFNT);
	GlobalFree(wszFNU);
	GlobalFree(wszFND);
	
	return 0;
}


void test__JPG2HWND(HWND hwnd)
{
	PAINTSTRUCT 		ps;
	HDC					hdc;
	RECT				rcClient;
	LPRECT				lprcDes;
	
	lprcDes = (LPRECT)GlobalAlloc(GPTR, sizeof(RECT));
	BeginPaint(hwnd, &ps);
	hdc = GetDC(hwnd);
	
	GetClientRect(hwnd, &rcClient);
	
	decompress_to_dc(hdc, lprcDes, L"c:\\tmp\\input.jpg");
	
	ReleaseDC(hwnd, hdc);
	EndPaint(hwnd, &ps);
	GlobalFree(lprcDes);
	
}

void test__HWND2JPG(HWND hwnd)
{
	
	PAINTSTRUCT		ps;
	HDC				hdc;
	RECT			rcClient;
	
	BeginPaint(hwnd, &ps);
	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rcClient);

	compress_to_jpg(hdc, &rcClient, L"C:\\TMP\\OUTPUT.JPG");
	
	ReleaseDC(hwnd, hdc);
	EndPaint(hwnd, &ps);
		
}

void test__HWND2JPG(HWND hwnd, LPCWSTR wszFNm)
{
	
	PAINTSTRUCT		ps;
	HDC				hdc;
	RECT			rcClient;
	
	BeginPaint(hwnd, &ps);
	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rcClient);

	compress_to_jpg(hdc, &rcClient, wszFNm);
	
	ReleaseDC(hwnd, hdc);
	EndPaint(hwnd, &ps);
		
}

void CXHwnd2Jpeg(HWND hwnd, LPCWSTR wszFNm)
{
	
	PAINTSTRUCT		ps;
	HDC				hdc;
	RECT			rcClient;
	
	BeginPaint(hwnd, &ps);
	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rcClient);

	compress_to_jpg(hdc, &rcClient, wszFNm);
	
	ReleaseDC(hwnd, hdc);
	EndPaint(hwnd, &ps);
		
}


void CatPath(char * BasePath, const char * FilePath)
{	// from "http://www.sentex.net/~mwandel/jhead/"
	const char slash = '\\';
    int l;
    
    l = strlen(BasePath);
    if (FilePath[1] == ':'){
        // Its a windows absolute path.
        l = 0;
    }
    if (FilePath[0] == slash || FilePath[0] == '.' || l == 0){
        // Its an absolute path, or there was no base path.
        strcpy(BasePath, FilePath);
        return;
    }
    if (BasePath[l-1] != slash){
        BasePath[l++] = slash;
        BasePath[l] = 0;
    }
    strcat(BasePath, FilePath);
    // Note that the combined path may contains things like "foo/../bar".   We assume
    // that the filesystem will take care of these.
}


void SaveFormImageOnMem(HWND hwnd, BOOL bUpd)
{
	// this function saves the image represented by the HDC of hwnd into the global memory image-handle g_hbm,
	// 		if bUpd = FALSE, and restores it from this memory when bUpd = TRUE
	PAINTSTRUCT			ps;
	HDC					hdc;		// a...
	HDC					hdcMem;
	// HBITMAP				hbm;	//   -replaced with the global variable g_hbm ... must be global to restore the content
	HBITMAP				hbmOld;		// ...a
    RECT				rcClient;	//
	
    // // opening procedure...
	GetClientRect(hwnd, &rcClient);
	if(bUpd) BeginPaint(hwnd, &ps);
	hdc = GetDC(hwnd);
	hdcMem = CreateCompatibleDC(hdc);				//
	if(!bUpd)
		g_hbm = CreateCompatibleBitmap(hdc, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top);
	hbmOld = (HBITMAP)SelectObject(hdcMem, g_hbm);
	// // saving or updating procedure
	if(bUpd)	// restoring the content of the DC of the client-area of the window from memory...
		BitBlt(hdc, 0, 0, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top, hdcMem, 0, 0, SRCCOPY);
	else		// saving the content of the DC of the client-area of the window into the memory...
		BitBlt(hdcMem, 0, 0, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top, hdc, 0, 0, SRCCOPY);
	// // closing procedure
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);
	ReleaseDC(hwnd, hdc);
	if(bUpd) EndPaint(hwnd, &ps);

}


DWORD ProvideTheFileName(HWND hwnd, LPWSTR szwFNm)
{
	OPENFILENAMEW 	ofn;       	// common dialog box structure
    
    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szwFNm;
    ofn.nMaxFile = (MAX_PATH+1)*0x400+1;
    ofn.lpstrFilter = L"JPEG Files (*.jpg)\0*.jpg\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER|OFN_PATHMUSTEXIST;
	if(wcsncmp(szwFNm, L"MULT", 4)==0)			// open multiple files
	{
		ofn.Flags|=OFN_ALLOWMULTISELECT;		// flag
	}
    // Display the Open dialog box.
    if(wcsncmp(szwFNm, L"info", 4)==0)			// Save as... // in this case, the file may or may not exists
    {
    	// TODO: Code to check if the file exists and options to overwrite or not
    	if(GetSaveFileNameW(&ofn)==TRUE)
    	{
    		return 3603;
    	}
    	else
    	{
	    	if(CommDlgExtendedError()!=0)
	    	{
	    		MessageBox(NULL, "Save file name error", "Message from programmer dude", MB_OK|MB_ICONERROR);
	    	}
    	}
    }
    else										// open
    {  
    	ofn.Flags|=OFN_FILEMUSTEXIST;			// to be opened
		if (GetOpenFileNameW(&ofn)==TRUE)
	    {
			return 3604;
	    }
	    else
	    {
	    	if(CommDlgExtendedError()!=0)
	    	{
	    		MessageBox(NULL, "Open file name error", "Message from programmer dude", MB_OK|MB_ICONERROR);
	    	}
	    }
	}
	return 1137;
}


int CXGetExifInfoEx(LPCWSTR szwFNm, EXIFInfo* pexifRes)	
{
	// gets the EXIF INFO in a pointer to EXIFInfo structure..
	// returns -11 in case the file does not exists...
	DWORD 		dwBytesRead = 0;
	int			nOut = 0;			// when all OK, the function returns this value;
	
	if(CXGetJpgBytes(szwFNm, &dwBytesRead, NULL)==-11)			
	{
		return -11;		// the file does not exists...
	}
	else
	{
		// just get the file size first...
		HANDLE 			hJpgBytes = GlobalAlloc(GHND, dwBytesRead);			// buffer to receive the JPG bytes
		unsigned char* 	szJpgBytes = (unsigned char*)GlobalLock(hJpgBytes);	// buffer to receive the JPG bytes
	
		CXGetJpgBytes(szwFNm, &dwBytesRead, szJpgBytes);			// just get the file size first...
		
		EXIFInfo		exifTmp;
		if(exifTmp.parseFrom(szJpgBytes, dwBytesRead))		// does not works direct with pexifRes!!! WHY?????
		{
			nOut = -3;			// that means, no exit info...
		}
		GlobalUnlock(hJpgBytes);
		CloseHandle(hJpgBytes);
		memcpy(pexifRes, &exifTmp, sizeof(EXIFInfo));		
	}
	return nOut;
}

int CXFullBlue(BYTE* blue, BYTE* green, BYTE* red)
{	// fancy stuff, not something serious
	if(
		((*red+32)<(*blue))&&
		((*green+32)<(*blue))&&
		((*blue)>16)&&
		TRUE)
	{
		*red = 0;
		*green = 0;
		*blue = 255;
	}
	return 0;
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////`
