;;
;;  swedish.nsh
;;
;;  Swedish language strings for the Windows Pidgin NSIS installer.
;;  Windows Code page: 1252
;;
;;  Author: Tore Lundqvist <tlt@mima.x.se>, 2003.
;;  Author: Peter Hjalmarsson <xake@telia.com>, 2005.
;;  Version 3

; Make sure to update the PIDGIN_MACRO_LANGUAGEFILE_END macro in
; langmacros.nsh when updating this file

; Startup Checks
!define INSTALLER_IS_RUNNING			"Installationsprogrammet k�rs redan."
!define PIDGIN_IS_RUNNING			"En instans av Pidgin k�rs redan. Avsluta Pidgin och f�rs�k igen."
!define GTK_INSTALLER_NEEDED			"K�rmilj�n GTK+ �r antingen inte installerat eller beh�ver uppgraderas.$\rVar god installera v${GTK_MIN_VERSION} eller h�gre av GTK+-k�rmilj�n."

; License Page
!define PIDGIN_LICENSE_BUTTON			"N�sta >"
!define PIDGIN_LICENSE_BOTTOM_TEXT		"$(^Name) �r utgivet under GPL. Licensen finns tillg�nglig h�r f�r infromationssyften enbart. $_CLICK"

; Components Page
!define PIDGIN_SECTION_TITLE			"Pidgin Snabbmeddelandeklient (obligatorisk)"
!define GTK_SECTION_TITLE			"GTK+-k�rmilj� (obligatorisk)"
!define PIDGIN_SHORTCUTS_SECTION_TITLE "Genv�gar"
!define PIDGIN_DESKTOP_SHORTCUT_SECTION_TITLE "Skrivbord"
!define PIDGIN_STARTMENU_SHORTCUT_SECTION_TITLE "Startmeny"
!define PIDGIN_SECTION_DESCRIPTION		"Pidgin k�rnfiler och DLL:er"
!define GTK_SECTION_DESCRIPTION		"En GUI-verktygsupps�ttning f�r flera olika plattformar som Pidgin anv�nder."

!define PIDGIN_SHORTCUTS_SECTION_DESCRIPTION   "Genv�gar f�r att starta Pidgin"
!define PIDGIN_DESKTOP_SHORTCUT_DESC   "Skapar en genv�g till Pidgin p� skrivbordet"
!define PIDGIN_STARTMENU_SHORTCUT_DESC   "Skapar ett till�gg i startmenyn f�r Pidgin"

; GTK+ Directory Page
!define GTK_UPGRADE_PROMPT			"En �ldre version av GTK+ runtime hittades, vill du uppgradera den?$\rOBS! $(^Name) kommer kanske inte att fungera om du inte uppgraderar."

; Installer Finish Page
!define PIDGIN_FINISH_VISIT_WEB_SITE		"Bes�k Windows-Pidgin hemsida"

; Pidgin Section Prompts and Texts
!define PIDGIN_UNINSTALL_DESC			"$(^Name) (enbart f�r avinstallation)"

; GTK+ Section Prompts
!define GTK_INSTALL_ERROR			"Fel vid installation av GTK+ runtime."
!define GTK_BAD_INSTALL_PATH			"Den s�kv�g du angivit g�r inte att komma �t eller skapa."

; Uninstall Section Prompts
!define un.PIDGIN_UNINSTALL_ERROR_1         "Avinstalleraren kunde inte hitta registerv�rden f�r Pidgin.$\rAntagligen har en annan anv�ndare installerat applikationen."
!define un.PIDGIN_UNINSTALL_ERROR_2         "Du har inte r�ttigheter att avinstallera den h�r applikationen."