;;
;;  dutch.nsh
;;
;;  Default language strings for the Windows Gaim NSIS installer.
;;  Windows Code page: 1252
;;
;;  Author: Vincent van Adrighem <vincent@dirck.mine.nu>
;;  Version 2
;;

; Startup GTK+ check
!define GTK_INSTALLER_NEEDED			"De GTK+ runtime-omgeving is niet aanwezig of moet vernieuwd worden.$\rInstalleer v${GTK_VERSION} of nieuwer van de GTK+ runtime-omgeving"

; License Page
!define GAIM_LICENSE_BUTTON			"Volgende >"
!define GAIM_LICENSE_BOTTOM_TEXT		"$(^Name) wordt uitgegeven onder de GPL licentie. Deze licentie wordt hier slechts ter informatie aangeboden. $_CLICK"

; Components Page
!define GAIM_SECTION_TITLE			"Gaim Instant Messaging Client (vereist)"
!define GTK_SECTION_TITLE			"GTK+ runtime-omgeving (vereist)"
!define GTK_THEMES_SECTION_TITLE		"GTK+ thema's"
!define GTK_NOTHEME_SECTION_TITLE		"Geen thema"
!define GTK_WIMP_SECTION_TITLE		"Wimp thema"
!define GTK_BLUECURVE_SECTION_TITLE		"Bluecurve thema"
!define GTK_LIGHTHOUSEBLUE_SECTION_TITLE	"Light House Blue thema"
!define GAIM_SECTION_DESCRIPTION		"Gaim hoofdbestanden en dlls"
!define GTK_SECTION_DESCRIPTION		"Een multi-platform gebruikersinterface, gebruikt door Gaim"
!define GTK_THEMES_SECTION_DESCRIPTION	"GTK+ thema's veranderen het uiterlijk en gedrag van GTK+ programma's."
!define GTK_NO_THEME_DESC			"Geen GTK+ thema installeren"
!define GTK_WIMP_THEME_DESC			"GTK-Wimp (Windows impersonator) is een GTK+ thema wat goed past in een windows omgeving."
!define GTK_BLUECURVE_THEME_DESC		"Het Bluecurve thema (standaardthema voor RedHat Linux)."
!define GTK_LIGHTHOUSEBLUE_THEME_DESC	"Het Lighthouseblue thema."

; GTK+ Directory Page
!define GTK_UPGRADE_PROMPT			"Er is een oude versie van GTK+ gevonden. Wilt u deze bijwerken?$\rLet op: Gaim werkt misschien niet als u dit niet doet."

; Installer Finish Page
!define GAIM_FINISH_VISIT_WEB_SITE		"Neem een kijkje op de Windows Gaim webpagina"

; Gaim Section Prompts and Texts
!define GAIM_UNINSTALL_DESC			"Gaim (alleen verwijderen)"
!define GAIM_PROMPT_WIPEOUT			"Uw oude Gaim map staat op het punt om verwijderd te worden. Wilt u doorgaan?$\r$\rLet op: Alle door uzelf ge�nstalleerde plugins zullen ook verwijderd worden.$\rDe gebruikersinstellingen van Gaim worden niet aangeraakt."
!define GAIM_PROMPT_DIR_EXISTS		"De gegeven installatiemap bestaat al. Eventuele inhoud zal verwijderd worden. Wilt u doorgaan?"

; GTK+ Section Prompts
!define GTK_INSTALL_ERROR			"Fout bij installatie van GTK+ runtime omgeving."
!define GTK_BAD_INSTALL_PATH			"Het door u gegeven pad kan niet benaderd worden."

; GTK+ Themes section
!define GTK_NO_THEME_INSTALL_RIGHTS		"U heeft geen toestemming om een GTK+ thema te installeren."

; Uninstall Section Prompts
!define un.GAIM_UNINSTALL_ERROR_1         "De uninstaller kon geen register-ingangen voor Gaim vinden.$\rWaarschijnlijk heeft een andere gebruiker het programma ge�nstalleerd."
!define un.GAIM_UNINSTALL_ERROR_2         "U mag dit programma niet verwijderen."
