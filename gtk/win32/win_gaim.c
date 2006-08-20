/*
 *  win_gaim.c
 *
 *  Date: June, 2002
 *  Description: Entry point for win32 gaim, and various win32 dependant
 *  routines.
 *
 * Gaim is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/* This is for ATTACH_PARENT_PROCESS */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x501
#endif
#include <windows.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define WIN32_PROXY_REGKEY "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"

typedef int (CALLBACK* LPFNGAIMMAIN)(HINSTANCE, int, char**);
typedef void (CALLBACK* LPFNSETDLLDIRECTORY)(LPCTSTR);
typedef BOOL (CALLBACK* LPFNATTACHCONSOLE)(DWORD);

/*
 *  PROTOTYPES
 */
static LPFNGAIMMAIN gaim_main = NULL;
static LPFNSETDLLDIRECTORY MySetDllDirectory = NULL;


static BOOL read_reg_string(HKEY key, char* sub_key, char* val_name, LPBYTE data, LPDWORD data_len) {
	HKEY hkey;
	BOOL ret = FALSE;
	LONG retv;

	if (ERROR_SUCCESS == (retv = RegOpenKeyEx(key, sub_key, 0,
					KEY_QUERY_VALUE, &hkey))) {
		if (ERROR_SUCCESS == (retv = RegQueryValueEx(hkey, val_name,
						NULL, NULL, data, data_len)))
			ret = TRUE;
		else {
			TCHAR szBuf[80];

			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
					NULL, retv, 0,
					(LPTSTR) &szBuf, sizeof(szBuf), NULL);

			printf("Could not read reg key '%s' subkey '%s' value: '%s'\nError: (%ld) %s\n",
					((key == HKEY_LOCAL_MACHINE) ? "HKLM" :
					 (key == HKEY_CURRENT_USER) ? "HKCU" :
					 "???"),
					sub_key, val_name, retv, szBuf);
		}
		RegCloseKey(hkey);
	}
	else {
		TCHAR szBuf[80];

		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, retv, 0,
				(LPTSTR) &szBuf, sizeof(szBuf), NULL);
		printf("Could not open reg subkey: %s\nError: (%ld) %s\n",
				sub_key, retv, szBuf);
	}

	return ret;
}

