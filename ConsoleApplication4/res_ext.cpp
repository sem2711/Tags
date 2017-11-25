#include <iostream>
#include <Windows.h>
#include <Winuser.h>
#include <tchar.h>
#include <StrSafe.h>
#include <sstream>

TCHAR szFileName[MAX_PATH];

// DWORD-alignment macro
#define DWORD_ALIGN(x)  x = ((x + 3) & ~3)

#define LANG_NAME_LEN				16 
#define INFO_LINE_LEN				1024
#define RES_TYPE_LEN				64


// Table of Locale ID and Language codes mappings
LPTSTR language_codes[][3] = {
	{ _T("1025"),			_T("AB"),				_T("ar") },
	{ _T("1046"),			_T("BR"),				_T("pt-BR") },
	{ _T("1027"),			_T("CA"),				_T("ca") },
	{ _T("1029"),			_T("CZ"),				_T("cs") },
	{ _T("1030"),			_T("DK"),				_T("da") },
	{ _T("1031"),			_T("D"),				_T("de") },
	{ _T("1032"),			_T("GR"),				_T("el") },
	{ _T("1033"),			_T("EN"),				_T("en") },
	{ _T("2057"),			_T("B"),				_T("en-GB") },
	{ _T("1034"),			_T("E"),				_T("es") },
	{ _T("2058"),			_T("MX"),				_T("es-MX") },
	{ _T("1035"),			_T("K"),				_T("fi") },
	{ _T("1036"),			_T("FU"),				_T("fr") },
	{ _T("1037"),			_T("HB"),				_T("he") },
	{ _T("1050"),			_T("CR"),				_T("hr") },
	{ _T("1038"),			_T("MG"),				_T("hu") },
	{ _T("1040"),			_T("T"),				_T("it") },
	{ _T("1043"),			_T("N"),				_T("nl") },
	{ _T("1044"),			_T("H"),				_T("nb") },
	{ _T("1045"),			_T("PL"),				_T("pl") },
	{ _T("2070"),			_T("PO"),				_T("pt") },
	{ _T("1048"),			_T("RO"),				_T("ro") },
	{ _T("1049"),			_T("RS"),				_T("ru") },
	{ _T("1051"),			_T("SL"),				_T("sk") },
	{ _T("1053"),			_T("S"),				_T("sv") },
	{ _T("1055"),			_T("TU"),				_T("tr") },
	{ _T("1058"),			_T("UK"),				_T("ua") },
	{ _T("3081"),			_T("X"),				_T("en-AU") },
	{ _T("1028"),			_T("CHT"),				_T("zh-Hant") },
	{ _T("2052"),			_T("CHS"),				_T("zh-Hans") },
	{ _T("1057"),			_T("ID"),				_T("id") },
	{ _T("1086"),			_T("MY"),				_T("ms") },
	{ _T("1066"),			_T("VN"),				_T("vi") },
	{ _T("1054"),			_T("TH"),				_T("th") },
	{ _T("1041"),			_T("J"),				_T("ja") },
	{ _T("1042"),			_T("KH"),				_T("ko") },
	{ _T("4100"),			_T("CSG"),				_T("zh-SG") }
};


LPTSTR windowStyles[] = {
	TEXT("WS_TABSTOP"), TEXT("WS_GROUP"), TEXT("WS_THICKFRAME"), TEXT("WS_SYSMENU"), TEXT("WS_HSCROLL"), 
	TEXT("WS_VSCROLL"), TEXT("WS_DLGFRAME"), TEXT("WS_BORDER"), TEXT("WS_MAXIMIZE"), TEXT("WS_CLIPCHILDREN"),
	TEXT("WS_CLIPSIBLINGS"), TEXT("WS_DISABLED"), TEXT("WS_VISIBLE"), TEXT("WS_MINIMIZE"), TEXT("WS_CHILD"), TEXT("WS_POPUP")
};

LPTSTR dialogStyles[] = {
	TEXT("DS_ABSALIGN"), TEXT("DS_SYSMODAL"), TEXT("DS_3DLOOK"), TEXT("DS_FIXEDSYS"), TEXT("DS_NOFAILCREATE"), TEXT("DS_LOCALEDIT"), TEXT("DS_SETFONT"), 
	TEXT("DS_MODALFRAME"), TEXT("DS_NOIDLEMSG"), TEXT("DS_SETFOREGROUND"), TEXT("DS_CONTROL"), TEXT("DS_CENTER"), TEXT("DS_CENTERMOUSE"), TEXT("DS_CONTEXTHELP"),
};

LPTSTR buttonStyles[] = {
	TEXT("BS_LEFTTEXT"), TEXT("BS_ICON"), TEXT("BS_BITMAP"), TEXT("BS_LEFT"), TEXT("BS_RIGHT"), 
	TEXT("BS_TOP"), TEXT("BS_BOTTOM"), TEXT("BS_PUSHLIKE"), TEXT("BS_MULTILINE"), TEXT("BS_NOTIFY"), TEXT("BS_FLAT")
};

