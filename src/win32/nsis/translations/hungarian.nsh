;;
;;  hungarian.nsh
;;
;;  Default language strings for the Windows Gaim NSIS installer.
;;  Windows Code page: 1250
;;
;;  Author: Sutto Zoltan <suttozoltan@chello.hu
;;

; Startup GTK+ check
LangString GTK_INSTALLER_NEEDED			${LANG_HUNGARIAN} "A GTK+ futtat� k�rnyezet hi�nyzik vagy �jabb verzi� sz�ks�ges.$\rK�rem install�lja a v${GTK_VERSION} vagy magasabb verzi�j� GTK+ futtat� k�rnyezetet."

; Components Page
LangString GAIM_SECTION_TITLE				${LANG_HUNGARIAN} "Gaim IM kliens (sz�ks�ges)"
LangString GTK_SECTION_TITLE				${LANG_HUNGARIAN} "GTK+ futtat� k�rnyezet (sz�ks�ges)"
LangString GTK_THEMES_SECTION_TITLE			${LANG_HUNGARIAN} "GTK+ t�m�k"
LangString GTK_NOTHEME_SECTION_TITLE		${LANG_HUNGARIAN} "Nincs t�ma"
LangString GTK_WIMP_SECTION_TITLE			${LANG_HUNGARIAN} "Wimp t�ma"
LangString GTK_BLUECURVE_SECTION_TITLE		${LANG_HUNGARIAN} "Bluecurve t�ma"
LangString GTK_LIGHTHOUSEBLUE_SECTION_TITLE	${LANG_HUNGARIAN} "Light House Blue t�ma"
LangString GAIM_SECTION_DESCRIPTION			${LANG_HUNGARIAN} "Gaim f�jlok �s dll-ek"
LangString GTK_SECTION_DESCRIPTION			${LANG_HUNGARIAN} "Gaim �ltal haszn�lt t�bb-platformos grafikus k�rnyezet"
LangString GTK_THEMES_SECTION_DESCRIPTION		${LANG_HUNGARIAN} "GTK+ t�m�k megv�ltoztatj�k a GTK+ alkalmaz�sok kin�zet�t."
LangString GTK_NO_THEME_DESC				${LANG_HUNGARIAN} "Ne install�lja a GTK+ t�m�kat"
LangString GTK_WIMP_THEME_DESC			${LANG_HUNGARIAN} "GTK-Wimp (Windows ut�nzat) egy Windows k�rnyezettel harmoniz�l� GTK t�ma."
LangString GTK_BLUECURVE_THEME_DESC			${LANG_HUNGARIAN} "A Bluecurve t�ma."
LangString GTK_LIGHTHOUSEBLUE_THEME_DESC		${LANG_HUNGARIAN} "A Lighthouseblue t�ma."

; Extra GTK+ Dir Selector Page
LangString GTK_PAGE_TITLE				${LANG_HUNGARIAN} "Telep�t�s hely�nek kiv�laszt�sa"
LangString GTK_PAGE_SUBTITLE				${LANG_HUNGARIAN} "V�lassza ki azt a k�nyvt�rat ahova a GTK+ k�v�nja telep�teni"
LangString GTK_PAGE_INSTALL_MSG1			${LANG_HUNGARIAN} "A telep�t� a k�vetkez� k�nyvt�rba fogja telep�teni a GTK+"
LangString GTK_PAGE_INSTALL_MSG2			${LANG_HUNGARIAN} "M�sik k�nyvt�rba telep�t�shez kattintson a Tall�z gombra �s v�lasszon egy m�sik k�nyvt�rat. Kattintson a Tov�bb gombra."
LangString GTK_PAGE_UPGRADE_MSG1			${LANG_HUNGARIAN} "A telep�t� friss�teni fogja a GTK+, melyet a k�vetkez� k�nyvt�rban tal�lt"
LangString GTK_UPGRADE_PROMPT				${LANG_HUNGARIAN} "Egy r�gi verzi�j� GTK+ futtat� k�rnyezet van telep�tve. K�v�nja friss�teni?$\rMegjegyz�s: Gaim val�sz�n�leg nem fog m�k�dni amig nem friss�ti."

; Gaim Section Prompts and Texts
LangString GAIM_UNINSTALL_DESC			${LANG_HUNGARIAN} "Gaim (csak elt�vol�t�s)"
LangString GAIM_PROMPT_WIPEOUT			${LANG_HUNGARIAN} "Az �n kor�bbi Gaim k�nyvt�ra t�rl�dni fog. Folytatni szeretn�?$\r$\rMegjegyz�s: Minden �n �ltal telep�tett plugin t�rl�dni fog.$\rGaim felhaszn�l�i be�ll�t�sokat ez nem �rinti."
LangString GAIM_PROMPT_DIR_EXISTS			${LANG_HUNGARIAN} "A telep�t�skor megadott k�nyvt�r m�r l�tezik. Minden �llom�ny t�rl�dni fog.$\rFolytatni szeretn�?"

; GTK+ Section Prompts
LangString GTK_INSTALL_ERROR				${LANG_HUNGARIAN} "Hiba a GTK+ futtat� telep�t�se k�zben."
LangString GTK_BAD_INSTALL_PATH			${LANG_HUNGARIAN} "A megadott el�r�si �t nem el�rhet� vagy nem hozhat� l�tre."

; GTK+ Themes section
LangString GTK_NO_THEME_INSTALL_RIGHTS		${LANG_HUNGARIAN} "Nincs jogosults�ga a GTK+ t�m�k telep�t�s�hez."

; Uninstall Section Prompts
LangString un.GAIM_UNINSTALL_ERROR_1         	${LANG_HUNGARIAN} "Az elt�vol�t� nem tal�lt Gaim registry bejegyz�seket.$\rVal�sz�n�leg egy m�sik felhaszn�l� telep�tette az alkalmaz�st."
LangString un.GAIM_UNINSTALL_ERROR_2         	${LANG_HUNGARIAN} "Nincs jogosults�ga az alkalmaz�s elt�vol�t�s�hoz."