static void dll_prep() {
	char path[MAX_PATH + 1];
	HMODULE hmod;
	HKEY hkey;
#ifdef PORTABLE
	/* We assume that GTK+ is installed under \\path\to\Gaim\..\GTK
	 * First we find \\path\to
	 */
	if (GetModuleFileName(NULL, path, MAX_PATH) != 0) {
		char *tmp = path;
		char *prev = NULL;
		char *prev2 = NULL;

		while ((tmp = strchr(tmp, '\\'))) {
			prev2 = prev;
			prev = tmp;
			tmp++;
		}

		if (prev2) {
			prev2[0] = '\0';
		}
	} else {
		printf("Unable to determine current executable path. \n"
			"This will prevent the settings dir from being set.\n"
			"Assuming GTK+ is in the PATH.\n");
	}

	if (path) {
		/* Set up the settings dir base to be \\path\to
		 * The actual settings dir will be \\path\to\.gaim */
		char settingsdir[strlen(path) + strlen("GAIMHOME=") + 1];
		char aspelldir[strlen(path) + strlen("GAIM_ASPELL_DIR=\\Aspell\\bin") + 1];

		snprintf(settingsdir, sizeof(settingsdir), "GAIMHOME=%s", path);
		printf("Setting settings dir: %s\n", settingsdir);
		putenv(settingsdir);

		snprintf(aspelldir, sizeof(aspelldir), "GAIM_ASPELL_DIR=%s\\Aspell\\bin", path);
		printf(aspelldir);
		putenv(aspelldir);

		/* set the GTK+ path to be \\path\to\GTK\bin */
		strcat(path, "\\GTK\\bin");
	} else
		return;
#else /* PORTABLE */
	char gtkpath[MAX_PATH + 1];
	DWORD plen;

	plen = sizeof(gtkpath);
	hkey = HKEY_CURRENT_USER;
	if (!read_reg_string(hkey, "SOFTWARE\\GTK\\2.0", "Path",
			(LPBYTE) &gtkpath, &plen)) {
		hkey = HKEY_LOCAL_MACHINE;
		if (!read_reg_string(hkey, "SOFTWARE\\GTK\\2.0", "Path",
				(LPBYTE) &gtkpath, &plen)) {
			printf("GTK+ Path Registry Key not found. "
				"Assuming GTK+ is in the PATH.\n");
			return;
		}
	}

	/* this value is replaced during a successful RegQueryValueEx() */
	plen = sizeof(path);
	/* Determine GTK+ dll path .. */
	if (!read_reg_string(hkey, "SOFTWARE\\GTK\\2.0", "DllPath",
				(LPBYTE) &path, &plen)) {
		strcpy(path, gtkpath);
		strcat(path, "\\bin");
	}
#endif
	printf("GTK+ path found: %s\n", path);

	if ((hmod = GetModuleHandle("kernel32.dll"))) {
		MySetDllDirectory = (LPFNSETDLLDIRECTORY) GetProcAddress(
			hmod, "SetDllDirectoryA");
		if (!MySetDllDirectory)
			printf("SetDllDirectory not supported\n");
	} else
		printf("Error getting kernel32.dll module handle\n");

	/* For Windows XP SP1+ / Server 2003 we use SetDllDirectory to avoid dll hell */
	if (MySetDllDirectory) {
		printf("Using SetDllDirectory\n");
		MySetDllDirectory(path);
	}

	/* For the rest, we set the current directory and make sure
	 * SafeDllSearch is set to 0 where needed. */
	else {
		OSVERSIONINFO osinfo;

		printf("Setting current directory to GTK+ dll directory\n");
		SetCurrentDirectory(path);
		/* For Windows 2000 (SP3+) / WinXP (No SP):
		 * If SafeDllSearchMode is set to 1, Windows system directories are
		 * searched for dlls before the current directory. Therefore we set it
		 * to 0.
		 */
		osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osinfo);
		if ((osinfo.dwMajorVersion == 5 &&
			osinfo.dwMinorVersion == 0 &&
			strcmp(osinfo.szCSDVersion, "Service Pack 3") >= 0) ||
			(osinfo.dwMajorVersion == 5 &&
			osinfo.dwMinorVersion == 1 &&
			strcmp(osinfo.szCSDVersion, "") >= 0)
		) {
			DWORD regval = 1;
			DWORD reglen = sizeof(DWORD);

			printf("Using Win2k (SP3+) / WinXP (No SP)... Checking SafeDllSearch\n");
			read_reg_string(HKEY_LOCAL_MACHINE,
				"System\\CurrentControlSet\\Control\\Session Manager",
				"SafeDllSearchMode",
				(LPBYTE) &regval,
				&reglen);

			if (regval != 0) {
				printf("Trying to set SafeDllSearchMode to 0\n");
				regval = 0;
				if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					"System\\CurrentControlSet\\Control\\Session Manager", 
					0,  KEY_SET_VALUE, &hkey
				) == ERROR_SUCCESS) {
					if (RegSetValueEx(hkey,
						"SafeDllSearchMode", 0,
						REG_DWORD, (LPBYTE) &regval,
						sizeof(DWORD)
					) != ERROR_SUCCESS)
						printf("Error writing SafeDllSearchMode. Error: %u\n",
						(UINT) GetLastError());
					RegCloseKey(hkey);
				} else
					printf("Error opening Session Manager key for writing. Error: %u\n",
						(UINT) GetLastError());
			} else
				printf("SafeDllSearchMode is set to 0\n");
		}/*end else*/
	}
}