LPTSTR staticStyles[] = {
	TEXT("SS_REALSIZECONTROL"), TEXT("SS_NOPREFIX"), TEXT("SS_NOTIFY"), TEXT("SS_CENTERIMAGE"), TEXT("SS_RIGHTJUST"), 
	TEXT("SS_REALSIZEIMAGE"), TEXT("SS_SUNKEN"), TEXT("SS_EDITCONTROL"), TEXT("SS_ENDELLIPSIS"), TEXT("SS_PATHELLIPSIS")
};

LPTSTR listboxStyles[]{
	TEXT("LBS_NOTIFY"), TEXT("LBS_SORT"), TEXT("LBS_NOREDRAW"), TEXT("LBS_MULTIPLESEL"), TEXT("LBS_OWNERDRAWFIXED"), TEXT("LBS_OWNERDRAWVARIABLE"), 
	TEXT("LBS_HASSTRINGS"), TEXT("LBS_USETABSTOPS"), TEXT("LBS_NOINTEGRALHEIGHT"), TEXT("LBS_MULTICOLUMN"), TEXT("LBS_WANTKEYBOARDINPUT"), TEXT("LBS_EXTENDEDSEL"),
	TEXT("LBS_DISABLENOSCROLL"), TEXT("LBS_NODATA"), TEXT("LBS_NOSEL"), TEXT("LBS_COMBOBOX")
};

LPTSTR comboStyles[] = {
	TEXT("CBS_OWNERDRAWFIXED"), TEXT("CBS_OWNERDRAWVARIABLE"), TEXT("CBS_AUTOHSCROLL"), TEXT("CBS_OEMCONVERT"), TEXT("CBS_SORT"), 
	TEXT("CBS_HASSTRINGS"), TEXT("CBS_NOINTEGRALHEIGHT"), TEXT("CBS_DISABLENOSCROLL"), TEXT("CBS_UPPERCASE"), TEXT("CBS_LOWERCASE")
};

LPTSTR editStyles[] = {
	TEXT("ES_CENTER"), TEXT("ES_RIGHT"), TEXT("ES_MULTILINE"), TEXT("ES_UPPERCASE"), TEXT("ES_LOWERCASE"), TEXT("ES_PASSWORD"), TEXT("ES_AUTOVSCROLL"), 
	TEXT("ES_AUTOHSCROLL"), TEXT("ES_NOHIDESEL"), TEXT("ES_OEMCONVERT"), TEXT("ES_READONLY"), TEXT("ES_WANTRETURN"), TEXT("ES_NUMBER"), TEXT("ES_LEFT")
};

LPTSTR menuStyles[] = {
	TEXT("MF_GRAYED"), TEXT("MF_DISABLED"), TEXT("MF_BITMAP"), TEXT("MF_CHECKED"), TEXT("MF_POPUP"), TEXT("MF_MENUBARBREAK"),
	TEXT("MF_MENUBREAK"), TEXT("MF_END"), TEXT("MF_OWNERDRAW"), TEXT("MF_USECHECKBITMAPS"), TEXT("MF_BYPOSITION"), TEXT("MF_SEPARATOR"),
	TEXT("MF_DEFAULT"), TEXT("MF_SYSMENU"), TEXT("MF_RIGHTJUSTIFY"), TEXT("MF_MOUSESELECT")
};

TCHAR szResourcePath[MAX_PATH];
TCHAR szDirectoryPath[MAX_PATH];

bool parseArguments(int argc, TCHAR * argv[]);
void printUsage();
BOOL ProcessDialogResource(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName, LPTSTR szCurrDirPath);
BOOL ProcessMenuResource(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName, LPTSTR szCurrDirPath);
BOOL ProcessStringResource(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName, LPTSTR szCurrDirPath);
BOOL SaveInfo(LPCTSTR szFilePath, LPCTSTR szValue);
BOOL GetDirectoryPath(LPCTSTR szFilePath, LPTSTR szDirPath);
BOOL ParseDialogStyle(DWORD style, std::wstring& str);
BOOL ParseControlStyle(DWORD style, WORD controlType, std::wstring& str);
BOOL ParseWindowStyle(DWORD style, std::wstring& str);
BOOL GetMenuOption(WORD wOption, std::wstring& str);

BOOL ResTypeProc(HMODULE hDll, LPTSTR lpType, LONG_PTR lParam);
BOOL ResNameProc(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName, LONG_PTR lParam);
BOOL ResLangProc(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName, WORD wLanguage, LONG_PTR lParam);

int _tmain(int argc, TCHAR * argv[]) {
	bool ok = parseArguments(argc, argv);
	if (!ok) {
		printUsage();
		exit(1);
	}
	HMODULE hDll = LoadLibraryEx(szResourcePath, NULL, LOAD_LIBRARY_AS_IMAGE_RESOURCE);
	if (hDll == NULL) {
		DWORD dwErrCode = GetLastError();
		std::cout << "Unable to load library\nError code: " << dwErrCode << std::endl;
		exit(dwErrCode);
	}
	TCHAR szCurrPath[MAX_PATH] = { 0 };
	BOOL fOK = EnumResourceTypes(hDll, (ENUMRESTYPEPROC)ResTypeProc, (LONG_PTR)szCurrPath);
	FreeLibrary(hDll);
	return 0;
}

