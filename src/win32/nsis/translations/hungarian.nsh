;;
;;  hungarian.nsh
;;
;;  Default language strings for the Windows Gaim NSIS installer.
;;  Windows Code page: 1250
;;
;;  Author: Sutto Zoltan <suttozoltan@chello.hu
;;

; Startup GTK+ check
!define GTK_INSTALLER_NEEDED			"A GTK+ futtat� k�rnyezet hi�nyzik vagy �jabb verzi� sz�ks�ges.$\rK�rem install�lja a v${GTK_VERSION} vagy magasabb verzi�j� GTK+ futtat� k�rnyezetet."

; Components Page
!define GAIM_SECTION_TITLE			"Gaim IM kliens (sz�ks�ges)"
!define GTK_SECTION_TITLE			"GTK+ futtat� k�rnyezet (sz�ks�ges)"
!define GTK_THEMES_SECTION_TITLE		"GTK+ t�m�k"
!define GTK_NOTHEME_SECTION_TITLE		"Nincs t�ma"
!define GTK_WIMP_SECTION_TITLE		"Wimp t�ma"
!define GTK_BLUECURVE_SECTION_TITLE		"Bluecurve t�ma"
!define GTK_LIGHTHOUSEBLUE_SECTION_TITLE	"Light House Blue t�ma"
!define GAIM_SECTION_DESCRIPTION		"Gaim f�jlok �s dll-ek"
!define GTK_SECTION_DESCRIPTION		"Gaim �ltal haszn�lt t�bb-platformos grafikus k�rnyezet"
!define GTK_THEMES_SECTION_DESCRIPTION	"GTK+ t�m�k megv�ltoztatj�k a GTK+ alkalmaz�sok kin�zet�t."
!define GTK_NO_THEME_DESC			"Ne install�lja a GTK+ t�m�kat"
!define GTK_WIMP_THEME_DESC			"GTK-Wimp (Windows ut�nzat) egy Windows k�rnyezettel harmoniz�l� GTK t�ma."
!define GTK_BLUECURVE_THEME_DESC		"A Bluecurve t�ma."
!define GTK_LIGHTHOUSEBLUE_THEME_DESC	"A Lighthouseblue t�ma."

; GTK+ Directory Page
!define GTK_UPGRADE_PROMPT			"Egy r�gi verzi�j� GTK+ futtat� k�rnyezet van telep�tve. K�v�nja friss�teni?$\rMegjegyz�s: Gaim val�sz�n�leg nem fog m�k�dni amig nem friss�ti."

; Gaim Section Prompts and Texts
!define GAIM_UNINSTALL_DESC			"Gaim (csak elt�vol�t�s)"
!define GAIM_PROMPT_WIPEOUT			"Az �n kor�bbi Gaim k�nyvt�ra t�rl�dni fog. Folytatni szeretn�?$\r$\rMegjegyz�s: Minden �n �ltal telep�tett plugin t�rl�dni fog.$\rGaim felhaszn�l�i be�ll�t�sokat ez nem �rinti."
!define GAIM_PROMPT_DIR_EXISTS		"A telep�t�skor megadott k�nyvt�r m�r l�tezik. Minden �llom�ny t�rl�dni fog.$\rFolytatni szeretn�?"

; GTK+ Section Prompts
!define GTK_INSTALL_ERROR			"Hiba a GTK+ futtat� telep�t�se k�zben."
!define GTK_BAD_INSTALL_PATH			"A megadott el�r�si �t nem el�rhet� vagy nem hozhat� l�tre."

; GTK+ Themes section
!define GTK_NO_THEME_INSTALL_RIGHTS		"Nincs jogosults�ga a GTK+ t�m�k telep�t�s�hez."

; Uninstall Section Prompts
!define un.GAIM_UNINSTALL_ERROR_1         "Az elt�vol�t� nem tal�lt Gaim registry bejegyz�seket.$\rVal�sz�n�leg egy m�sik felhaszn�l� telep�tette az alkalmaz�st."
!define un.GAIM_UNINSTALL_ERROR_2         "Nincs jogosults�ga az alkalmaz�s elt�vol�t�s�hoz."
