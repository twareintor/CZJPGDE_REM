/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "cxbars.h"


CZTitleBar::CZTitleBar(HWND hwnd, int nHeight, LPRECT lprcAreas, BOOL bActive)
{	// this version does not have minimize or maximize/restore buttons
	LPRECT			lprcClient;
	LPRECT			lprcTitle;		// a rectangle for the entire title bar
	LPRECT			lprcLMenu;		// rectangle for the top-left menu button
	LPRECT			lprcClose;		// rectangle for the top-right close button
	LPRECT			lprcCText;		// rectangle for the window-caption button (move button)

	lprcClient = (LPRECT)GlobalAlloc(GPTR, sizeof(RECT));
	lprcTitle = (LPRECT)GlobalAlloc(GPTR, sizeof(RECT));
	lprcLMenu = (LPRECT)GlobalAlloc(GPTR, sizeof(RECT));
	lprcClose = (LPRECT)GlobalAlloc(GPTR, sizeof(RECT));
	lprcCText = (LPRECT)GlobalAlloc(GPTR, sizeof(RECT));
	
	GetClientRect(hwnd, lprcClient);
	SetRect(lprcTitle, lprcClient->left, lprcClient->top, lprcClient->right, lprcClient->top+nHeight);
	SetRect(lprcLMenu, lprcTitle->left, lprcTitle->top, lprcTitle->left+nHeight, lprcTitle->bottom);
	SetRect(lprcCText, lprcTitle->left+nHeight, lprcTitle->top, lprcTitle->right-nHeight, lprcTitle->bottom);
	SetRect(lprcClose, lprcTitle->right-nHeight, lprcTitle->top, lprcTitle->right, lprcTitle->bottom);
	
	if(lprcAreas!=NULL)
	{	// getting only the information about the areas (buttons) wherein to click for an event
		memcpy(lprcAreas+0, lprcLMenu, sizeof(RECT));	// top-left menu [-]button (Windows 3.11 style)
		memcpy(lprcAreas+1, lprcCText, sizeof(RECT));	// window caption (move) button, then place for minimize, maximize/restore button not present here
		memcpy(lprcAreas+4, lprcClose, sizeof(RECT));	// top-right close [X] button
	}
	else
	{	// drawing the toolbars as well as printing the window caption on the title bar
		LPTSTR			sztCaption;
		LPWSTR			szwCaption;
		
		szwCaption = (LPWSTR)GlobalAlloc(GPTR, MAX_PATH*12);
		sztCaption = (LPTSTR)GlobalAlloc(GPTR, MAX_PATH*12);
		
		GetWindowText(hwnd, sztCaption, MAX_PATH*12);		// getting the window caption, and setting it to Unicode...
		mbstowcs(szwCaption, sztCaption, strlen(sztCaption)+1);
		GlobalFree(sztCaption);

		PAINTSTRUCT		ps;
		HDC				hdc;
		HBRUSH			hbr, hbrOld;
		BeginPaint(hwnd, &ps);
		hdc = GetDC(hwnd);

		LineTo(hdc, lprcClient->right-lprcClient->left, 0);
		LineTo(hdc, lprcClient->right-lprcClient->left, lprcClient->bottom-lprcClient->top);
		LineTo(hdc, 0, lprcClient->bottom-lprcClient->top);
		LineTo(hdc, 0, 0);
		
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 192));

		hbr = CreateSolidBrush((RGB(102, 125, 102))*(bActive)+(RGB(225, 225, 225))*(!bActive));
		hbrOld = (HBRUSH)SelectObject(hdc, hbr);
		RoundRect(hdc, lprcLMenu->left-lprcTitle->left, lprcLMenu->top-lprcTitle->top, lprcLMenu->right-lprcTitle->left, lprcLMenu->bottom-lprcTitle->top, 4, 4);
		DrawTextW(hdc, L"-", -1, lprcLMenu, DT_SINGLELINE|DT_CENTER|DT_BOTTOM);
		hbr = CreateSolidBrush((RGB(109, 109, 255))*(bActive)+(RGB(225, 225, 225))*(!bActive));
		hbrOld = (HBRUSH)SelectObject(hdc, hbr);
		RoundRect(hdc, lprcCText->left-lprcTitle->left, lprcCText->top-lprcTitle->top, lprcCText->right-lprcTitle->left, lprcCText->bottom-lprcTitle->top, 4, 4);
		DrawTextW(hdc, szwCaption, -1, lprcCText, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		GlobalFree(szwCaption);
		hbr = CreateSolidBrush((RGB(255, 109, 109))*(bActive)+(RGB(225, 225, 225))*(!bActive));
		hbrOld = (HBRUSH)SelectObject(hdc, hbr);
		RoundRect(hdc, lprcClose->left-lprcTitle->left, lprcClose->top-lprcTitle->top, lprcClose->right-lprcTitle->left, lprcClose->bottom-lprcTitle->top, 4, 4);
		DrawTextW(hdc, L"x", -1, lprcClose, DT_CENTER|DT_VCENTER);
		
		DeleteObject(hbr);		
		SelectObject(hdc, hbrOld);
		DeleteObject(hbrOld);
		ReleaseDC(hwnd, hdc);
		EndPaint(hwnd, &ps);
	}

	GlobalFree(lprcTitle);
	GlobalFree(lprcLMenu);
	GlobalFree(lprcClose);
	GlobalFree(lprcCText);
	GlobalFree(lprcClient);
}