bool parseArguments(int argc, TCHAR * argv[])
{
	bool res = true;
	if (argc < 2) {
		return false;
	}
	StringCchCopy(szResourcePath, STRSAFE_MAX_CCH, argv[1]);
	GetDirectoryPath(szResourcePath, szDirectoryPath);
	return res;
}

void printUsage() {

}

BOOL ResTypeProc(HMODULE hDll, LPTSTR lpType, LONG_PTR lParam) {
	BOOL res = TRUE;
	TCHAR szResType[RES_TYPE_LEN];
	if (IS_INTRESOURCE(lpType))
	{
		USHORT uTypeCode = (USHORT)lpType;
		switch (uTypeCode) {
		case 11:
			StringCchCopy(szResType, RES_TYPE_LEN, TEXT("RT_MESSAGETABLE"));
			break;
		case 5:
			StringCchCopy(szResType, RES_TYPE_LEN, TEXT("RT_DIALOG"));
			break;
		case 4:
			StringCchCopy(szResType, RES_TYPE_LEN, TEXT("RT_MENU"));
			break;
		case 6:
			StringCchCopy(szResType, RES_TYPE_LEN, TEXT("RT_STRING"));
			break;
		default:
			return TRUE;
		}
	}
	else
	{
		StringCchPrintf((LPTSTR)lParam, MAX_PATH, TEXT("%s%s\\"), szDirectoryPath, lpType);
	}
	// Create a path for a specified resource type.
	StringCchPrintf((LPTSTR)lParam, MAX_PATH, TEXT("%s%s\\"), szDirectoryPath, szResType);
	CreateDirectory((LPTSTR)lParam, NULL);
	
	return EnumResourceNames(hDll, lpType, (ENUMRESNAMEPROC)ResNameProc, lParam);
}

BOOL ResNameProc(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName, LONG_PTR lParam) {
	// Default return value
	BOOL res = TRUE;
	size_t chNameLen = 0;
	HRESULT hRes;
	if ((USHORT)lpType != 0x0006) {
		hRes = StringCchLength(lpName, STRSAFE_MAX_CCH, &chNameLen);
		if (FAILED(hRes)) {
			std::wcout << "Error in Resource Name. Resource Type : " << (LPTSTR)lParam << std::endl;
			return FALSE;
		}
		if (chNameLen > 0) {
			std::wcout << lpName << std::endl;
		}
		else {
			std::cout << "No resource found...\n";
		}
	}
	else {
		std::wcout << (USHORT)lpName << std::endl;
	}

	if (IS_INTRESOURCE(lpType)) {
		USHORT uResType = (USHORT)lpType;
		switch (uResType) {
		case 4: // Menu
			res = ProcessMenuResource(hModule, lpType, lpName, (LPTSTR)lParam);
			break;
		case 5: // Dialog
			res = ProcessDialogResource(hModule, lpType, lpName, (LPTSTR)lParam);
			break;
		case 6:
			res = ProcessStringResource(hModule, lpType, lpName, (LPTSTR)lParam);
			break;
		case 11:
			break;
		default:
			break;
		}
	}
	return res;
}

