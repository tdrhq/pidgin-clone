;;
;;  norwegian.nsh
;;
;;  Norwegian language strings for the Windows Gaim NSIS installer.
;;  Windows Code page: 1252
;;
;;  J�rgen_Vinne_Iversen <jorgenvi@tihlde.org>
;;  Version 2
;;

; Startup GTK+ check
!define GTK_INSTALLER_NEEDED			"GTK+ runtime environment mangler eller trenger en oppgradering.$\rVennligst install�r GTK+ v${GTK_VERSION} eller h�yere"

; License Page
!define GAIM_LICENSE_BUTTON			"Neste >"
!define GAIM_LICENSE_BOTTOM_TEXT		"$(^Name) er utgitt under GPL (GNU General Public License). Lisensen er oppgitt her kun med henblikk p� informasjon. $_CLICK"

; Components Page
!define GAIM_SECTION_TITLE			"Gaim Hurtigmeldingsklient (obligatorisk)"
!define GTK_SECTION_TITLE			"GTK+ Runtime Environment (obligatorisk)"
!define GTK_THEMES_SECTION_TITLE		"GTK+ Tema"
!define GTK_NOTHEME_SECTION_TITLE		"Ingen tema"
!define GTK_WIMP_SECTION_TITLE		"Wimp-tema"
!define GTK_BLUECURVE_SECTION_TITLE		"Bluecurve-tema"
!define GTK_LIGHTHOUSEBLUE_SECTION_TITLE	"Light House Blue-tema"
!define GAIM_SECTION_DESCRIPTION		"Gaims kjernefiler og dll'er"
!define GTK_SECTION_DESCRIPTION		"Et GUI-verkt�y for flere ulike plattformer, brukes av GAIM."
!define GTK_THEMES_SECTION_DESCRIPTION	"GTK+ Tema kan endre utseendet og f�lelsen av GTK+ applikasjoner."
!define GTK_NO_THEME_DESC			"Ikke install�r noe GTK+ tema."
!define GTK_WIMP_THEME_DESC			"GTK-Wimp (Windows-imitator) er et GTK-tema som passer godt inn i Windows-milj�."
!define GTK_BLUECURVE_THEME_DESC		"Bluecurve-tema."
!define GTK_LIGHTHOUSEBLUE_THEME_DESC	"Lighthouseblue-tema."

; GTK+ Directory Page
!define GTK_UPGRADE_PROMPT			"En eldre versjon av GTK+ runtime ble funnet. �nsker du � oppgradere?$\rMerk: Gaim vil kanskje ikke virke hvis du ikke oppgraderer."

; Installer Finish Page
!define GAIM_FINISH_VISIT_WEB_SITE		"Bes�k Gaim for Windows' Nettside"

; Gaim Section Prompts and Texts
!define GAIM_UNINSTALL_DESC			"Gaim (kun avinstallering)"
!define GAIM_PROMPT_WIPEOUT			"Din gamle Gaim-katalog holder p� � slettes. �nsker du � fortsette?$\r$\rMerk: Eventuelle ikke-standard plugin'er du har installert vil bli slettet.$\rGaims brukerinstillinger vil ikke bli ber�rt."
!define GAIM_PROMPT_DIR_EXISTS		"Installasjonskatalogen du har spesifisert finnes fra f�r. Eventuelt innhold$\rvil bli slettet. �nsker du � fortsette?"

; GTK+ Section Prompts
!define GTK_INSTALL_ERROR			"En feil oppstod ved installering av GTK+ runtime."
!define GTK_BAD_INSTALL_PATH			"Stien du oppga kan ikke aksesseres eller lages."

; GTK+ Themes section
!define GTK_NO_THEME_INSTALL_RIGHTS		"Du har ikke rettigheter til � installere et GTK+ tema."

; Uninstall Section Prompts
!define un.GAIM_UNINSTALL_ERROR_1		"Avinstalleringsprogrammet kunne ikke finne noen registeroppf�ring for Gaim.$\rTrolig har en annen bruker installert denne applikasjonen."
!define un.GAIM_UNINSTALL_ERROR_2		"Du har ikke rettigheter til � avinstallere denne applikasjonen."