static char* wgaim_lcid_to_posix(LCID lcid) {
	char *posix = NULL;
	int lang_id = PRIMARYLANGID(lcid);
	int sub_id = SUBLANGID(lcid);

	switch (lang_id) {
		case LANG_ARABIC: break;
		case LANG_BULGARIAN: posix = "bg"; break;
		case LANG_CATALAN: posix = "ca"; break;
		case LANG_CHINESE:
			switch (sub_id) {
				case SUBLANG_CHINESE_SIMPLIFIED:
					posix = "zh_CN"; break;
				case SUBLANG_CHINESE_TRADITIONAL:
					posix = "zh_TW"; break;
				default:
					posix = "zh"; break;
			}
			break;
		case LANG_CZECH: posix = "cs"; break;
		case LANG_DANISH: posix = "da"; break;
		case LANG_GERMAN: posix = "de"; break;
		case LANG_GREEK: posix = "el"; break;
		case LANG_ENGLISH:
			switch (sub_id) {
				case SUBLANG_ENGLISH_UK:
					posix = "en_GB"; break;
				case SUBLANG_ENGLISH_AUS:
					posix = "en_AU"; break;
				case SUBLANG_ENGLISH_CAN:
					posix = "en_CA"; break;
				default:
					posix = "en"; break;
			}
			break;
		case LANG_SPANISH: posix = "es"; break;
		case LANG_FINNISH: posix = "fi"; break;
		case LANG_FRENCH: posix = "fr"; break;
		case LANG_HEBREW: posix = "he"; break;
		case LANG_HUNGARIAN: posix = "hu"; break;
		case LANG_ICELANDIC: break;
		case LANG_ITALIAN: posix = "it"; break;
		case LANG_JAPANESE: posix = "ja"; break;
		case LANG_KOREAN: posix = "ko"; break;
		case LANG_DUTCH: posix = "nl"; break;
		case LANG_NORWEGIAN:
			switch (sub_id) {
				case SUBLANG_NORWEGIAN_BOKMAL:
					posix = "nb"; break;
				case SUBLANG_NORWEGIAN_NYNORSK:
					posix = "nn"; break;
			}
			break;
		case LANG_POLISH: posix = "pl"; break;
		case LANG_PORTUGUESE:
			switch (sub_id) {
				case SUBLANG_PORTUGUESE_BRAZILIAN:
					posix = "pt_BR"; break;
				default:
				posix = "pt"; break;
			}
			break;
		case LANG_ROMANIAN: posix = "ro"; break;
		case LANG_RUSSIAN: posix = "ru"; break;
		/* LANG_CROATIAN == LANG_SERBIAN for some reason
		 * We'll need to do something here if we ever get a croatian translation */
		/* case LANG_CROATIAN: posix = "hr"; break;*/
		case LANG_SERBIAN:
			switch (sub_id) {
				case SUBLANG_SERBIAN_LATIN:
					posix = "sr@Latn"; break;
				case SUBLANG_SERBIAN_CYRILLIC:
					posix = "sr"; break;
			}
			break;
		case LANG_SLOVAK: posix = "sk"; break;
		case LANG_ALBANIAN: posix = "sq"; break;
		case LANG_SWEDISH: posix = "sv"; break;
		case LANG_THAI: posix = "th"; break;
		case LANG_TURKISH: posix = "tr"; break;
		case LANG_URDU: break;
		case LANG_INDONESIAN: break;
		case LANG_UKRAINIAN: posix = "uk"; break;
		case LANG_BELARUSIAN: break;
		case LANG_SLOVENIAN: posix = "sl"; break;
		case LANG_ESTONIAN: posix = "et"; break;
		case LANG_LATVIAN: break;
		case LANG_LITHUANIAN: posix = "lt"; break;
		case LANG_FARSI: break;
		case LANG_VIETNAMESE: posix = "vi"; break;
		case LANG_ARMENIAN: break;
		case LANG_AZERI: posix = "az"; break;
		case LANG_BASQUE: break;
		case LANG_MACEDONIAN: posix = "mk"; break;
		case LANG_AFRIKAANS: break;
		case LANG_GEORGIAN: posix = "ka"; break;
		case LANG_FAEROESE: break;
		case LANG_HINDI: posix = "hi"; break;
		case LANG_MALAY: break;
		case LANG_KAZAK: break;
		case LANG_KYRGYZ: break;
		case LANG_SWAHILI: break;
		case LANG_UZBEK: break;
		case LANG_TATAR: break;
		case LANG_BENGALI: break;
		case LANG_PUNJABI: posix = "pa"; break;
		case LANG_GUJARATI: posix = "gu"; break;
		case LANG_ORIYA: break;
		case LANG_TAMIL: posix = "ta"; break;
		case LANG_TELUGU: break;
		case LANG_KANNADA: break;
		case LANG_MALAYALAM: break;
		case LANG_ASSAMESE: break;
		case LANG_MARATHI: break;
		case LANG_SANSKRIT: break;
		case LANG_MONGOLIAN: break;
		case LANG_GALICIAN: posix = "gl"; break;
		case LANG_KONKANI: break;
		case LANG_MANIPURI: break;
		case LANG_SINDHI: break;
		case LANG_SYRIAC: break;
		case LANG_KASHMIRI: break;
		case LANG_NEPALI: break;
		case LANG_DIVEHI: break;
	}

	/* Deal with exceptions */
	if (posix == NULL) {
		switch (lcid) {
			case 2125: posix = "my_MM"; break; /* Myanmar (Burmese) */
			case 1076: posix = "xh"; break; /* Xhosa */
			case 9999: posix = "ku"; break; /* Kurdish (from NSIS) */
		}
	}

	return posix;
}

/* Determine and set Gaim locale as follows (in order of priority):
   - Check GAIMLANG env var
   - Check NSIS Installer Language reg value
   - Use default user locale
*/
static const char *wgaim_get_locale() {
	const char *locale = NULL;
	LCID lcid;
#ifndef PORTABLE
	char data[10];
	DWORD datalen = 10;
#endif

	/* Check if user set GAIMLANG env var */
	if ((locale = getenv("GAIMLANG")))
		return locale;

#ifndef PORTABLE
	if (read_reg_string(HKEY_CURRENT_USER, "SOFTWARE\\gaim",
			"Installer Language", (LPBYTE) &data, &datalen)) {
		if ((locale = wgaim_lcid_to_posix(atoi(data))))
			return locale;
	}
#endif

	lcid = GetUserDefaultLCID();
	if ((locale = wgaim_lcid_to_posix(lcid)))
		return locale;

	return "en";
}

