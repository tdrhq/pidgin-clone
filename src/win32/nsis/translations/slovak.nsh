;;
;;  slovak.nsh
;;
;;  Slovak language strings for the Windows Gaim NSIS installer.
;;  Windows Code page: 1250
;;
;;  Author: dominik@internetkosice.sk
;;  Version 2

; Startup GTK+ check
!define GTK_INSTALLER_NEEDED			"GTK+ runtime prostredie ch�ba alebo mus� by� upgradovan�.$\rNain�talujte, pros�m, GTK+ runtime verziu v${GTK_VERSION}, alebo nov�iu"

; License Page
!define GAIM_LICENSE_BUTTON			"�alej >"
!define GAIM_LICENSE_BOTTOM_TEXT		"$(^Name) je vydan� pod GPL licenciou. T�to licencia je len pre informa�n� ��ely. $_CLICK"

; Components Page
!define GAIM_SECTION_TITLE			"Gaim Instant Messaging Klient (nevyhnutn�)"
!define GTK_SECTION_TITLE			"GTK+ Runtime prostredie (nevyhnutn�)"
!define GTK_THEMES_SECTION_TITLE		"GTK+ t�my"
!define GTK_NOTHEME_SECTION_TITLE		"�iadna grafick� t�ma"
!define GTK_WIMP_SECTION_TITLE		"Wimp grafick� t�ma"
!define GTK_BLUECURVE_SECTION_TITLE		"Bluecurve grafick� t�ma"
!define GTK_LIGHTHOUSEBLUE_SECTION_TITLE	"Light House Blue grafick� t�ma"
!define GAIM_SECTION_DESCRIPTION		"Jadro Gaim-u a nevyhnutn� DLL s�bory"
!define GTK_SECTION_DESCRIPTION		"Multiplatformov� GUI n�stroje, pou��van� Gaim-om"
!define GTK_THEMES_SECTION_DESCRIPTION	"Pomocou GTK+ grafick�ch t�m m��ete zmeni� vzh�ad GTK+ aplik�ci�."
!define GTK_NO_THEME_DESC			"Nein�talova� GTK+ grafick� t�mu"
!define GTK_WIMP_THEME_DESC			"GTK-Wimp (Windows impersonator) je GTK grafick� t�ma, ktor� pekne lad� s prostred�m Windows."
!define GTK_BLUECURVE_THEME_DESC		"Bluecurve grafick� t�ma."
!define GTK_LIGHTHOUSEBLUE_THEME_DESC	"Lighthouseblue grafick� t�ma"

; GTK+ Directory Page
!define GTK_UPGRADE_PROMPT			"Bola n�jden� star�ia verzia GTK+ runtime. Prajete si upgradova� s��asn� verziu?$\rPozn�mka: Gaim nemus� po upgradovan� fungova� spr�vne."

; Installer Finish Page
!define GAIM_FINISH_VISIT_WEB_SITE		"Nav�t�vi� webstr�nku Windows Gaim"

; Gaim Section Prompts and Texts
!define GAIM_UNINSTALL_DESC			"Gaim (len odstr�ni�)"
!define GAIM_PROMPT_WIPEOUT			"V� adres�r Gaim bude zmazan�. Chcete pokra�ova�?$\r$\rPozn�mka: V�etky pr�davne pluginy, ktor� ste nain�talovali bud� tie� zmazan�.$\rNastavenia u�ivate�sk�ho ��tu Gaim-u bud� ponechan�."
!define GAIM_PROMPT_DIR_EXISTS		"Adres�r, ktor� ste zadali, u� existuje. Jeho obsah bude zmazan�. Chcete pokra�ova�?"

; GTK+ Section Prompts
!define GTK_INSTALL_ERROR			"Chyba pri in�tal�cii GTK+ runtime."
!define GTK_BAD_INSTALL_PATH			"Zadan� cesta nie je pr�stupn� alebo ju nie je mo�n� vytvori�."

; GTK+ Themes section
!define GTK_NO_THEME_INSTALL_RIGHTS		"Nem�te opr�vnenie na in�tal�ciu GTK+ grafickej t�my."

; Uninstall Section Prompts
!define un.GAIM_UNINSTALL_ERROR_1		"In�tal�toru sa nepodarilo n�js� polo�ky v registri pre Gaim.$\rJe mo�n�, �e t�to aplik�ciu nain�taloval in� pou��vate�."
!define un.GAIM_UNINSTALL_ERROR_2		"Nem�te opr�vnenie na odin�tal�ciu tejto aplik�cie."
