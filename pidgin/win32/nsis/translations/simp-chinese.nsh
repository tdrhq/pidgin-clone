;;  vim:syn=winbatch:encoding=8bit-cp936:fileencoding=8bit-cp936:
;;  simp-chinese.nsh
;;
;;  Simplified Chinese language strings for the Windows Pidgin NSIS installer.
;;  Windows Code page: 936
;;
;;  Author: Funda Wang" <fundawang@linux.net.cn>
;;  Version 2
;;

; Startup GTK+ check
!define INSTALLER_IS_RUNNING			"安装程序已经运行。"
!define PIDGIN_IS_RUNNING			"Pidgin 的实例正在运行中。请退出 Pidgin 然后再试一次。"
!define GTK_INSTALLER_NEEDED			"可能缺少 GTK+ 运行时刻环境，或者需要更新该环境。$\r请安装 v${GTK_MIN_VERSION} 或更高版本的 GTK+ 运行时刻环境"

; License Page
!define PIDGIN_LICENSE_BUTTON			"下一步 >"
!define PIDGIN_LICENSE_BOTTOM_TEXT		"$(^Name) 以 GPL 许可发布。在此提供此许可仅为参考。$_CLICK"

; Components Page
!define PIDGIN_SECTION_TITLE			"Pidgin 即时通讯程序(必需)"
!define GTK_SECTION_TITLE			"GTK+ 运行时刻环境(必需)"
!define PIDGIN_SHORTCUTS_SECTION_TITLE "快捷方式"
!define PIDGIN_DESKTOP_SHORTCUT_SECTION_TITLE "桌面"
!define PIDGIN_STARTMENU_SHORTCUT_SECTION_TITLE "开始菜单"
!define PIDGIN_SECTION_DESCRIPTION		"Pidgin 核心文件和 DLLs"
!define GTK_SECTION_DESCRIPTION		"Pidgin 所用的多平台 GUI 工具包"

!define PIDGIN_SHORTCUTS_SECTION_DESCRIPTION   "启动 Pidgin 的快捷方式"
!define PIDGIN_DESKTOP_SHORTCUT_DESC   "在桌面上创建 Pidgin 的快捷方式"
!define PIDGIN_STARTMENU_SHORTCUT_DESC   "在开始菜单中创建 Pidgin 的快捷方式"

; GTK+ Directory Page
!define GTK_UPGRADE_PROMPT			"发现了旧版本的 GTK+ 运行时刻。您想要升级吗?$\r注意: 除非您进行升级，否则 $(^Name) 可能无法工作。"

; Finish Page
!define PIDGIN_FINISH_VISIT_WEB_SITE		"浏览 Windows Pidgin 网页"

; GTK+ Section Prompts
!define GTK_INSTALL_ERROR			"安装 GTK+ 运行时刻失败。"
!define GTK_BAD_INSTALL_PATH			"无法访问或创建您输入的路径。"

; Uninstall Section Prompts
!define un.PIDGIN_UNINSTALL_ERROR_1         "卸载程序找不到 Pidgin 的注册表项目。$\r可能是另外的用户安装了此程序。"
!define un.PIDGIN_UNINSTALL_ERROR_2         "您没有权限卸载此程序。"

; Spellcheck Section Prompts
!define PIDGIN_SPELLCHECK_SECTION_TITLE		"拼写检查支持"
!define PIDGIN_SPELLCHECK_ERROR			"安装拼写检查出错"
!define PIDGIN_SPELLCHECK_DICT_ERROR		"安装拼写检查字典出错"
!define PIDGIN_SPELLCHECK_SECTION_DESCRIPTION	"拼写检查支持。(安装需要连接到 Internet)"
!define ASPELL_INSTALL_FAILED			"安装失败"
!define PIDGIN_SPELLCHECK_BRETON			"布里多尼语"
!define PIDGIN_SPELLCHECK_CATALAN			"加泰罗尼亚语"
!define PIDGIN_SPELLCHECK_CZECH			"捷克语"
!define PIDGIN_SPELLCHECK_WELSH			"威尔士语"
!define PIDGIN_SPELLCHECK_DANISH			"丹麦语"
!define PIDGIN_SPELLCHECK_GERMAN			"德语"
!define PIDGIN_SPELLCHECK_GREEK			"希腊语"
!define PIDGIN_SPELLCHECK_ENGLISH			"英语"
!define PIDGIN_SPELLCHECK_ESPERANTO		"世界语"
!define PIDGIN_SPELLCHECK_SPANISH			"西班牙语"
!define PIDGIN_SPELLCHECK_FAROESE			"法罗语"
!define PIDGIN_SPELLCHECK_FRENCH			"法语"
!define PIDGIN_SPELLCHECK_ITALIAN			"意大利语"
!define PIDGIN_SPELLCHECK_DUTCH			"荷兰语"
!define PIDGIN_SPELLCHECK_NORWEGIAN		"挪威语"
!define PIDGIN_SPELLCHECK_POLISH			"波兰语"
!define PIDGIN_SPELLCHECK_PORTUGUESE		"葡萄牙语"
!define PIDGIN_SPELLCHECK_ROMANIAN			"罗马尼亚语"
!define PIDGIN_SPELLCHECK_RUSSIAN			"俄语"
!define PIDGIN_SPELLCHECK_SLOVAK			"斯洛伐克语"
!define PIDGIN_SPELLCHECK_SWEDISH			"瑞典语"
!define PIDGIN_SPELLCHECK_UKRAINIAN		"乌克兰语"
