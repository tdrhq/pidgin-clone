;;
;;  lithuanian.nsh
;;
;;  Lithuanian translation strings for the Windows Gaim NSIS installer.
;;  Windows Code page: 1257
;;
;;  Version 1
;;  Note: If translating this file, replace "!insertmacro GAIM_MACRO_DEFAULT_STRING"
;;  with "!define".

; Make sure to update the GAIM_MACRO_LANGUAGEFILE_END macro in
; langmacros.nsh when updating this file

; Startup Checks
!define GAIM_MACRO_DEFAULT_STRING INSTALLER_IS_RUNNING			"Diegimo programa jau paleista."
!define GAIM_MACRO_DEFAULT_STRING GAIM_IS_RUNNING			"�iuo metu Gaim yra paleistas. U�darykite �i� program� ir pabandykite i� naujo."
!define GAIM_MACRO_DEFAULT_STRING GTK_INSTALLER_NEEDED			"GTK+ vykdymo meto aplinkos n�ra arba ji turi b�ti atnaujinta.$\r�diekite v${GTK_VERSION} arba naujesn� GTK+ vykdymo meto aplinkos versij�"

; License Page
!define GAIM_MACRO_DEFAULT_STRING GAIM_LICENSE_BUTTON			"Toliau >"
!define GAIM_MACRO_DEFAULT_STRING GAIM_LICENSE_BOTTOM_TEXT		"$(^Name) yra i�leistas GNU bendrosios vie�osios licenzijos (GPL) s�lygomis.  Licenzija �ia yra pateikta tik susipa�inimo tikslams. $_CLICK"

; Components Page
!define GAIM_MACRO_DEFAULT_STRING GAIM_SECTION_TITLE			"Gaim pokalbi� klientas (b�tinas)"
!define GAIM_MACRO_DEFAULT_STRING GTK_SECTION_TITLE			"GTK+ vykdymo meto aplinka (b�tina)"
!define GAIM_MACRO_DEFAULT_STRING GTK_THEMES_SECTION_TITLE		"GTK+ apipavidalinimai"
!define GAIM_MACRO_DEFAULT_STRING GTK_NOTHEME_SECTION_TITLE		"Jokio apipavidalinimo"
!define GAIM_MACRO_DEFAULT_STRING GTK_WIMP_SECTION_TITLE		"Wimp apipavidalinimas"
!define GAIM_MACRO_DEFAULT_STRING GTK_BLUECURVE_SECTION_TITLE	"Bluecurve apipavidalinimas"
!define GAIM_MACRO_DEFAULT_STRING GTK_LIGHTHOUSEBLUE_SECTION_TITLE	"Light House Blue apipavidalinimas"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SHORTCUTS_SECTION_TITLE "Nuorodos"
!define GAIM_MACRO_DEFAULT_STRING GAIM_DESKTOP_SHORTCUT_SECTION_TITLE "Darbalaukyje"
!define GAIM_MACRO_DEFAULT_STRING GAIM_STARTMENU_SHORTCUT_SECTION_TITLE "Pradiniame meniu"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SECTION_DESCRIPTION		"Pagrindiniai Gaim failai"
!define GAIM_MACRO_DEFAULT_STRING GTK_SECTION_DESCRIPTION		"Daugiaplatforminis vartotojo s�sajos priemoni� komplektas, naudojamas Gaim."
!define GAIM_MACRO_DEFAULT_STRING GTK_THEMES_SECTION_DESCRIPTION	"GTK+ apipavidalinimai gali pakeisti GTK+ program� i�vaizd�."
!define GAIM_MACRO_DEFAULT_STRING GTK_NO_THEME_DESC			"Ne�diegti GTK+ apipavidalinimo."
!define GAIM_MACRO_DEFAULT_STRING GTK_WIMP_THEME_DESC			"GTK-Wimp (Windows imitatorius) yra gerai Windows aplinkoje derantis GTK apipavidalinimas."
!define GAIM_MACRO_DEFAULT_STRING GTK_BLUECURVE_THEME_DESC		"Bluecurve apipavidalinimas."
!define GAIM_MACRO_DEFAULT_STRING GTK_LIGHTHOUSEBLUE_THEME_DESC	"Lighthouseblue apipavidalinimas."
!define GAIM_MACRO_DEFAULT_STRING GAIM_SHORTCUTS_SECTION_DESCRIPTION   "Gaim paleidimo nuorodos"
!define GAIM_MACRO_DEFAULT_STRING GAIM_DESKTOP_SHORTCUT_DESC   "Sukurti nuorod� � Gaim darbastalyje."
!define GAIM_MACRO_DEFAULT_STRING GAIM_STARTMENU_SHORTCUT_DESC   "Sukurti pradinio meniu �ra��, skirt� Gaim."