CZToolbar::CZToolbar(HWND hwnd, int nGutter, int nHeight, LPRECT lprcButtons)
{
	PAINTSTRUCT		ps;
	HDC				hdc;
	
	LPRECT			lprcClient;
	LPRECT			lprcTools;
	LPRECT			lprcOpenF;
	
	lprcOpenF = (LPRECT)GlobalAlloc(GPTR, sizeof(RECT));
	GetClientRect(hwnd, lprcClient);
	
	SetRect(lprcTools, lprcClient->left, lprcClient->top+nGutter, lprcClient->right, lprcClient->top+nGutter+nHeight);
	SetRect(lprcOpenF, lprcTools->left, lprcTools->top, lprcTools->left+100, lprcTools->bottom);
	
	BeginPaint(hwnd, &ps);
	hdc = GetDC(hwnd);
	SetBkMode(hdc, TRANSPARENT);
	RoundRect(hdc, lprcOpenF->left-lprcClient->left, lprcOpenF->top-lprcClient->top, lprcOpenF->right-lprcClient->left, lprcOpenF->bottom-lprcClient->top, 4, 4);
	DrawTextW(hdc, L"Open...", -1, lprcOpenF, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	ReleaseDC(hwnd, hdc);
	EndPaint(hwnd, &ps);
	
	GlobalFree(lprcOpenF);
	GlobalFree(lprcTools);
	GlobalFree(lprcClient);
}


/**********************************************************************************************************


		case WM_LBUTTONDOWN:
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
						case 0:
						{
							MessageBoxW(hwnd, L"CZ Image View", L"Application information", MB_OK|MB_ICONINFORMATION); 
							break;	
						}
						case 1:
						{
							MessageBox(NULL, "Move To:", NULL, MB_OK); 
							break;
						}
						case 4:
						{
							int nMsgRes = 
								MessageBoxW(hwnd, L"Are you sure you want to close?", L"CZ Image application close", MB_YESNO|MB_ICONQUESTION); 
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
			if(CXModifyJpgImage(hwnd,0,120)>=0)
			{
				SaveFormImageOnMem(hwnd,FALSE);
				MessageBox(NULL,"Done!","CXJPGDE application",MB_OK|MB_ICONINFORMATION);
			}
			break;
		}
		case WM_PAINT:
		{
			CZTitleBar	cztitlebar(hwnd, 18, NULL, TRUE);
			// place to draw toolbar(s)
			SaveFormImageOnMem(hwnd, TRUE);
			break;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam)){
				case ID_FILE_EXIT:
				{
					PostQuitMessage(0);
					break;
				}
			}
			break;
		}
		case WM_KILLFOCUS:
		{
			CZTitleBar		cztb(hwnd, 18, NULL, FALSE);
			break;
		}
		case WM_SETFOCUS:
		{
			CZTitleBar		cztb(hwnd, 18, NULL, TRUE);
			break;
		}



**********************************************************************************************************/














/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
