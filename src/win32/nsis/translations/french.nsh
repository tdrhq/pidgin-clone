;;
;;  french.nsh
;;
;;  French language strings for the Windows Gaim NSIS installer.
;;  Windows Code page: 1252
;;
;;  Author: Eric Boumaour <zongo@nekeme.net>, 2003.
;;  Version 2
;;

; Startup GTK+ check
LangString GTK_INSTALLER_NEEDED			${LANG_FRENCH} "Les biblioth�ques de l'environnement GTK+ ne sont pas install�es ou n�cessitent une mise � jour.$\rVeuillez installer les biblioth�ques GTK+ v${GTK_VERSION} ou plus r�centes."

; License Page
LangString GAIM_LICENSE_BUTTON			${LANG_FRENCH} "Suivant >"
LangString GAIM_LICENSE_BOTTOM_TEXT			${LANG_FRENCH} "$(^Name) est disponible sous licence GPL. Le texte de licence suivant est fourni uniquement � titre informatif. $_CLICK" 

; Components Page
LangString GAIM_SECTION_TITLE				${LANG_FRENCH} "Gaim messagerie instantan�e (obligatoire)"
LangString GTK_SECTION_TITLE				${LANG_FRENCH} "Biblioth�ques GTK+ (obligatoire)"
LangString GTK_THEMES_SECTION_TITLE			${LANG_FRENCH} "Th�mes GTK+"
LangString GTK_NOTHEME_SECTION_TITLE		${LANG_FRENCH} "Pas de th�me"
LangString GTK_WIMP_SECTION_TITLE			${LANG_FRENCH} "Th�me Wimp"
LangString GTK_BLUECURVE_SECTION_TITLE		${LANG_FRENCH} "Th�me Bluecurve"
LangString GTK_LIGHTHOUSEBLUE_SECTION_TITLE	${LANG_FRENCH} "Th�me Light House Blue"
LangString GAIM_SECTION_DESCRIPTION			${LANG_FRENCH} "Fichiers et DLLs de base de Gaim"
LangString GTK_SECTION_DESCRIPTION			${LANG_FRENCH} "A multi-platform GUI toolkit, used by Gaim"
LangString GTK_THEMES_SECTION_DESCRIPTION		${LANG_FRENCH} "Les th�mes GTK+ permettent de changer l'aspect des applications GTK+."
LangString GTK_NO_THEME_DESC				${LANG_FRENCH} "Ne pas installer de th�me GTK+"
LangString GTK_WIMP_THEME_DESC			${LANG_FRENCH} "GTK-Wimp (imitateur de Windows) est un th�me de GTK+ qui se fond dans l'environnement graphique de Windows."
LangString GTK_BLUECURVE_THEME_DESC			${LANG_FRENCH} "Th�me Bluecurve"
LangString GTK_LIGHTHOUSEBLUE_THEME_DESC		${LANG_FRENCH} "Th�me Lighthouseblue"

; GTK+ Dir Selector Page
LangString GTK_UPGRADE_PROMPT				${LANG_FRENCH} "Une ancienne version des biblioth�ques GTK+ a �t� trouv�e. Voulez-vous la mettre � jour ?$\rNote : Gaim peut ne pas fonctionner sans cela."

; Installer Finish Page
LangString GAIM_FINISH_VISIT_WEB_SITE		${LANG_FRENCH} "Visitez la page web de Gaim Windows" 

; Gaim Section Prompts and Texts
LangString GAIM_UNINSTALL_DESC			${LANG_FRENCH} "Gaim (supprimer uniquement)"
LangString GAIM_PROMPT_WIPEOUT			${LANG_FRENCH} "L'ancien r�pertoire de Gaim va �tre supprim�. Voulez-vous continuer ?$\r$\rNote : Tous les plugins non standards que vous avez install�s seront aussi supprim�s.$\rLes configurations des utilisateurs de Gaim ne sont pas touch�s."
LangString GAIM_PROMPT_DIR_EXISTS			${LANG_FRENCH} "Le r�pertoire d'installation existe d�j�. Son contenu sera effac�.$\rVoulez-vous continuer ?"

; GTK+ Section Prompts
LangString GTK_INSTALL_ERROR				${LANG_FRENCH} "Erreur lors de l'installation des biblioth�ques GTK+"
LangString GTK_BAD_INSTALL_PATH			${LANG_FRENCH} "Le r�pertoire d'installation ne peut pas �tre cr�� ou n'est pas accessible."

; GTK+ Themes section
LangString GTK_NO_THEME_INSTALL_RIGHTS		${LANG_FRENCH} "Vous n'avez pas les permissions pour installer un th�me GTK+"

; Uninstall Section Prompts
LangString un.GAIM_UNINSTALL_ERROR_1         	${LANG_FRENCH} "Les clefs de Gaim n'ont pas �t� trouv�es dans la base de registres.$\rL'application a peut-�tre �t� install�e par un utilisateur diff�rent."
LangString un.GAIM_UNINSTALL_ERROR_2         	${LANG_FRENCH} "Vous n'avez pas les permissions pour supprimer cette application."

