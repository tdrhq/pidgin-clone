;;
;;  brazilian.nsh
;;
;;  Portuguese (Brazilian) language strings for the Windows Gaim NSIS installer.
;;  Windows Code page: 1252
;;
;;  Author: Maur�cio de Lemos Rodrigues Collares Neto <mauricioc@myrealbox.com>, 2003.
;;

; Startup GTK+ check
LangString GTK_INSTALLER_NEEDED			${LANG_BRAZILIAN} "O ambiente de tempo de execu��o do GTK+ est� ausente ou precisa ser atualizado.$\rFavor instalar a vers�o v${GTK_VERSION} ou superior do ambiente de tempo de execu��o do GTK+."

; Components Page
LangString GAIM_SECTION_TITLE				${LANG_BRAZILIAN} "Cliente de mensagens instant�neas Gaim (requerido)"
LangString GTK_SECTION_TITLE				${LANG_BRAZILIAN} "Ambiente de tempo de execu��o do GTK+ (requerido)"
LangString GTK_THEMES_SECTION_TITLE			${LANG_BRAZILIAN} "Temas do GTK+"
LangString GTK_NOTHEME_SECTION_TITLE		${LANG_BRAZILIAN} "Nenhum tema"
LangString GTK_WIMP_SECTION_TITLE			${LANG_BRAZILIAN} "Tema 'Wimp'"
LangString GTK_BLUECURVE_SECTION_TITLE		${LANG_BRAZILIAN} "Tema 'Bluecurve'"
LangString GTK_LIGHTHOUSEBLUE_SECTION_TITLE	${LANG_BRAZILIAN} "Tema 'Light House Blue'"
LangString GAIM_SECTION_DESCRIPTION			${LANG_BRAZILIAN} "Arquivos e bibliotecas principais do Gaim"
LangString GTK_SECTION_DESCRIPTION			${LANG_BRAZILIAN} "Um conjunto de ferramentas multi-plataforma para interface do usu�rio, usado pelo Gaim"
LangString GTK_THEMES_SECTION_DESCRIPTION		${LANG_BRAZILIAN} "Os temas do GTK+ podem mudar a apar�ncia e o funcionamento dos aplicativos GTK+."
LangString GTK_NO_THEME_DESC				${LANG_BRAZILIAN} "N�o instalar um tema do GTK+"
LangString GTK_WIMP_THEME_DESC			${LANG_BRAZILIAN} "O tema 'GTK-Wimp' ('Windows impersonator', personificador do Windows) � um tema GTK que combina bem com o ambiente de �rea de trabalho do Windows."
LangString GTK_BLUECURVE_THEME_DESC			${LANG_BRAZILIAN} "O tema 'Bluecurve'."
LangString GTK_LIGHTHOUSEBLUE_THEME_DESC		${LANG_BRAZILIAN} "O tema 'Lighthouseblue'."

; Extra GTK+ Dir Selector Page
LangString GTK_PAGE_TITLE				${LANG_BRAZILIAN} "Escolha o local da instala��o"
LangString GTK_PAGE_SUBTITLE				${LANG_BRAZILIAN} "Escolha a pasta em que o GTK+ ser� instalado"
LangString GTK_PAGE_INSTALL_MSG1			${LANG_BRAZILIAN} "O instalador ir� instalar o GTK+ na seguinte pasta"
LangString GTK_PAGE_INSTALL_MSG2			${LANG_BRAZILIAN} "Para instalar numa pasta diferente, clique em Procurar e selecione outra pasta. Aperte Pr�ximo para continuar."
LangString GTK_PAGE_UPGRADE_MSG1			${LANG_BRAZILIAN} "O instalador ir� atualizar o GTK+ encontrado na seguinte pasta"
LangString GTK_UPGRADE_PROMPT				${LANG_BRAZILIAN} "Uma vers�o antiga do ambiente de tempo de execu��o do GTK+ foi encontrada. Voc� deseja atualiz�-lo?$\rNota: O Gaim poder� n�o funcionar a menos que voc� o fa�a."

; Gaim Section Prompts and Texts
LangString GAIM_UNINSTALL_DESC			${LANG_BRAZILIAN} "Gaim (apenas remover)"
LangString GAIM_PROMPT_WIPEOUT			${LANG_BRAZILIAN} "Sua antiga instala��o do Gaim est� prestes a ser removida. Voc� gostaria de continuar?$\r$\rNota: Quaisquer plugins n�o-padr�o que voc� pode ter instalado ser�o removidos.$\rAs configura��es de usu�rio do Gaim n�o ser�o afetadas."
LangString GAIM_PROMPT_DIR_EXISTS			${LANG_BRAZILIAN} "O diret�rio de instala��o do que voc� especificou j� existe. Qualquer conte�do$\rser� deletado. Deseja continuar?"

; GTK+ Section Prompts
LangString GTK_INSTALL_ERROR				${LANG_BRAZILIAN} "Erro ao instalar o ambiente de tempo de execu��o do GTK+."
LangString GTK_BAD_INSTALL_PATH			${LANG_BRAZILIAN} "O caminho que voc� digitou n�o p�de ser acessado ou criado."

; GTK+ Themes section
LangString GTK_NO_THEME_INSTALL_RIGHTS		${LANG_BRAZILIAN} "Voc� n�o tem permiss�o para instalar um tema do GTK+."

; Uninstall Section Prompts
LangString un.GAIM_UNINSTALL_ERROR_1         	${LANG_BRAZILIAN} "O desinstalador n�o p�de encontrar entradas de registro do Gaim.$\r� prov�vel que outro usu�rio tenha instalado esta aplica��o."
LangString un.GAIM_UNINSTALL_ERROR_2         	${LANG_BRAZILIAN} "Voc� n�o tem permiss�o para desinstalar essa aplica��o."
