;;
;;  romanian.nsh
;;
;;  Romanian language strings for the Windows Gaim NSIS installer.
;;  Windows Code page: 1250
;;
;;  Author: Mi�u Moldovan <dumol@go.ro>
;;

; Startup GTK+ check
!define GTK_INSTALLER_NEEDED			"Mediul GTK+ nu e prezent sau ave�i o versiune prea veche.$\rInstala�i cel pu�in versiunea v${GTK_VERSION} a mediului GTK+"

; Components Page
!define GAIM_SECTION_TITLE			"Client de mesagerie instantanee (obligatoriu)"
!define GTK_SECTION_TITLE			"Mediu GTK+ (obligatoriu)"
!define GTK_THEMES_SECTION_TITLE		"Teme GTK+"
!define GTK_NOTHEME_SECTION_TITLE		"F�r� teme"
!define GTK_WIMP_SECTION_TITLE		"Tem� Wimp"
!define GTK_BLUECURVE_SECTION_TITLE		"Tem� Bluecurve"
!define GTK_LIGHTHOUSEBLUE_SECTION_TITLE	"Tem� Light House Blue"
!define GAIM_SECTION_DESCRIPTION		"Fi�iere Gaim �i dll-uri"
!define GTK_SECTION_DESCRIPTION		"Un mediu de dezvoltare multiplatform� utilizat de Gaim"
!define GTK_THEMES_SECTION_DESCRIPTION	"Temele GTK+ schimb� aparen�a aplica�iilor GTK+."
!define GTK_NO_THEME_DESC			"Nu instala o tem� GTK+"
!define GTK_WIMP_THEME_DESC			"GTK-Wimp este o tem� GTK+ �n acord cu mediul Windows."
!define GTK_BLUECURVE_THEME_DESC		"Tema Bluecurve."
!define GTK_LIGHTHOUSEBLUE_THEME_DESC	"Tema Lighthouseblue."

; GTK+ Directory Page
!define GTK_UPGRADE_PROMPT			"Ave�i o versiune veche a mediului GTK+. Dori�i s� o actualiza�i?$\rNot�: E posibil ca Gaim s� nu func�ioneze cu versiunea veche."

; Gaim Section Prompts and Texts
!define GAIM_UNINSTALL_DESC			"Gaim (doar dezinstalare)"
!define GAIM_PROMPT_WIPEOUT			"Vechiul director Gaim va fi �ters. Dori�i s� continua�i?$\r$\rNot�: Orice module externe vor fi �terse.$\rSet�rile utilizatorilor Gaim nu vor fi afectate."
!define GAIM_PROMPT_DIR_EXISTS		"Directorul ales pentru instalare exist� deja.$\rCon�inutul s�u va fi �ters. Dori�i s� continua�i?"

; GTK+ Section Prompts
!define GTK_INSTALL_ERROR			"Eroare la instalarea mediului GTK+."
!define GTK_BAD_INSTALL_PATH			"Directorul specificat nu poate fi accesat sau creat."

; GTK+ Themes section
!define GTK_NO_THEME_INSTALL_RIGHTS		"Nu ave�i drepturile de acces necesare instal�rii unei teme GTK+."

; Uninstall Section Prompts
!define un.GAIM_UNINSTALL_ERROR_1         "Programul de dezinstalare nu a g�sit intr�ri Gaim �n regi�tri.$\rProbabil un alt utilizator a instalat aceast� aplica�ie."
!define un.GAIM_UNINSTALL_ERROR_2         "Nu ave�i drepturile de acces necesare dezinstal�rii acestei aplica�ii."
