;;
;;  romanian.nsh
;;
;;  Romanian language strings for the Windows Gaim NSIS installer.
;;  Windows Code page: 1250
;;
;;  Author: Mi�u Moldovan <dumol@go.ro>
;;

; Startup GTK+ check
LangString GTK_INSTALLER_NEEDED			${LANG_ROMANIAN} "Mediul GTK+ nu e prezent sau ave�i o versiune prea veche.$\rInstala�i cel pu�in versiunea v${GTK_VERSION} a mediului GTK+"

; Components Page
LangString GAIM_SECTION_TITLE				${LANG_ROMANIAN} "Client de mesagerie instantanee (obligatoriu)"
LangString GTK_SECTION_TITLE				${LANG_ROMANIAN} "Mediu GTK+ (obligatoriu)"
LangString GTK_THEMES_SECTION_TITLE			${LANG_ROMANIAN} "Teme GTK+"
LangString GTK_NOTHEME_SECTION_TITLE		${LANG_ROMANIAN} "F�r� teme"
LangString GTK_WIMP_SECTION_TITLE			${LANG_ROMANIAN} "Tem� Wimp"
LangString GTK_BLUECURVE_SECTION_TITLE		${LANG_ROMANIAN} "Tem� Bluecurve"
LangString GTK_LIGHTHOUSEBLUE_SECTION_TITLE	${LANG_ROMANIAN} "Tem� Light House Blue"
LangString GAIM_SECTION_DESCRIPTION			${LANG_ROMANIAN} "Fi�iere Gaim �i dll-uri"
LangString GTK_SECTION_DESCRIPTION			${LANG_ROMANIAN} "Un mediu de dezvoltare multiplatform� utilizat de Gaim"
LangString GTK_THEMES_SECTION_DESCRIPTION		${LANG_ROMANIAN} "Temele GTK+ schimb� aparen�a aplica�iilor GTK+."
LangString GTK_NO_THEME_DESC				${LANG_ROMANIAN} "Nu instala o tem� GTK+"
LangString GTK_WIMP_THEME_DESC			${LANG_ROMANIAN} "GTK-Wimp este o tem� GTK �n acord cu mediul Windows."
LangString GTK_BLUECURVE_THEME_DESC			${LANG_ROMANIAN} "Tema Bluecurve."
LangString GTK_LIGHTHOUSEBLUE_THEME_DESC		${LANG_ROMANIAN} "Tema Lighthouseblue."

; Extra GTK+ Dir Selector Page
LangString GTK_PAGE_TITLE				${LANG_ROMANIAN} "Alege�i loca�ia instal�rii"
LangString GTK_PAGE_SUBTITLE				${LANG_ROMANIAN} "Alege�i directorul �n care dori�i s� instala�i GTK+"
LangString GTK_PAGE_INSTALL_MSG1			${LANG_ROMANIAN} "Instalarea va copia GTK+ �n acest director"
LangString GTK_PAGE_INSTALL_MSG2			${LANG_ROMANIAN} "Pentru a instala �ntr-un alt director, ap�sa�i Browse �i alege�i un alt director. Ap�sa�i Next pentru a continua"
LangString GTK_PAGE_UPGRADE_MSG1			${LANG_ROMANIAN} "Instalarea va actualiza mediul GTK+ prezent �n directorul"
LangString GTK_UPGRADE_PROMPT				${LANG_ROMANIAN} "Ave�i o versiune veche a mediului GTK+. Dori�i s� o actualiza�i?$\rNot�: E posibil ca Gaim s� nu func�ioneze cu versiunea veche."

; Gaim Section Prompts and Texts
LangString GAIM_UNINSTALL_DESC			${LANG_ROMANIAN} "Gaim (doar dezinstalare)"
LangString GAIM_PROMPT_WIPEOUT			${LANG_ROMANIAN} "Vechiul director Gaim va fi �ters. Dori�i s� continua�i?$\r$\rNot�: Orice module externe vor fi �terse.$\rSet�rile utilizatorilor Gaim nu vor fi afectate."
LangString GAIM_PROMPT_DIR_EXISTS			${LANG_ROMANIAN} "Directorul ales pentru instalare exist� deja.$\rCon�inutul s�u va fi �ters. Dori�i s� continua�i?"

; GTK+ Section Prompts
LangString GTK_INSTALL_ERROR				${LANG_ROMANIAN} "Eroare la instalarea mediului GTK+."
LangString GTK_BAD_INSTALL_PATH			${LANG_ROMANIAN} "Directorul specificat nu poate fi accesat sau creat."

; GTK+ Themes section
LangString GTK_NO_THEME_INSTALL_RIGHTS		${LANG_ROMANIAN} "Nu ave�i drepturile de acces necesare instal�rii unei teme GTK+."

; Uninstall Section Prompts
LangString un.GAIM_UNINSTALL_ERROR_1         	${LANG_ROMANIAN} "Programul de dezinstalare nu a g�sit intr�ri Gaim �n regi�tri.$\rProbabil un alt utilizator a instalat aceast� aplica�ie."
LangString un.GAIM_UNINSTALL_ERROR_2         	${LANG_ROMANIAN} "Nu ave�i drepturile de acces necesare dezinstal�rii acestei aplica�ii."
