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
!define GTK_INSTALLER_NEEDED			"Les biblioth�ques de l'environnement GTK+ ne sont pas install�es ou n�cessitent une mise � jour.$\rVeuillez installer les biblioth�ques GTK+ v${GTK_VERSION} ou plus r�centes."

; License Page
!define GAIM_LICENSE_BUTTON			"Suivant >"
!define GAIM_LICENSE_BOTTOM_TEXT		"$(^Name) est disponible sous licence GPL. Le texte de licence suivant est fourni uniquement � titre informatif. $_CLICK" 

; Components Page
!define GAIM_SECTION_TITLE			"Gaim messagerie instantan�e (obligatoire)"
!define GTK_SECTION_TITLE			"Biblioth�ques GTK+ (obligatoire)"
!define GTK_THEMES_SECTION_TITLE		"Th�mes GTK+"
!define GTK_NOTHEME_SECTION_TITLE		"Pas de th�me"
!define GTK_WIMP_SECTION_TITLE		"Th�me Wimp"
!define GTK_BLUECURVE_SECTION_TITLE		"Th�me Bluecurve"
!define GTK_LIGHTHOUSEBLUE_SECTION_TITLE	"Th�me Light House Blue"
!define GAIM_SECTION_DESCRIPTION		"Fichiers et DLLs de base de Gaim"
!define GTK_SECTION_DESCRIPTION		"A multi-platform GUI toolkit, used by Gaim"
!define GTK_THEMES_SECTION_DESCRIPTION	"Les th�mes GTK+ permettent de changer l'aspect des applications GTK+."
!define GTK_NO_THEME_DESC			"Ne pas installer de th�me GTK+"
!define GTK_WIMP_THEME_DESC			"GTK-Wimp (imitateur de Windows) est un th�me de GTK+ qui se fond dans l'environnement graphique de Windows."
!define GTK_BLUECURVE_THEME_DESC		"Th�me Bluecurve"
!define GTK_LIGHTHOUSEBLUE_THEME_DESC	"Th�me Lighthouseblue"

; GTK+ Dir Selector Page
!define GTK_UPGRADE_PROMPT			"Une ancienne version des biblioth�ques GTK+ a �t� trouv�e. Voulez-vous la mettre � jour ?$\rNote : Gaim peut ne pas fonctionner sans cela."

; Installer Finish Page
!define GAIM_FINISH_VISIT_WEB_SITE		"Visitez la page web de Gaim Windows" 

; Gaim Section Prompts and Texts
!define GAIM_UNINSTALL_DESC			"Gaim (supprimer uniquement)"
!define GAIM_PROMPT_WIPEOUT			"L'ancien r�pertoire de Gaim va �tre supprim�. Voulez-vous continuer ?$\r$\rNote : Tous les plugins non standards que vous avez install�s seront aussi supprim�s.$\rLes configurations des utilisateurs de Gaim ne sont pas touch�s."
!define GAIM_PROMPT_DIR_EXISTS		"Le r�pertoire d'installation existe d�j�. Son contenu sera effac�.$\rVoulez-vous continuer ?"

; GTK+ Section Prompts
!define GTK_INSTALL_ERROR			"Erreur lors de l'installation des biblioth�ques GTK+"
!define GTK_BAD_INSTALL_PATH			"Le r�pertoire d'installation ne peut pas �tre cr�� ou n'est pas accessible."

; GTK+ Themes section
!define GTK_NO_THEME_INSTALL_RIGHTS		"Vous n'avez pas les permissions pour installer un th�me GTK+"

; Uninstall Section Prompts
!define un.GAIM_UNINSTALL_ERROR_1         "Les clefs de Gaim n'ont pas �t� trouv�es dans la base de registres.$\rL'application a peut-�tre �t� install�e par un utilisateur diff�rent."
!define un.GAIM_UNINSTALL_ERROR_2         "Vous n'avez pas les permissions pour supprimer cette application."

