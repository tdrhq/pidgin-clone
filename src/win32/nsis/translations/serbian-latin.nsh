;;
;;  serbian-latin.nsh
;;
;;  Serbian (Latin) language strings for the Windows Gaim NSIS installer.
;;  Windows Code page: 1250
;;
;;  Author: Danilo Segan <dsegan@gmx.net>
;;

; Startup GTK+ check
LangString GTK_INSTALLER_NEEDED			${LANG_SERBIAN} "GTK+ okolina za izvr�avanje ili nije na�ena ili se moraunaprediti.$\rMolimo instalirajte v${GTK_VERSION} ili ve�u GTK+ okoline za izvr�avanje"

; Components Page
LangString GAIM_SECTION_TITLE				${LANG_SERBIAN} "Gaim klijent za brze poruke (neophodno)"
LangString GTK_SECTION_TITLE				${LANG_SERBIAN} "GTK+ okolina za izvr�avanje (neophodno)"
LangString GTK_THEMES_SECTION_TITLE			${LANG_SERBIAN} "GTK+ teme"
LangString GTK_NOTHEME_SECTION_TITLE		${LANG_SERBIAN} "Bez teme"
LangString GTK_WIMP_SECTION_TITLE			${LANG_SERBIAN} "Wimp tema"
LangString GTK_BLUECURVE_SECTION_TITLE		${LANG_SERBIAN} "Bluecurve tema"
LangString GTK_LIGHTHOUSEBLUE_SECTION_TITLE	${LANG_SERBIAN} "Light House Blue tema"
LangString GAIM_SECTION_DESCRIPTION			${LANG_SERBIAN} "Osnovne Gaim datoteke i dinami�ke biblioteke"
LangString GTK_SECTION_DESCRIPTION			${LANG_SERBIAN} "Skup oru�a za grafi�ko okru�enje, za vi�e platformi, koristi ga Gaim"
LangString GTK_THEMES_SECTION_DESCRIPTION		${LANG_SERBIAN} "GTK+ teme menjaju izgled i na�in rada GTK+ aplikacija."
LangString GTK_NO_THEME_DESC				${LANG_SERBIAN} "Ne instaliraj GTK+ temu"
LangString GTK_WIMP_THEME_DESC			${LANG_SERBIAN} "GTK-Wimp (Windows imitator) je GTK tema koja se dobro uklapa u Windows radno okru�enje."
LangString GTK_BLUECURVE_THEME_DESC			${LANG_SERBIAN} "Bluecurve tema."
LangString GTK_LIGHTHOUSEBLUE_THEME_DESC		${LANG_SERBIAN} "Lighthouseblue tema."

; Extra GTK+ Dir Selector Page
LangString GTK_PAGE_TITLE				${LANG_SERBIAN} "Izaberite mesto gde �e se instalirati program"
LangString GTK_PAGE_SUBTITLE				${LANG_SERBIAN} "Izaberite folder u koji �elite da se GTK+ instalira"
LangString GTK_PAGE_INSTALL_MSG1			${LANG_SERBIAN} "Program �e instalirati GTK+ u ovaj folder"
LangString GTK_PAGE_INSTALL_MSG2			${LANG_SERBIAN} "Da biste instalirali u neki drugi folder, kliknite na Pregled i izaberite neki drugi folder. Kliknite na Nastavak da bi produ�ili dalje."
LangString GTK_PAGE_UPGRADE_MSG1			${LANG_SERBIAN} "Program �e unaprediti GTK+ koji je na�en u ovom folderu"
LangString GTK_UPGRADE_PROMPT				${LANG_SERBIAN} "Na�ena je stara verzija GTK+ izvr�ne okoline. Da li �elite da je unapredite?$\rPrimedba: Ukoliko to ne uradite, Gaim mo�da ne�e raditi."

; Gaim Section Prompts and Texts
LangString GAIM_UNINSTALL_DESC			${LANG_SERBIAN} "Gaim (samo uklanjanje)"
LangString GAIM_PROMPT_WIPEOUT			${LANG_SERBIAN} "Va� stari Gaim direktorijum �e biti obrisan. Da li �elite da nastavite?$\r$\rPrimedba: Svi nestandardni dodaci koje ste instalirali �e biti obrisani.$\rGaim postavke korisnika ne�e biti promenjene."
LangString GAIM_PROMPT_DIR_EXISTS			${LANG_SERBIAN} "Instalacioni direktorijum koji ste naveli ve� postoji. Sav sadr�aj$\r�e biti obrisan. Da li �elite da nastavite?"

; GTK+ Section Prompts
LangString GTK_INSTALL_ERROR				${LANG_SERBIAN} "Gre�ka prilikom instalacije GTK+ okoline za izvr�avanje."
LangString GTK_BAD_INSTALL_PATH			${LANG_SERBIAN} "Putanja koju ste naveli se ne mo�e ni napraviti niti joj se mo�e pri�i."

; GTK+ Themes section
LangString GTK_NO_THEME_INSTALL_RIGHTS		${LANG_SERBIAN} "Nemate ovla��enja za instalaciju GTK+ teme."

; Uninstall Section Prompts
LangString un.GAIM_UNINSTALL_ERROR_1         	${LANG_SERBIAN} "Program za uklanjanje instalacije ne mo�e da prona�e stavke registra za Gaim.$\rVerovatno je ovu aplikaciju instalirao drugi korisnik."
LangString un.GAIM_UNINSTALL_ERROR_2         	${LANG_SERBIAN} "Nemate ovla��enja za deinstalaciju ove aplikacije."
