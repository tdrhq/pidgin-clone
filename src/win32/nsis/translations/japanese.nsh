;;
;;  japanese.nsh
;;
;;  Japanese language strings for the Windows Gaim NSIS installer.
;;  Windows Code page: 932
;;
;;  Author: "Takeshi Kurosawa" <t-kuro@abox23.so-net.ne.jp>
;;  Version 2
;;

; Startup GTK+ check
!define GTK_INSTALLER_NEEDED			"GTK+�����^�C�������������������̓A�b�v�O���[�h����K�v������܂��B$\rv${GTK_VERSION}�������͂���ȏ��GTK+�����^�C�����C���X�g�[�����Ă��������B"

; License Page
!define GAIM_LICENSE_BUTTON			"���� >"
!define GAIM_LICENSE_BOTTOM_TEXT		"$(^Name)��GPL���C�Z���X�̌��Ń����[�X����Ă��܂��B���C�Z���X�͂����ɎQ�l�̂��߂����ɒ񋟂���Ă��܂��B $_CLICK"

; Components Page
!define GAIM_SECTION_TITLE			"Gaim�C���X�^���g���b�Z���W�� (�K�{)"
!define GTK_SECTION_TITLE			"GTK+ Runtime Environment (�K�{)"
!define GTK_THEMES_SECTION_TITLE		"GTK+�̃e�[�}"
!define GTK_NOTHEME_SECTION_TITLE		"�e�[�}�Ȃ�"
!define GTK_WIMP_SECTION_TITLE		"Wimp�e�[�}"
!define GTK_BLUECURVE_SECTION_TITLE		"Bluecurve�e�[�}"
!define GTK_LIGHTHOUSEBLUE_SECTION_TITLE	"Light House Blue�e�[�}"
!define GAIM_SECTION_DESCRIPTION		"Gaim�̊j�ƂȂ�t�@�C����dll"
!define GTK_SECTION_DESCRIPTION		"Gaim�̎g���Ă���}���`�v���b�g�t�H�[��GUI�c�[���L�b�g"
!define GTK_THEMES_SECTION_DESCRIPTION	"GTK+�̃e�[�}�́AGTK+�̃A�v���P�[�V�����̃��b�N���t�B�[����ς����܂��B"
!define GTK_NO_THEME_DESC			"GTK+�̃e�[�}���C���X�g�[�����Ȃ�"
!define GTK_WIMP_THEME_DESC			"GTK-Wimp (Windows impersonator)��Windows�̃f�X�N�g�b�v���Ƃ悭���a�����e�[�}�ł��B"
!define GTK_BLUECURVE_THEME_DESC		"Bluecurve�e�[�}�B"
!define GTK_LIGHTHOUSEBLUE_THEME_DESC	"Lighthouseblue�e�[�}�B"

; GTK+ Directory Page
!define GTK_UPGRADE_PROMPT			"�Â��o�[�W������GTK+�����^�C����������܂����B�A�b�v�O���[�h���܂���?$\r����: Gaim�̓A�b�v�O���[�h���Ȃ����蓮���Ȃ��ł��傤�B"

; Installer Finish Page
!define GAIM_FINISH_VISIT_WEB_SITE		"Windows Gaim��Web�y�[�W��K��Ă��������B"

; Gaim Section Prompts and Texts
!define GAIM_UNINSTALL_DESC			"Gaim (�폜�̂�)"
!define GAIM_PROMPT_WIPEOUT			"�Â�Gaim�̃t�H���_�̍폜�Ɋւ��āB���s���܂���?$\r$\r����: ���Ȃ��̃C���X�g�[���������ׂĂ̔�W���ȃv���O�C���͍폜����܂��B$\rGaim�̐ݒ�͉e�����󂯂܂���B"
!define GAIM_PROMPT_DIR_EXISTS		"���Ȃ��̎w�肵���C���X�g�[����̃t�H���_�͂��łɑ��݂��Ă��܂��B���e�͂��ׂ�$\r�폜����܂��B���s���܂���?"

; GTK+ Section Prompts
!define GTK_INSTALL_ERROR			"GTK+�����^�C���̃C���X�g�[���ŃG���[���������܂����B"
!define GTK_BAD_INSTALL_PATH			"���Ȃ��̓��͂����p�X�ɃA�N�Z�X�܂��͍쐬�ł��܂���B"

; GTK+ Themes section
!define GTK_NO_THEME_INSTALL_RIGHTS		"���Ȃ���GTK+�̃e�[�}���C���X�g�[�����錠���������Ă��܂���B"

; Uninstall Section Prompts
!define un.GAIM_UNINSTALL_ERROR_1		"�A���C���X�g�[����Gaim�̃��W�X�g���G���g���𔭌��ł��܂���ł����B$\r�����炭�ʂ̃��[�U�ɃC���X�g�[�����ꂽ�ł��傤�B"
!define un.GAIM_UNINSTALL_ERROR_2		"���Ȃ��͂��̃A�v���P�[�V�������A���C���X�g�[�����錠���������Ă��܂���B"
