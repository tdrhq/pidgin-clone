;;
;;  swedish.nsh
;;
;;  Swedish language strings for the Windows Gaim NSIS installer.
;;  Windows Code page: 1252
;;
;;  Author: Tore Lundqvist <tlt@mima.x.se>, 2003.
;;

; Startup GTK+ check
!define GTK_INSTALLER_NEEDED			"$\"GTK+ runtime environment$\" saknas eller beh�ver uppgraderas$\rInstallera v${GTK_VERSION} eller h�gre av GTK+."

; Components Page
!define GAIM_SECTION_TITLE			"Gaim-snabbmeddelandeklient (obligatorisk)"
!define GTK_SECTION_TITLE			"GTK+ Runtime Environment (obligatorisk)"
!define GTK_THEMES_SECTION_TITLE		"GTK+teman"
!define GTK_NOTHEME_SECTION_TITLE		"Inget tema"
!define GTK_WIMP_SECTION_TITLE		"Wimp-tema"
!define GTK_BLUECURVE_SECTION_TITLE		"Bluecurve-tema"
!define GTK_LIGHTHOUSEBLUE_SECTION_TITLE	"Light House Blue-tema"
!define GAIM_SECTION_DESCRIPTION		"Gaims k�nrfiler och DLL:er"
!define GTK_SECTION_DESCRIPTION		"En GUI-verktygsupps�ttning f�r flera olika plattformar som Gaim beh�ver."
!define GTK_THEMES_SECTION_DESCRIPTION	"GTK+teman kan �ndra k�nslan av och utseendet p� GTK+applikationer."
!define GTK_NO_THEME_DESC			"Installera inte n�got GTK+tema"
!define GTK_WIMP_THEME_DESC			"GTK-Wimp (Windows impersonator) ett GTK-tema som sm�lter bra in i Windows-milj�n."
!define GTK_BLUECURVE_THEME_DESC		"The Bluecurve-tema."
!define GTK_LIGHTHOUSEBLUE_THEME_DESC	"The Lighthouseblue-tema."

; GTK+ Directory Page
!define GTK_UPGRADE_PROMPT			"En �ldre version av GTK+ runtime hittades, vill du uppgradera den?$\rOBS! Gaim kommer kanske inte att fungera om du inte uppgraderar."

; Gaim Section Prompts and Texts
!define GAIM_UNINSTALL_DESC			"Gaim (enbart f�r avinstallation)"
!define GAIM_PROMPT_WIPEOUT			"Din gamla Gaim-katalog kommer att raderas, vill du forts�tta?$\r$\rOBS! om du har installerat n�gra extra insticksmoduler kommer de raderas.$\rGaims anv�ndarinst�llningar kommer inte p�verkas."
!define GAIM_PROMPT_DIR_EXISTS		"Den katalog du vill installera i finns redan. Allt i katalogen$\rkommer att raderas, vill du forts�tta?"

; GTK+ Section Prompts
!define GTK_INSTALL_ERROR			"Fel vid installation av GTK+ runtime."
!define GTK_BAD_INSTALL_PATH			"Den s�kv�g du angivit g�r inte att komma �t eller skapa."

; GTK+ Themes section
!define GTK_NO_THEME_INSTALL_RIGHTS		"Du har inte r�ttigheter att installera ett GTK+tema."

; Uninstall Section Prompts
!define un.GAIM_UNINSTALL_ERROR_1         "Avinstalleraren kunde inte hitta registerv�rden f�r Gaim.$\rAntagligen har en annan anv�ndare installerat applikationen."
!define un.GAIM_UNINSTALL_ERROR_2         "Du har inte r�ttigheter att avinstallera den h�r applikationen."
