;;
;;  portuguese.nsh
;;
;;  Portuguese (PT) language strings for the Windows Gaim NSIS installer.
;;  Windows Code page: 1252
;;
;;  Author: Duarte Serrano Gon�alves Henriques <duarte_henriques@myrealbox.com>, 2003.
;;  Version 2
;;

; Startup GTK+ check
LangString GTK_INSTALLER_NEEDED			${LANG_PORTUGUESE} "O ambiente de tempo de execu��o do GTK+ est� ausente ou precisa de ser actualizado.$\rPor favor instale a vers�o v${GTK_VERSION} ou superior do ambiente de tempo de execu��o do GTK+."

; License Page
LangString GAIM_LICENSE_BUTTON			${LANG_PORTUGUESE} "Seguinte >"
LangString GAIM_LICENSE_BOTTOM_TEXT			${LANG_PORTUGUESE} "$(^Name) est� dispon�vel sob alicen�a GPL. O texto da licen�a � fornecido meramente a t�tulo informativo. $_CLICK" 

; Components Page
LangString GAIM_SECTION_TITLE				${LANG_PORTUGUESE} "Cliente de mensagens instant�neas Gaim (obrigat�rio)"
LangString GTK_SECTION_TITLE				${LANG_PORTUGUESE} "Ambiente de tempo de execu��o do GTK+ (obrigat�rio)"
LangString GTK_THEMES_SECTION_TITLE			${LANG_PORTUGUESE} "Temas do GTK+"
LangString GTK_NOTHEME_SECTION_TITLE		${LANG_PORTUGUESE} "Nenhum tema"
LangString GTK_WIMP_SECTION_TITLE			${LANG_PORTUGUESE} "Tema 'Wimp'"
LangString GTK_BLUECURVE_SECTION_TITLE		${LANG_PORTUGUESE} "Tema 'Bluecurve'"
LangString GTK_LIGHTHOUSEBLUE_SECTION_TITLE	${LANG_PORTUGUESE} "Tema 'Light House Blue'"
LangString GAIM_SECTION_DESCRIPTION			${LANG_PORTUGUESE} "Ficheiros e bibliotecas principais do Gaim"
LangString GTK_SECTION_DESCRIPTION			${LANG_PORTUGUESE} "Um conjunto de ferramentas de interface gr�fica multi-plataforma, usado pelo Gaim"
LangString GTK_THEMES_SECTION_DESCRIPTION		${LANG_PORTUGUESE} "Os temas do GTK+ podem mudar a apar�ncia dos programas GTK+."
LangString GTK_NO_THEME_DESC				${LANG_PORTUGUESE} "N�o instalar um tema do GTK+"
LangString GTK_WIMP_THEME_DESC			${LANG_PORTUGUESE} "O tema 'GTK-Wimp' ('Windows impersonator', personificador do Windows) � um tema GTK que combina bem com o ambiente de trabalho do Windows."
LangString GTK_BLUECURVE_THEME_DESC			${LANG_PORTUGUESE} "O tema 'Bluecurve'."
LangString GTK_LIGHTHOUSEBLUE_THEME_DESC		${LANG_PORTUGUESE} "O tema 'Lighthouseblue'."

; GTK+ Directory Page
LangString GTK_UPGRADE_PROMPT				${LANG_PORTUGUESE} "Foi encontrada ma vers�o antiga do ambiente de tempo de execu��o do GTK+. Deseja actualiz�-lo?$\rNota: O Gaim poder� n�o funcionar se n�o o fizer."

; Installer Finish Page
LangString GAIM_FINISH_VISIT_WEB_SITE		${LANG_PORTUGUESE} "Visite a p�gina web do Gaim para Windows" 

; Gaim Section Prompts and Texts
LangString GAIM_UNINSTALL_DESC			${LANG_PORTUGUESE} "Gaim (remover apenas)"
LangString GAIM_PROMPT_WIPEOUT			${LANG_PORTUGUESE} "A sua antiga instala��o do Gaim est� prestes a ser removida. Deseja continuar?$\r$\rNota: Quaisquer plugins n�o-padr�o que poder� ter instalado ser�o removidos.$\rAs configura��es de utilizador do Gaim n�o ser�o afectadas."
LangString GAIM_PROMPT_DIR_EXISTS			${LANG_PORTUGUESE} "A directoria de instala��o do que especificou j� existe. Qualquer conte�do$\rser� apagado. Deseja continuar?"

; GTK+ Section Prompts
LangString GTK_INSTALL_ERROR				${LANG_PORTUGUESE} "Erro ao instalar o ambiente de tempo de execu��o do GTK+."
LangString GTK_BAD_INSTALL_PATH			${LANG_PORTUGUESE} "Imposs�vel aceder ou criar o caminho que digitou."

; GTK+ Themes section
LangString GTK_NO_THEME_INSTALL_RIGHTS		${LANG_PORTUGUESE} "N�o tem permiss�o para instalar um tema do GTK+."

; Uninstall Section Prompts
LangString un.GAIM_UNINSTALL_ERROR_1         	${LANG_PORTUGUESE} "O desinstalador n�o p�de encontrar entradas de registo do Gaim.$\r� prov�vel que outro utilizador tenha instalado este programa."
LangString un.GAIM_UNINSTALL_ERROR_2         	${LANG_PORTUGUESE} "N�o tem permiss�o para desinstalar este programa."