static void wgaim_set_locale() {
	const char *locale = NULL;
	char envstr[25];

	locale = wgaim_get_locale();

	snprintf(envstr, 25, "LANG=%s", locale);
	printf("Setting locale: %s\n", envstr);
	putenv(envstr);
}

static BOOL wgaim_set_running() {
	HANDLE h;

	if ((h = CreateMutex(NULL, FALSE, "gaim_is_running"))) {
		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			MessageBox(NULL,
				"An instance of Gaim is already running",
				NULL, MB_OK | MB_TOPMOST);
			return FALSE;
		}
	}
	return TRUE;
}

static void wgaim_set_proxy() {
	DWORD regval = 1;
	DWORD reglen = sizeof(DWORD);

	/* If the proxy server environment variables are already set,
	 * we shouldn't override them */
	if (getenv("HTTP_PROXY") || getenv("http_proxy") || getenv("HTTPPROXY"))
		return;

	if (read_reg_string(HKEY_CURRENT_USER, WIN32_PROXY_REGKEY,
				"ProxyEnable",
				(LPBYTE) &regval, &reglen) && (regval & 1)) {
		char proxy_server[2048];
		char *c = NULL;
		reglen = sizeof(proxy_server);

		if (!read_reg_string(HKEY_CURRENT_USER, WIN32_PROXY_REGKEY,
				"ProxyServer", (LPBYTE) &proxy_server, &reglen))
			return;

		if ((reglen > strlen("http="))
				&& (c = strstr(proxy_server, "http="))) {
			char *d;
			c += strlen("http=");
			d = strchr(c, ';');
			if (d) {
				*d = '\0';
			}
			/* c now points the proxy server (and port) */
		}

		if (c) {
			const char envstr_prefix[] = "HTTP_PROXY=http://";
			char envstr[sizeof(envstr_prefix) + strlen(c) + 1];
			snprintf(envstr, sizeof(envstr), "%s%s",
				envstr_prefix, c);
			printf("Setting HTTP Proxy: %s\n", envstr);
			putenv(envstr);
		}
	}

}

#ifdef __GNUC__
#  ifndef _stdcall
#    define _stdcall  __attribute__((stdcall))
#  endif
#endif

int _stdcall
WinMain (struct HINSTANCE__ *hInstance, struct HINSTANCE__ *hPrevInstance,
		char *lpszCmdLine, int nCmdShow) {
	char errbuf[512];
	char gaimdir[MAX_PATH];
	HMODULE hmod;

	/* If debug or help or version flag used, create console for output */
	if (strstr(lpszCmdLine, "-d") || strstr(lpszCmdLine, "-h") || strstr(lpszCmdLine, "-v")) {
		LPFNATTACHCONSOLE MyAttachConsole = NULL;
		if ((hmod = GetModuleHandle("kernel32.dll"))) {
			MyAttachConsole =
				(LPFNATTACHCONSOLE)
				GetProcAddress(hmod, "AttachConsole");
		}
		if ((MyAttachConsole && MyAttachConsole(ATTACH_PARENT_PROCESS))
				|| AllocConsole())
			freopen("CONOUT$", "w", stdout);
	}

	/* Load exception handler if we have it */
	if (GetModuleFileName(NULL, gaimdir, MAX_PATH) != 0) {
		char *tmp = gaimdir;
		char *prev = NULL;

		while ((tmp = strchr(tmp, '\\'))) {
			prev = tmp;
			tmp++;
		}

		if (prev) {
			prev[0] = '\0';
			strcat(gaimdir, "\\exchndl.dll");
			if (LoadLibrary(gaimdir))
				printf("Loaded exchndl.dll\n");
		}
	} else {
		snprintf(errbuf, 512,
			"Error getting module filename. Error: %u",
			(UINT) GetLastError());
		MessageBox(NULL, errbuf, NULL, MB_OK | MB_TOPMOST);
	}

#ifndef PORTABLE
	if (!getenv("GAIM_NO_DLL_CHECK"))
#endif
		dll_prep();

	wgaim_set_locale();
	/* If help or version flag used, do not check Mutex */
	if (!strstr(lpszCmdLine, "-h") && !strstr(lpszCmdLine, "-v"))
		if (!getenv("GAIM_MULTI_INST") && !wgaim_set_running())
			return 0;

	wgaim_set_proxy();

	/* Now we are ready for Gaim .. */
	if ((hmod = LoadLibrary("gtkgaim.dll"))) {
		gaim_main = (LPFNGAIMMAIN) GetProcAddress(hmod, "gaim_main");
	}

	if (!gaim_main) {
		snprintf(errbuf, 512, "Error loading gaim.dll. Error: %u",
			(UINT) GetLastError());
		MessageBox(NULL, errbuf, NULL, MB_OK | MB_TOPMOST);
		return 0;
	}

	return gaim_main (hInstance, __argc, __argv);
}