BOOL ProcessStringResource(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName, LPTSTR szCurrDirPath) {
	LPVOID lpResource = NULL;
	HGLOBAL hgResource = NULL;
	TCHAR szInfoLine[INFO_LINE_LEN] = { 0 };

	HRSRC hRsrc = FindResource(hModule, lpName, lpType);
	if (hRsrc != NULL) {
		HGLOBAL hgResource = LoadResource(hModule, hRsrc);
		if (hgResource != NULL) {
			DWORD dwResSize = SizeofResource(hModule, hRsrc);
			lpResource = LockResource(hgResource);
			if (lpResource != NULL) {
				// Use these to keep track of where we're at
				DWORD dwDataSize = 0;
				StringCchPrintf(szFileName, MAX_PATH, TEXT("%s%i.txt"), szCurrDirPath, (USHORT)lpName);

				// This guy will move through the global data
				LPWORD wPointer = NULL;


				// Cast a BYTE pointer to the global data 
				// so we can manipulate it
				BYTE *pbyGlobalData = (BYTE *)(lpResource);
				wPointer = (LPWORD)pbyGlobalData;
				LPWORD wInitial = wPointer;

				// Get the language for Menu
				TCHAR szResLang[LANG_NAME_LEN] = { 0 };
				EnumResourceLanguages(hModule, lpType, lpName, (ENUMRESLANGPROC)ResLangProc, (LONG_PTR)szResLang);
				StringCchPrintf(szInfoLine, INFO_LINE_LEN, TEXT("%i STRINGTABLE\r\nLANGUAGE %s"), (USHORT)lpName, szResLang);
				SaveInfo(szFileName, szInfoLine);
				while (*(wPointer++) == 0x0000);
				wPointer--;
				WORD stringLen = 0;

				while (dwDataSize < dwResSize) {
					stringLen = *wPointer;
					if (stringLen == 0) {
						while (*(wPointer++) == 0x0000);
						wPointer--;
						WORD optStats = *wPointer;
						dwDataSize = ((LPBYTE)wPointer - (LPBYTE)wInitial);
						continue;
					}
					wPointer += 1;
					TCHAR szString[4096] = { 0 };
					StringCchCopy(szString, stringLen + 1, (LPTSTR)wPointer);
					wPointer += stringLen;
					StringCchPrintf(szInfoLine, INFO_LINE_LEN, TEXT("\"%s\""), szString);
					SaveInfo(szFileName, szInfoLine);
					while (*(wPointer++) == 0x0000);
					wPointer--;
					dwDataSize = ((LPBYTE)wPointer - (LPBYTE)wInitial);
				}

				// Cleanup
				if (lpResource != NULL && hgResource != NULL)
				{
					UnlockResource(hgResource);
					FreeResource(hgResource);
				}
				SaveInfo(szFileName, TEXT("\r\n\r\n"));
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
		else {
			DWORD dwErrCode = GetLastError();
			std::cout << "Failed to load a resource. Error code: " << dwErrCode << std::endl;
			return FALSE;
		}
	}
	else {
		DWORD dwErrCode = GetLastError();
		std::cout << "Failed to find a resource. Error code: " << dwErrCode << std::endl;
		return FALSE;
	}
}

BOOL ProcessMenuResource(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName, LPTSTR szCurrDirPath) {
	LPVOID lpResource = NULL;
	HGLOBAL hgResource = NULL;
	TCHAR szInfoLine[INFO_LINE_LEN] = { 0 };

	HRSRC hRsrc = FindResource(hModule, lpName, lpType);
	if (hRsrc != NULL) {
		HGLOBAL hgResource = LoadResource(hModule, hRsrc);
		if (hgResource != NULL) {
			PMENUITEMTEMPLATEHEADER lpMenuHeader;
			PMENUITEMTEMPLATE lpMenuItem;
			
			DWORD dwResSize = SizeofResource(hModule, hRsrc);
			lpResource = LockResource(hgResource);
			if (lpResource != NULL) {
				// Use these to keep track of where we're at
				DWORD dwMenuItemTemplateHeaderSize = sizeof(MENUITEMTEMPLATEHEADER);
				DWORD dwMenuItemTemplateSize = sizeof(MENUITEMTEMPLATE);
				DWORD dwDataPosition = 0;
				DWORD dwDataSize = 0;
				StringCchPrintf(szFileName, MAX_PATH, TEXT("%s%s.txt"), szCurrDirPath, lpName);

				// This guy will move through the global data
				BYTE *wPointer = NULL;


				// Cast a BYTE pointer to the global data 
				// so we can manipulate it
				BYTE *pbyGlobalData = (BYTE *)(lpResource);

				// This is the menu header
				lpMenuHeader = (PMENUITEMTEMPLATEHEADER)pbyGlobalData;
				dwDataPosition = dwMenuItemTemplateHeaderSize + lpMenuHeader->offset;
				wPointer = pbyGlobalData;
				wPointer += (dwDataPosition * 2);
				dwDataSize += (dwDataPosition * 2);

				// Get the language for Menu
				TCHAR szResLang[LANG_NAME_LEN] = { 0 };
				EnumResourceLanguages(hModule, lpType, lpName, (ENUMRESLANGPROC)ResLangProc, (LONG_PTR)szResLang);
				StringCchPrintf(szInfoLine, INFO_LINE_LEN, TEXT("%s MENU\r\nLANGUAGE %s"), lpName, szResLang);
				SaveInfo(szFileName, szInfoLine);

				while (dwDataSize < dwResSize) {
					lpMenuItem = (PMENUITEMTEMPLATE)wPointer;
					//TCHAR szMenuOption[LANG_NAME_LEN] = { 0 };
					//std::wstring strOption;
					//BOOL fHasOption = GetMenuOption(lpMenuItem->mtOption, strOption);
					size_t strLen = 0;
					StringCbLength(lpMenuItem->mtString, STRSAFE_MAX_LENGTH, &strLen);
					if (strLen > 0) {
						StringCchPrintf(szInfoLine, INFO_LINE_LEN, TEXT("MENUITEM %i, \"%s\""), lpMenuItem->mtID, lpMenuItem->mtString);
					}
					else {
						if (lpMenuItem->mtID == 0) {
							StringCchPrintf(szInfoLine, INFO_LINE_LEN, TEXT("MENUITEM SEPARATOR"));
						}
					}
					SaveInfo(szFileName, szInfoLine);
					dwDataPosition = (dwMenuItemTemplateSize + strLen);
					wPointer += dwDataPosition;
					dwDataSize += dwDataPosition;
				}

				// Cleanup
				if (lpResource != NULL && hgResource != NULL)
				{
					UnlockResource(hgResource);
					FreeResource(hgResource);
				}
				SaveInfo(szFileName, TEXT("\r\n\r\n"));
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
		else {
			DWORD dwErrCode = GetLastError();
			std::cout << "Failed to load a resource. Error code: " << dwErrCode << std::endl;
			return FALSE;
		}
	}
	else {
		DWORD dwErrCode = GetLastError();
		std::cout << "Failed to find a resource. Error code: " << dwErrCode << std::endl;
		return FALSE;
	}
}

BOOL ProcessDialogResource(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName, LPTSTR szCurrDirPath) {

	LPVOID lpResource = NULL;
	HGLOBAL hgResource = NULL;
	TCHAR szInfoLine[INFO_LINE_LEN] = { 0 };

	HRSRC hRsrc = FindResource(hModule, lpName, lpType);
	if (hRsrc != NULL) {
		HGLOBAL hgResource = LoadResource(hModule, hRsrc);
		if (hgResource != NULL) {
			// Use these to enumerate the resource controls
			LPDLGTEMPLATE lpTemplate;
			LPDLGITEMTEMPLATE lpItemTemplate;

			DWORD dwResSize = SizeofResource(hModule, hRsrc);
			lpResource = LockResource(hgResource);
			if (lpResource != NULL) {
				// Use these to keep track of where we're at
				DWORD dwDlgTemplateSize = sizeof(DLGTEMPLATE);
				DWORD dwDlgItemTemplateSize = sizeof(DLGITEMTEMPLATE);
				DWORD dwDataPosition = 0;
				StringCchPrintf(szFileName, MAX_PATH, TEXT("%s%s.txt"), szCurrDirPath, lpName);

				// This guy will move through the global data
				WORD *wPointer = NULL;
				

				// Cast a BYTE pointer to the global data 
				// so we can manipulate it
				BYTE *pbyGlobalData = (BYTE *)(lpResource);

				// This is the dialog itself
				lpTemplate = (LPDLGTEMPLATE)pbyGlobalData;

				// Printing the main information about the dialog
				StringCchPrintf(szInfoLine, INFO_LINE_LEN, TEXT("%s DIALOG %i, %i, %i, %i"), lpName, lpTemplate->x, lpTemplate->y, lpTemplate->cx, lpTemplate->cy);
				SaveInfo(szFileName, szInfoLine);
				// Printing style
				std::wstring strStyles;
				ParseDialogStyle(lpTemplate->style, strStyles);
				ParseWindowStyle(lpTemplate->style, strStyles);
				StringCchPrintf(szInfoLine, INFO_LINE_LEN, TEXT("STYLE %s"), strStyles.c_str());
				SaveInfo(szFileName, szInfoLine);

				dwDataPosition = dwDlgTemplateSize;

				// The dialog's menu is the first 
				// item after the dialog template (1.)	
				wPointer = (WORD *)(pbyGlobalData + dwDataPosition);
				WORD *wInitial = wPointer;
				WORD wSign = *wPointer;
				WORD wMenuOrd;

				switch (wSign) {
				case 0x0000:
					StringCchPrintf(szInfoLine, INFO_LINE_LEN, TEXT("%s"), TEXT("NO MENU"));
					SaveInfo(szFileName, szInfoLine);
					// No menu resource
					wPointer += 1;
					break;
				case 0xffff:
					// There's one other element which specifies
					// the ordinal value of the menu
					wPointer += 1;
					wMenuOrd = (*wPointer);
					StringCchPrintf(szInfoLine, INFO_LINE_LEN, TEXT("Menu ID: %i"), wMenuOrd);
					SaveInfo(szFileName, szInfoLine);
					wPointer += 1;
					break;
				default:
					std::wstring strMenuResource((LPCWSTR)wPointer);
					SaveInfo(szFileName, strMenuResource.c_str());
					while ((*wPointer++) != 0x0000);
					break;
				}

				wSign = *wPointer;

				switch (wSign) {
				case 0x0000:
					StringCchPrintf(szInfoLine, INFO_LINE_LEN, TEXT("%s"), TEXT("Predefined Window class"));
					SaveInfo(szFileName, szInfoLine);
					// No menu resource
					wPointer += 1;
					break;
				case 0xffff:
					// There's one other element which specifies
					// the ordinal value of the menu
					wPointer += 1;
					wMenuOrd = (*wPointer);
					StringCchPrintf(szInfoLine, INFO_LINE_LEN, TEXT("CLASS ID: %i"), wMenuOrd);
					SaveInfo(szFileName, szInfoLine);
					wPointer += 1;
					break;
				default:
					std::wstring strClassResource((LPCWSTR)wPointer);
					StringCchPrintf(szInfoLine, INFO_LINE_LEN, TEXT("CLASS %s"), strClassResource.c_str());
					SaveInfo(szFileName, szInfoLine);
					while ((*wPointer++) != 0x0000);
					break;
				}

				wSign = *wPointer;
				
				switch (wSign) {
				case 0x0000:
					StringCchPrintf(szInfoLine, INFO_LINE_LEN, TEXT("%s"), TEXT("NO TITLE"));
					SaveInfo(szFileName, szInfoLine);
					// No menu resource
					wPointer += 1;
					break;
				default:
					std::wstring strTitleResource((LPCWSTR)wPointer);
					StringCchPrintf(szInfoLine, INFO_LINE_LEN, TEXT("TITLE %i"), strTitleResource.c_str());
					SaveInfo(szFileName, szInfoLine);
					while ((*wPointer++) != 0x0000);
					break;
				}

				// Getting a language name for current resource.
				TCHAR szLangCode[LANG_NAME_LEN];
				EnumResourceLanguages(hModule, lpType, lpName, (ENUMRESLANGPROC)ResLangProc, (LONG_PTR)szLangCode);
				StringCchPrintf(szInfoLine, INFO_LINE_LEN, TEXT("LANGUAGE %s"), szLangCode);
				SaveInfo(szFileName, szInfoLine);

				// Check for the font point size and 
				// typeface members.  These may or may not be there.	
				if (lpTemplate->style & DS_SETFONT)
				{
					WORD wPointSize = (*wPointer);
					wPointer += 1;
					// The font name
					std::wstring strTypeFace((LPCWSTR)wPointer);
					StringCchPrintf(szInfoLine, INFO_LINE_LEN, TEXT("FONT %i, \"%s\""), wPointSize, strTypeFace.c_str());
					SaveInfo(szFileName, szInfoLine);
					while ((*wPointer++) != 0x0000);
				}

				WORD wArrSize = ((BYTE*)wPointer - (BYTE*)wInitial);

				// Use this to keep track of each DLGITEMTEMPLATE size
				DWORD dwItemSize = 0;

				// How many controls are we dealing with? 
				UINT iNumControls = lpTemplate->cdit;

				dwDataPosition += wArrSize;

				// Now that we've passed all of the DLGTEMPLATE 
				// stuff, we need to make sure we're aligned on a DWORD boundary
				DWORD_ALIGN(dwDataPosition);

				// Spin through all of the controls
				for (int j = 0; j < iNumControls; j++)
				{
					// First point to the item's template
					lpItemTemplate = (LPDLGITEMTEMPLATE)(pbyGlobalData + dwDataPosition);

					// Point to the arrays after the DLGITEMTEMPLATE (2.)
					wPointer = (WORD *)(pbyGlobalData + dwDataPosition + dwDlgItemTemplateSize);
					wInitial = wPointer;
					std::wstring strClassName;
					std::wstring strTitle;
					WORD wTitleOrd = 0xffff;

					wSign = *wPointer;
					
					switch (wSign) {
					case 0xffff:
						// Treat the following 16-bit as a predefined control class. 
						wPointer += 1;
						wMenuOrd = *wPointer;
						wPointer += 1;
						
						switch (wMenuOrd) {
						case 0x0080:
							strClassName = TEXT("BUTTON");
							break;
						case 0x0081:
							strClassName = TEXT("EDIT");
							break;
						case 0x0082:
							strClassName = TEXT("STATIC");
							break;
						case 0x0083:
							strClassName = TEXT("LISTBOX");
							break;
						case 0x0084:
							strClassName = TEXT("SCROLLBAR");
							break;
						case 0x0085:
							strClassName = TEXT("COMBOBOX");
							break;
						}
						break;
					default:
						// Treat the following words as a Unicode null-terminated string 
						// representing a class name.
						strClassName = std::wstring((LPCWSTR)wPointer);
						while ((*wPointer++) != 0x0000);
						break;
					}

					wSign = *wPointer;

					switch (wSign) {
					case 0xffff:
						// Treat the next 16-bit value as an ordinal number representing Resource ID.
						wPointer += 1;
						wTitleOrd = *wPointer;
						wPointer += 1;
						break;
					default:
						// Treat the following words as a Unicode null-terminated string 
						// representing a control title.
						strTitle = std::wstring((LPCWSTR)wPointer);
						while ((*wPointer++) != 0x0000);
						break;
					}

					WORD wCADataSize = 0;

					wSign = *wPointer;

					if (wSign != 0x0000) {
						wCADataSize = (*wPointer);
						wPointer += (wCADataSize / 2) - 1;
					}
					else {
						wPointer += 1;
					}

					strStyles.clear();
					ParseControlStyle(lpItemTemplate->style, wMenuOrd, strStyles);
					ParseWindowStyle(lpItemTemplate->style, strStyles);

					// Composing a string to save.
					if (wTitleOrd == 0xffff) {
						StringCchPrintf(szInfoLine, INFO_LINE_LEN, TEXT("CONTROL %i, \"%s\", %s, %s, %i, %i, %i, %i"), lpItemTemplate->id, strTitle.c_str(), strClassName.c_str(), strStyles.c_str(), lpItemTemplate->x, lpItemTemplate->y, lpItemTemplate->cx, lpItemTemplate->cy);
					}
					else {
						StringCchPrintf(szInfoLine, INFO_LINE_LEN, TEXT("CONTROL %i, %i, %s, %s, %i, %i, %i, %i"), lpItemTemplate->id, wTitleOrd, strClassName.c_str(), strStyles.c_str(), lpItemTemplate->x, lpItemTemplate->y, lpItemTemplate->cx, lpItemTemplate->cy);
					}
					SaveInfo(szFileName, szInfoLine);

					// Calculating the address of the next item.
					wArrSize = ((BYTE*)wPointer - (BYTE*)wInitial) + dwDlgItemTemplateSize;
					DWORD_ALIGN(wArrSize);
					dwDataPosition = dwDataPosition + wArrSize;
					DWORD_ALIGN(dwDataPosition);
				}

				// Cleanup
				if (lpResource != NULL && hgResource != NULL)
				{
					UnlockResource(hgResource);
					FreeResource(hgResource);
				}
				SaveInfo(szFileName, TEXT("\r\n\r\n"));
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
		else {
			DWORD dwErrCode = GetLastError();
			std::cout << "Failed to load a resource. Error code: " << dwErrCode << std::endl;
			return FALSE;
		}
	}
	else {
		DWORD dwErrCode = GetLastError();
		std::cout << "Failed to find a resource. Error code: " << dwErrCode << std::endl;
		return FALSE;
	}
}

BOOL SaveInfo(LPCTSTR szFilePath, LPCTSTR szValue) {
	BOOL res = FALSE;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	size_t stringLenChars = 0;
	DWORD dwBytesToWrite = 0;
	DWORD dwBytesWritten = 0;
	DWORD dwErrCode;
	BOOL fOK = FALSE;
	LPSTR szLineToWrite;

	if (szValue != NULL) {
		StringCchLength(szValue, STRSAFE_MAX_CCH, &stringLenChars);
	}
	else {
		return FALSE;
	}

	hFile = CreateFile(szFilePath, FILE_GENERIC_READ | FILE_GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		dwErrCode = GetLastError();
		std::wcout << TEXT("Failed to open file: ") << dwErrCode << std::endl;
		return res;
	}

	// Converting a source Unicode string to UTF-8.
	int utfCharsToWrite = WideCharToMultiByte(CP_UTF8, 0, szValue, (int)stringLenChars, NULL, 0, NULL, NULL);
	szLineToWrite = new char[utfCharsToWrite + 3];
	utfCharsToWrite = WideCharToMultiByte(CP_UTF8, 0, szValue, (int)stringLenChars, szLineToWrite, utfCharsToWrite, NULL, NULL);
	szLineToWrite[utfCharsToWrite] = '\0';

	// Adding LF/CR at the end.
	dwBytesToWrite = utfCharsToWrite + 2;
	StringCchCatA(szLineToWrite, utfCharsToWrite + 3, "\r\n");

	SetFilePointer(hFile, 0, NULL, FILE_END);
	fOK = WriteFile(hFile, (LPCVOID)szLineToWrite, dwBytesToWrite, &dwBytesWritten, NULL);
	if (!fOK) {
		dwErrCode = GetLastError();
		std::wcout << TEXT("Failed to open file: ") << dwErrCode << std::endl;
		return res;
	}
	delete [] szLineToWrite;
	res = TRUE;
	CloseHandle(hFile);
	return res;
}

BOOL GetDirectoryPath(LPCTSTR szFilePath, LPTSTR szDirPath) {
	BOOL result = TRUE;
	size_t fullPathLen = 0;
	size_t iterator = 0;
	TCHAR szLibraryName[MAX_PATH] = { 0 };
	TCHAR szBaseDirPath[MAX_PATH] = { 0 };
	StringCchLength(szFilePath, STRSAFE_MAX_CCH, &fullPathLen);
	iterator = fullPathLen;
	const TCHAR *currPos = szFilePath + fullPathLen;
	while (*(currPos--) != TEXT('\\')) { --iterator; }
	StringCchCopy(szBaseDirPath, iterator + 2, szFilePath);
	StringCchCopy(szLibraryName, (fullPathLen - iterator) - 4, currPos + 2);
	StringCchPrintf(szDirPath, MAX_PATH, TEXT("%s%s_Resources\\"), szBaseDirPath, szLibraryName);
	CreateDirectory(szDirPath, NULL);
	return result;
}

BOOL ParseWindowStyle(DWORD style, std::wstring& str) {

	DWORD ws = style & 0xffff0000;
	DWORD mask = 0x00010000;

	for (int i = 0; i < 16; ++i) {
		if (ws & (mask << i)) {
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += windowStyles[i];
		}
	}

	return TRUE;
}

BOOL ParseDialogStyle(DWORD style, std::wstring& str) {
	DWORD ds = style & 0x0000ffff;
	DWORD mask = 0x00000001;

	for (int i = 0; i < 14; ++i) {
		if (ds & (mask << i)) {
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += dialogStyles[i];
		}
	}

	return TRUE;
}

BOOL ParseControlStyle(DWORD style, WORD controlType, std::wstring& str) {

	// For BUTTON
	if (controlType == 0x0080) {
		DWORD bs = (style & 0x0000000f);

		switch (bs) {
		case BS_PUSHBUTTON:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("BS_PUSHBUTTON");
			break;
		case BS_DEFPUSHBUTTON:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("BS_DEFPUSHBUTTON");
			break;
		case BS_CHECKBOX:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("BS_CHECKBOX");
			break;
		case BS_AUTOCHECKBOX:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("BS_AUTOCHECKBOX");
			break;
		case BS_RADIOBUTTON:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("BS_RADIOBUTTON");
			break;
		case BS_3STATE:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("BS_3STATE");
			break;
		case BS_AUTO3STATE:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("BS_AUTO3STATE");
			break;
		case BS_GROUPBOX:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("BS_GROUPBOX");
			break;
		case BS_USERBUTTON:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("BS_USERBUTTON");
			break;
		case BS_AUTORADIOBUTTON:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("BS_AUTORADIOBUTTON");
			break;
		case BS_PUSHBOX:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("BS_PUSHBOX");
			break;
		case BS_OWNERDRAW:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("BS_OWNERDRAW");
			break;
		case BS_TYPEMASK:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("BS_TYPEMASK");
			break;
		
		}
		bs = (style & 0x0000fff0);
		DWORD mask = 0x00000020;
		for (int i = 0; i < 11; ++i) {
			if (bs & (mask << i)) {
				if (str.size() != 0) {
					str += TEXT(" | ");
				}
				str += buttonStyles[i];
			}
		}
	}


	// For STATIC
	if (controlType == 0x0082) {
		DWORD ss = style & 0x000000ff;
		switch (ss) {
		case SS_LEFT:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_LEFT");
			break;
		case SS_CENTER:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_CENTER");
			break;
		case SS_RIGHT:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_RIGHT");
			break;
		case SS_ICON:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_ICON");
			break;
		case SS_BLACKRECT:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_BLACKRECT");
			break;
		case SS_GRAYRECT:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_GRAYRECT");
			break;
		case SS_WHITERECT:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_WHITERECT");
			break;
		case SS_BLACKFRAME:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_BLACKFRAME");
			break;
		case SS_GRAYFRAME:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_GRAYFRAME");
			break;
		case SS_WHITEFRAME:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_WHITEFRAME");
			break;
		case SS_USERITEM:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_USERITEM");
			break;
		case SS_SIMPLE:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_SIMPLE");
			break;
		case SS_LEFTNOWORDWRAP:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_LEFTNOWORDWRAP");
			break;
		case SS_OWNERDRAW:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_OWNERDRAW");
			break;
		case SS_BITMAP:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_BITMAP");
			break;
		case SS_ENHMETAFILE:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_ENHMETAFILE");
			break;
		case SS_ETCHEDHORZ:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_ETCHEDHORZ");
			break;
		case SS_ETCHEDVERT:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_ETCHEDVERT");
			break;
		case SS_ETCHEDFRAME:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_ETCHEDFRAME");
			break;
		case SS_TYPEMASK:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("SS_TYPEMASK");
			break;
		}

		ss = (style & 0x0000fff0);
		DWORD mask = 0x00000040;
		for (int i = 0; i < 10; ++i) {
			if (ss & (mask << i)) {
				if (str.size() != 0) {
					str += TEXT(" | ");
				}
				str += staticStyles[i];
			}
		}
	}

	// For LISTBOX 
	if (controlType == 0x0083) {

		DWORD ls = style & 0x0000ffff;
		DWORD mask = 0x00000001;
		for (int i = 0; i < 16; ++i) {
			if (ls & (mask << i)) {
				if (str.size() != 0) {
					str += TEXT(" | ");
				}
				str += listboxStyles[i];
			}
		}
	}

	// For COMBOBOX
	if (controlType == 0x0085) {
		DWORD cs = style & 0x0000000f;

		switch (cs) {
		case CBS_SIMPLE:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("CBS_SIMPLE");
			break;
		case CBS_DROPDOWN:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("CBS_DROPDOWN");
			break;
		case CBS_DROPDOWNLIST:
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += TEXT("CBS_DROPDOWNLIST");
			break;
		}

		cs = style & 0x0000fff0;
		DWORD mask = 0x00000010;

		for (int i = 0; i < 10; ++i) {
			if (cs & (mask << i)) {
				if (str.size() != 0) {
					str += TEXT(" | ");
				}
				str += comboStyles[i];
			}
		}
	}

	// For EDIT
	if (controlType == 0x0081) {
		
		DWORD es = style & 0x0000ffff;
		if (es == 0x00000000) {
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += editStyles[13];
		}
		DWORD mask = 0x00000001;
		for (int i = 0; i < 13; ++i) {
			if (es & (mask << i)) {
				if (str.size() != 0) {
					str += TEXT(" | ");
				}
				str += editStyles[i];
			}
		}
	}

	return TRUE;
}

BOOL ResLangProc(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName, WORD wLanguage, LONG_PTR lParam) {
	BOOL res = TRUE;
	std::wstring strLangCode;
	std::wostringstream ss;
	ss << wLanguage;
	StringCchCopy((LPTSTR)lParam, LANG_NAME_LEN, ss.str().c_str());
	// Converting Windows language code to Apple language name
	size_t num_of_langs = _countof(language_codes);
	for (int i = 0; i < num_of_langs; ++i)
	{
		if (CompareString(LOCALE_USER_DEFAULT, NORM_IGNORECASE, language_codes[i][0], 4, ss.str().c_str(), 4) == CSTR_EQUAL)
		{
			StringCchCopy((LPTSTR)lParam, LANG_NAME_LEN, language_codes[i][1]);
			break;
		}
	}
	return res;
}

BOOL GetMenuOption(WORD wOption, std::wstring& str) {
	WORD ms = wOption & 0xffff;
	DWORD mask = 0x0001;
	if (wOption == 0) {
		str = TEXT("MF_STRING");
		return FALSE;
	}
	for (int i = 0; i < 16; ++i) {
		if (ms & (mask << i)) {
			if (str.size() != 0) {
				str += TEXT(" | ");
			}
			str += menuStyles[i];
		}
	}

	return TRUE;
}
