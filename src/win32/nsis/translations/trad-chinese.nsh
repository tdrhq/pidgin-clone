;;
;;  trad-chinese.nsh
;;
;;  Traditional Chineese language strings for the Windows Gaim NSIS installer.
;;  Windows Code page:950 
;;
;;  Author: Paladin R. Liu <paladin@ms1.hinet.net>
;;  Minor updates: Ambrose C. Li <acli@ada.dhs.org>
;;

; Startup GTK+ check
!define GTK_INSTALLER_NEEDED			"�䤣��ŦX�� GTK+ �������ҡC$\r�Цw�� v${GTK_VERSION} �H�W������ GTK+ �������ҡC"

; License Page
!define GAIM_LICENSE_BUTTON			"�U�@�B >"
!define GAIM_LICENSE_BOTTOM_TEXT		"$(^Name) �Y�H GPL �覡���v�o�G�A�b���C�X���v�ѡA���c�Y�@���ѦҤ��ΡC$_CLICK"

; Components Page
!define GAIM_SECTION_TITLE			"Gaim �D�{�� (����)"
!define GTK_SECTION_TITLE			"GTK+ �������� (����)"
!define GTK_THEMES_SECTION_TITLE		"GTK+ �G���D�D"
!define GTK_NOTHEME_SECTION_TITLE		"���w��"
!define GTK_WIMP_SECTION_TITLE		"Wimp �D�D"
!define GTK_BLUECURVE_SECTION_TITLE		"Bluecurve �D�D"
!define GTK_LIGHTHOUSEBLUE_SECTION_TITLE	"Light House Blue �D�D"
!define GAIM_SECTION_DESCRIPTION		"Gaim �֤��ɮפΰʺA�禡�w"
!define GTK_SECTION_DESCRIPTION		"Gaim �ҨϥΪ��󥭥x�ϧΤ����禡�w"
!define GTK_THEMES_SECTION_DESCRIPTION	"GTK+ �G���D�D�i�H�Ψӧ��� GTK+ ���ε{�����~�[�C"
!define GTK_NO_THEME_DESC			"���w�� GTK+ �G���D�D"
!define GTK_WIMP_THEME_DESC			"�uGTK-Wimp�v(Windows impersonator) �D�D�i�� GTK �ĤJ Windows �������������C"
!define GTK_BLUECURVE_THEME_DESC		"�uBluecurve�v�D�D"
!define GTK_LIGHTHOUSEBLUE_THEME_DESC	"�uLighthouseblue�v�D�D�C"

; GTK+ Directory Page
!define GTK_UPGRADE_PROMPT			"�o�{�@���ª��� GTK+ �������ҡC�z�n�N���ɯŶܡH$\r�Ъ`�N�G�p�G�z���ɯšAGaim �i��L�k���T���Q����C"

; Gaim Section Prompts and Texts
!define GAIM_UNINSTALL_DESC			"Gaim v${GAIM_VERSION} (�u�Ѳ���)"
!define GAIM_PROMPT_WIPEOUT			"�z���e�w�˩�ؿ������ª� Gaim �N�|�Q�����C�z�n�~��ܡH$\r$\r�Ъ`�N�G����z�Ҧw�˪��D�x����@�Ҳճ��N�Q�R���C$\r�� Gaim ���ϥΪ̳]�w�N���|����v�T�C"
!define GAIM_PROMPT_DIR_EXISTS		"�z�ҿ�w���w�˥ؿ��U���Ҧ��ɮ׳��N�Q�����C$\r�z�n�~��ܡH"

; GTK+ Section Prompts
!define GTK_INSTALL_ERROR			"�w�� GTK+ �������Үɵo�Ϳ��~�C"
!define GTK_BAD_INSTALL_PATH			"�z�ҿ�J���w�˥ؿ��L�k�s���ΫإߡC"

; GTK+ Themes section
!define GTK_NO_THEME_INSTALL_RIGHTS		"�z�ثe���v���L�k�w�� GTK+ �G���D�D�C"

; Uninstall Section Prompts
!define un.GAIM_UNINSTALL_ERROR_1         "�����{���L�k��� Gaim ���w�˸�T�C$\r�o���ӬO����L���ϥΪ̭��s�w�ˤF�o�ӵ{���C"
!define un.GAIM_UNINSTALL_ERROR_2         "�z�ثe���v���L�k���� Gaim�C"
