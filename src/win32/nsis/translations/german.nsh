;;
;;  german.nsh
;;
;;  German language strings for the Windows Gaim NSIS installer.
;;  Windows Code page: 1252
;;
;;  Author: Bjoern Voigt <bjoern@cs.tu-berlin.de>, 2003.
;;

; Startup GTK+ check
LangString GTK_INSTALLER_NEEDED			${LANG_GERMAN} "Die GTK+ Runtime Umgebung ist entweder nicht vorhanden oder sollte aktualisiert werden.$\rBitte installieren Sie v${GTK_VERSION} oder h�her der GTK+ Runtime"

; Components Page
LangString GAIM_SECTION_TITLE				${LANG_GERMAN} "Gaim Instant Messaging Client (erforderlich)"
LangString GTK_SECTION_TITLE				${LANG_GERMAN} "GTK+ Runtime Umgebung (erforderlich)"
LangString GTK_THEMES_SECTION_TITLE			${LANG_GERMAN} "GTK+ Themen"
LangString GTK_NOTHEME_SECTION_TITLE		${LANG_GERMAN} "Kein Thema"
LangString GTK_WIMP_SECTION_TITLE			${LANG_GERMAN} "Wimp Thema"
LangString GTK_BLUECURVE_SECTION_TITLE		${LANG_GERMAN} "Bluecurve Thema"
LangString GTK_LIGHTHOUSEBLUE_SECTION_TITLE	${LANG_GERMAN} "Light House Blue Thema"
LangString GAIM_SECTION_DESCRIPTION			${LANG_GERMAN} "Gaim Basis-Dateien und -DLLs"
LangString GTK_SECTION_DESCRIPTION			${LANG_GERMAN} "Ein Multi-Plattform GUI Toolkit, verwendet von Gaim"
LangString GTK_THEMES_SECTION_DESCRIPTION		${LANG_GERMAN} "GTK+ Themen k�nnen Aussehen und Bedienung von GTK+ Anwendungen ver�ndern."
LangString GTK_NO_THEME_DESC				${LANG_GERMAN} "Installiere kein GTK+ Thema"
LangString GTK_WIMP_THEME_DESC			${LANG_GERMAN} "GTK-Wimp (Windows Imitator) ist ein GTK Theme, da� sich besonders gut in den Windows Desktop integriert."
LangString GTK_BLUECURVE_THEME_DESC			${LANG_GERMAN} "Das Bluecurve Thema."
LangString GTK_LIGHTHOUSEBLUE_THEME_DESC		${LANG_GERMAN} "Das Lighthouseblue Thema."

; Extra GTK+ Dir Selector Page
LangString GTK_PAGE_TITLE				${LANG_GERMAN} "W�hlen Sie den Installationsort"
LangString GTK_PAGE_SUBTITLE				${LANG_GERMAN} "W�hlen Sie den Ordner, indem Sie GTK+ installieren"
LangString GTK_PAGE_INSTALL_MSG1			${LANG_GERMAN} "Das Setup wird GTK+ in den folgenden Ordner installieren"
LangString GTK_PAGE_INSTALL_MSG2			${LANG_GERMAN} "Um in einen anderen Ordner zu installieren, klicken Sie auf Durchsuchen und w�hlen Sie einen anderen Ordner. Klicken Sie Weiter zum Fortfahren."
LangString GTK_PAGE_UPGRADE_MSG1			${LANG_GERMAN} "Das Setup wird GTK+, gefunden im folgenden Ordner, aktualisieren"
LangString GTK_UPGRADE_PROMPT				${LANG_GERMAN} "Eine alte Version der GTK+ Runtime wurde gefunden. M�chten Sie aktualisieren?$\rHinweis: Gaim funktioniert evtl. nicht, wenn Sie nicht aktualisieren."

; Gaim Section Prompts and Texts
LangString GAIM_UNINSTALL_DESC			${LANG_GERMAN} "Gaim (nur entfernen)"
LangString GAIM_PROMPT_WIPEOUT			${LANG_GERMAN} "Ihre altes Gaim-Verzeichnis soll gel�scht werden. M�chten Sie fortfahren?$\r$\rHinweis: Alle nicht-Standard Plugins, die Sie evtl. installiert haben werden$\rgel�scht. Gaim-Benutzereinstellungen sind nicht betroffen."
LangString GAIM_PROMPT_DIR_EXISTS			${LANG_GERMAN} "Das Installationsverzeichnis, da� Sie angegeben haben, existiert schon. Der Verzeichnisinhalt$\rwird gel�scht. M�chten Sie fortfahren?"

; GTK+ Section Prompts
LangString GTK_INSTALL_ERROR				${LANG_GERMAN} "Fehler beim Installieren der GTK+ Runtime."
LangString GTK_BAD_INSTALL_PATH			${LANG_GERMAN} "Der Pfad, den Sie eingegeben haben, existiert nicht und kann nicht erstellt werden."

; GTK+ Themes section
LangString GTK_NO_THEME_INSTALL_RIGHTS		${LANG_GERMAN} "Sie haben keine Berechtigung, um ein GTK+ Theme zu installieren."

; Uninstall Section Prompts
LangString un.GAIM_UNINSTALL_ERROR_1         	${LANG_GERMAN} "Der Deinstaller konnte keine Registrierungsshl�ssel f�r Gaim finden.$\rEs ist wahrscheinlich, da� ein anderer Benutzer diese Anwendunng installiert hat."
LangString un.GAIM_UNINSTALL_ERROR_2         	${LANG_GERMAN} "Sie haben keine Berechtigung, diese Anwendung zu deinstallieren."