; GTK+ Directory Page
!define GAIM_MACRO_DEFAULT_STRING GTK_UPGRADE_PROMPT			"Rasta sena GTK+ vykdymo meto aplinkos versija. Ar norite j� atnaujinti?$\rPastaba: skland�iam Gaim darbui atnaujinimas gali b�ti reikalingas."

; Installer Finish Page
!define GAIM_MACRO_DEFAULT_STRING GAIM_FINISH_VISIT_WEB_SITE		"Aplankyti Windows Gaim tinklalap�"

; Gaim Section Prompts and Texts
!define GAIM_MACRO_DEFAULT_STRING GAIM_UNINSTALL_DESC			"Gaim (tik pa�alinti)"
!define GAIM_MACRO_DEFAULT_STRING GAIM_PROMPT_WIPEOUT			"J�s� senasis Gaim katalogas tuoj tur�t� b�ti i�trintas. Ar norite t�sti?$\r$\rPastaba: bet kokie nestandartiniai papildiniai, jeigu toki� �dieg�te, bus pa�alinti.$\rGaim vartotojo nustatymai nebus paliesti."
!define GAIM_MACRO_DEFAULT_STRING GAIM_PROMPT_DIR_EXISTS		"Nurodytas diegimo katalogas jau yra. Jo turinys$\rbus pa�alintas. Ar norite t�sti?"

; GTK+ Section Prompts
!define GAIM_MACRO_DEFAULT_STRING GTK_INSTALL_ERROR			"GTK+ vykdymo meto aplinkos diegimo klaida."
!define GAIM_MACRO_DEFAULT_STRING GTK_BAD_INSTALL_PATH			"J�s� nurodytas katalogas neprieinamas ar negali b�ti sukurtas."

; GTK+ Themes section
!define GAIM_MACRO_DEFAULT_STRING GTK_NO_THEME_INSTALL_RIGHTS	"Neturite teisi� �diegti GTK+ apipavidalinim�."

; Uninstall Section Prompts
!define GAIM_MACRO_DEFAULT_STRING un.GAIM_UNINSTALL_ERROR_1		"�alinimo programa nerado Gaim registro �ra��.$\rGalb�t kitas vartotojas instaliavo �i� program�."
!define GAIM_MACRO_DEFAULT_STRING un.GAIM_UNINSTALL_ERROR_2		"Neturite teisi� pa�alinti �ios programos."

; Spellcheck Section Prompts
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_SECTION_TITLE		"Ra�ybos tikrinimo palaikymas"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_ERROR			"Ra�ybos tikrinimo diegimo klaida"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_DICT_ERROR		"Ra�ybos tikrinimo �odyno diegimo klaida"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_SECTION_DESCRIPTION	"Ra�ybos tikrinimo palaikymas.  �diegimui b�tina interneto jungtis."
!define GAIM_MACRO_DEFAULT_STRING ASPELL_INSTALL_FAILED			"Diegimas nepavyko"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_BRETON			"Breton� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_CATALAN			"Katalon� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_CZECH			"�ek� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_WELSH			"Val� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_DANISH			"Dan� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_GERMAN			"Vokie�i� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_GREEK			"Graik� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_ENGLISH			"Angl� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_ESPERANTO		"Esperanto kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_SPANISH			"Ispan� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_FAROESE			"Farer� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_FRENCH			"Pranc�z� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_ITALIAN			"Ital� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_DUTCH			"Oland� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_NORWEGIAN		"Norveg� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_POLISH			"Lenk� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_PORTUGUESE		"Portugal� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_ROMANIAN			"Rumun� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_RUSSIAN			"Rus� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_SLOVAK			"Slovak� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_SWEDISH			"�ved� kalba"
!define GAIM_MACRO_DEFAULT_STRING GAIM_SPELLCHECK_UKRAINIAN		"Ukrainie�i� kalba"
