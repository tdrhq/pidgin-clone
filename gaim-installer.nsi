; Installer script for win32 Gaim
; Herman Bloggs <hermanator12002@yahoo.com>

; NOTE: this .NSI script is designed for NSIS v2.0b4+

;--------------------------------
;Configuration

;General
!ifdef WITH_GTK
OutFile "gaim-${GAIM_VERSION}.exe"
!else
!ifdef DEBUG
OutFile "gaim-${GAIM_VERSION}-debug.exe"
!else
OutFile "gaim-${GAIM_VERSION}-no-gtk.exe"
!endif
!endif
SetCompressor bzip2

DirShow show
ShowInstDetails show
ShowUninstDetails show
SetDateSave on

; $INSTDIR is set in .onInit function..

!include "MUI.nsh"
!include Sections.nsh

;--------------------------------
;Defines

!define MUI_PRODUCT			"Gaim"
!define MUI_VERSION			${GAIM_VERSION}

!define MUI_ICON			.\pixmaps\gaim-install.ico
!define MUI_UNICON			.\pixmaps\gaim-install.ico
!define MUI_SPECIALBITMAP		.\src\win32\nsis\gaim-intro.bmp
!define MUI_HEADERBITMAP		.\src\win32\nsis\gaim-header.bmp

!define GAIM_NSIS_INCLUDE_PATH		".\src\win32\nsis"
!define GAIM_INSTALLER_DEPS		"..\win32-dev\gaim-inst-deps"

!define GAIM_REG_KEY			"SOFTWARE\gaim"
!define GAIM_UNINSTALL_KEY		"SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Gaim"
!define HKLM_APP_PATHS_KEY		"SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\gaim.exe"
!define GAIM_STARTUP_RUN_KEY		"SOFTWARE\Microsoft\Windows\CurrentVersion\Run"
!define GAIM_UNINST_EXE			"gaim-uninst.exe"

!define GTK_VERSION			"2.2.2"
!define GTK_REG_KEY			"SOFTWARE\GTK\2.0"
!define PERL_REG_KEY			"SOFTWARE\Perl"
!define PERL_DLL			"perl58.dll"
!define GTK_DEFAULT_INSTALL_PATH	"$PROGRAMFILES\Common Files\GTK\2.0"
!define GTK_RUNTIME_INSTALLER		"..\gtk_installer\gtk-runtime*.exe"
!define GTK_THEME_DIR			"..\gtk_installer\gtk_themes"
!define GTK_DEFAULT_THEME_GTKRC_DIR	"share\themes\Default\gtk-2.0"
!define GTK_DEFAULT_THEME_ENGINE_DIR	"lib\gtk-2.0\2.2.0\engines"

;--------------------------------
;Pages
  
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE
  !insertmacro MUI_PAGE_COMPONENTS
  Page custom ShowGtkInstallDirChooser GtkInstallDirVerify
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Modern UI Configuration

  !define MUI_COMPONENTSPAGE_SMALLDESC
  !define MUI_ABORTWARNING

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  !define MUI_LANGDLL_REGISTRY_KEY ${GAIM_REG_KEY}
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"
  !define MUI_LANGDLL_ALWAYSSHOW

;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

  !insertmacro MUI_LANGUAGE "Bulgarian"
  !insertmacro MUI_LANGUAGE "SimpChinese"
  !insertmacro MUI_LANGUAGE "TradChinese"
  !insertmacro MUI_LANGUAGE "German"
  !insertmacro MUI_LANGUAGE "French"
  !insertmacro MUI_LANGUAGE "Italian"
  !insertmacro MUI_LANGUAGE "Korean"
  !insertmacro MUI_LANGUAGE "Hungarian"
  !insertmacro MUI_LANGUAGE "Dutch"
  !insertmacro MUI_LANGUAGE "PortugueseBR"
  !insertmacro MUI_LANGUAGE "Portuguese"
  !insertmacro MUI_LANGUAGE "Romanian"
  !insertmacro MUI_LANGUAGE "Serbian"
  !insertmacro MUI_LANGUAGE "Swedish"

;--------------------------------
;Translations

  !include "${GAIM_NSIS_INCLUDE_PATH}\translations\english.nsh"
  !include "${GAIM_NSIS_INCLUDE_PATH}\translations\german.nsh"
  !include "${GAIM_NSIS_INCLUDE_PATH}\translations\dutch.nsh"
  !include "${GAIM_NSIS_INCLUDE_PATH}\translations\french.nsh"
  !include "${GAIM_NSIS_INCLUDE_PATH}\translations\simp-chinese.nsh"
  !include "${GAIM_NSIS_INCLUDE_PATH}\translations\serbian-latin.nsh"
  !include "${GAIM_NSIS_INCLUDE_PATH}\translations\portuguese-br.nsh"
  !include "${GAIM_NSIS_INCLUDE_PATH}\translations\portuguese.nsh"
  !include "${GAIM_NSIS_INCLUDE_PATH}\translations\hungarian.nsh"
  !include "${GAIM_NSIS_INCLUDE_PATH}\translations\italian.nsh"
  !include "${GAIM_NSIS_INCLUDE_PATH}\translations\bulgarian.nsh"
  !include "${GAIM_NSIS_INCLUDE_PATH}\translations\swedish.nsh"
  !include "${GAIM_NSIS_INCLUDE_PATH}\translations\trad-chinese.nsh"
  !include "${GAIM_NSIS_INCLUDE_PATH}\translations\korean.nsh"
  !include "${GAIM_NSIS_INCLUDE_PATH}\translations\romanian.nsh"

;--------------------------------
;Data
  
  LicenseData /LANG=${LANG_ENGLISH}		"./COPYING"
  LicenseData /LANG=${LANG_GERMAN}		"./COPYING"
  LicenseData /LANG=${LANG_DUTCH}		"./COPYING"
  LicenseData /LANG=${LANG_FRENCH}		"./COPYING"
  LicenseData /LANG=${LANG_SIMPCHINESE} 	"./COPYING"
  LicenseData /LANG=${LANG_SERBIAN}		"./COPYING"
  LicenseData /LANG=${LANG_PORTUGUESEBR}	"./COPYING"
  LicenseData /LANG=${LANG_HUNGARIAN}		"./COPYING"
  LicenseData /LANG=${LANG_ITALIAN}		"./COPYING"
  LicenseData /LANG=${LANG_BULGARIAN}		"./COPYING"
  LicenseData /LANG=${LANG_SWEDISH}		"./COPYING"
  LicenseData /LANG=${LANG_TRADCHINESE}		"./COPYING"
  LicenseData /LANG=${LANG_KOREAN}		"./COPYING"
  LicenseData /LANG=${LANG_ROMANIAN}		"./COPYING"
  LicenseData /LANG=${LANG_PORTUGUESE}		"./COPYING"

;--------------------------------
;Reserve Files
  ; Only need this if using bzip2 compression

  ReserveFile "${GAIM_NSIS_INCLUDE_PATH}\gtkInstall.ini"
  !insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
  !insertmacro MUI_RESERVEFILE_LANGDLL 
  ReserveFile "${NSISDIR}\Plugins\AccessControl.dll"
  ReserveFile "${NSISDIR}\Plugins\UserInfo.dll"



;--------------------------------
;Uninstall any old version of Gaim

Section -SecUninstallOldGaim
  ; Check install rights..
  Call CheckUserInstallRights
  Pop $R0

  StrCmp $R0 "HKLM" gaim_hklm
  StrCmp $R0 "HKCU" gaim_hkcu done

  gaim_hkcu:
      ReadRegStr $R1 HKCU ${GAIM_REG_KEY} ""
      ReadRegStr $R2 HKCU ${GAIM_REG_KEY} "Version"
      ReadRegStr $R3 HKCU "${GAIM_UNINSTALL_KEY}" "UninstallString"
      Goto try_uninstall

  gaim_hklm:
      ReadRegStr $R1 HKLM ${GAIM_REG_KEY} ""
      ReadRegStr $R2 HKLM ${GAIM_REG_KEY} "Version"
      ReadRegStr $R3 HKLM "${GAIM_UNINSTALL_KEY}" "UninstallString"

  ; If previous version exists .. remove
  try_uninstall:
    StrCmp $R1 "" done
      ; Version key started with 0.60a3. Prior versions can't be 
      ; automaticlly uninstalled.
      StrCmp $R2 "" uninstall_problem
        ; Check if we have uninstall string..
        IfFileExists $R3 0 uninstall_problem
          ; Have uninstall string.. go ahead and uninstall.
          SetOverwrite on
          ; Need to copy uninstaller outside of the install dir
          ClearErrors
          CopyFiles /SILENT $R3 "$TEMP\${GAIM_UNINST_EXE}"
          SetOverwrite off
          IfErrors uninstall_problem
            ; Ready to uninstall..
            ClearErrors
	    ExecWait '"$TEMP\${GAIM_UNINST_EXE}" /S _?=$R1'
	    IfErrors exec_error
              Delete "$TEMP\${GAIM_UNINST_EXE}"
	      Goto done

	    exec_error:
              Delete "$TEMP\${GAIM_UNINST_EXE}"
              Goto uninstall_problem

        uninstall_problem:
	  ; In this case just wipe out previous Gaim install dir..
	  ; We get here because versions 0.60a1 and 0.60a2 don't have versions set in the registry
	  ; and versions 0.60 and lower did not correctly set the uninstall reg string 
	  ; (the string was set in quotes)
          MessageBox MB_YESNO $(GAIM_PROMPT_WIPEOUT) IDYES do_wipeout IDNO cancel_install
          cancel_install:
            Quit

          do_wipeout:
            StrCmp $R0 "HKLM" gaim_del_lm_reg gaim_del_cu_reg
            gaim_del_cu_reg:
              DeleteRegKey HKCU ${GAIM_REG_KEY}
              Goto uninstall_prob_cont
            gaim_del_lm_reg:
              DeleteRegKey HKLM ${GAIM_REG_KEY}

            uninstall_prob_cont:
	      RMDir /r "$R1"

  done:
SectionEnd


;--------------------------------
;GTK+ Runtime Install Section

!ifdef WITH_GTK
Section $(GTK_SECTION_TITLE) SecGtk
  SectionIn 1 RO

  Call CheckUserInstallRights
  Pop $R1

  SetOutPath $TEMP
  SetOverwrite on
  File /oname=gtk-runtime.exe ${GTK_RUNTIME_INSTALLER}
  SetOverwrite off

  ; This keeps track whether we install GTK+ or not..
  StrCpy $R5 "0"

  Call DoWeNeedGtk
  Pop $R0
  Pop $R6

  ; Pass on the language we are using to GTK+ installer via registry..
  WriteRegStr HKCU "${GTK_REG_KEY}" "Installer Language" "$LANGUAGE"

  StrCmp $R0 "0" have_gtk
  StrCmp $R0 "1" upgrade_gtk
  StrCmp $R0 "2" no_gtk no_gtk

  no_gtk:
    StrCmp $R1 "NONE" gtk_no_install_rights
    !insertmacro MUI_INSTALLOPTIONS_READ $R2 "gtkInstall.ini" "Field 4" "State"
    ClearErrors
    ExecWait '"$TEMP\gtk-runtime.exe" /S /D=$R2'
    Goto gtk_install_cont

  upgrade_gtk:
    MessageBox MB_YESNO $(GTK_UPGRADE_PROMPT) IDNO done
    ClearErrors
    ExecWait '"$TEMP\gtk-runtime.exe" /S'
    Goto gtk_install_cont

  gtk_install_cont:
    IfErrors gtk_install_error
      StrCpy $R5 "1"  ; marker that says we installed...
      Goto done

    gtk_install_error:
      Delete "$TEMP\gtk-runtime.exe"
      MessageBox MB_OK $(GTK_INSTALL_ERROR) IDOK
      Quit

  have_gtk:
    StrCpy $R2 $R6 ; Copy GTK+ path
    StrCmp $R1 "NONE" done ; If we have no rights.. can't re-install..
    ; Even if we have a sufficient version of GTK+, we give user choice to re-install.
    ClearErrors
    ExecWait '"$TEMP\gtk-runtime.exe" /S'
    IfErrors gtk_install_error
    Goto done

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ; end got_install rights

  gtk_no_install_rights:
    ClearErrors
    ExecWait '"$TEMP\gtk-runtime.exe" /S /D=$INSTDIR'
    IfErrors gtk_install_error
      SetOverwrite on
      ClearErrors
      CopyFiles /FILESONLY "$INSTDIR\bin\*.dll" $INSTDIR
      SetOverwrite off
      IfErrors gtk_install_error
        Delete "$INSTDIR\bin\*.dll"
        Goto done
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ; end gtk_no_install_rights

  done:
    Delete "$TEMP\gtk-runtime.exe"
SectionEnd ; end of GTK+ section
!endif

;--------------------------------
;Gaim Install Section

Section $(GAIM_SECTION_TITLE) SecGaim
  SectionIn 1 RO

  ; Check install rights..
  Call CheckUserInstallRights
  Pop $R0

  ; Get GTK+ lib dir if we have it..

  StrCmp $R0 "NONE" gaim_none
  StrCmp $R0 "HKLM" gaim_hklm gaim_hkcu

  gaim_hklm:
    ReadRegStr $R1 HKLM ${GTK_REG_KEY} "Path"
    WriteRegStr HKLM "${HKLM_APP_PATHS_KEY}" "" "$INSTDIR\gaim.exe"
    WriteRegStr HKLM "${HKLM_APP_PATHS_KEY}" "Path" "$R1\bin"
    WriteRegStr HKLM ${GAIM_REG_KEY} "" "$INSTDIR"
    WriteRegStr HKLM ${GAIM_REG_KEY} "Version" "${GAIM_VERSION}"
    WriteRegStr HKLM "${GAIM_UNINSTALL_KEY}" "DisplayName" $(GAIM_UNINSTALL_DESC)
    WriteRegStr HKLM "${GAIM_UNINSTALL_KEY}" "UninstallString" "$INSTDIR\${GAIM_UNINST_EXE}"
    ; Sets scope of the desktop and Start Menu entries for all users.
    SetShellVarContext "all"
    Goto gaim_install_files

  gaim_hkcu:
    ReadRegStr $R1 HKCU ${GTK_REG_KEY} "Path"
    StrCmp $R1 "" 0 gaim_hkcu1
      ReadRegStr $R1 HKLM ${GTK_REG_KEY} "Path"
    gaim_hkcu1:
    WriteRegStr HKCU ${GAIM_REG_KEY} "" "$INSTDIR"
    WriteRegStr HKCU ${GAIM_REG_KEY} "Version" "${GAIM_VERSION}"
    WriteRegStr HKCU "${GAIM_UNINSTALL_KEY}" "DisplayName" $(GAIM_UNINSTALL_DESC)
    WriteRegStr HKCU "${GAIM_UNINSTALL_KEY}" "UninstallString" "$INSTDIR\${GAIM_UNINST_EXE}"
    Goto gaim_install_files

  gaim_none:
    ReadRegStr $R1 HKLM ${GTK_REG_KEY} "Path"

  gaim_install_files:
    SetOutPath "$INSTDIR"
    ; Gaim files
    SetOverwrite on
    File /r .\win32-install-dir\*.*
    !ifdef DEBUG
    File "${GAIM_INSTALLER_DEPS}\exchndl.dll"
    !endif

    ; Install shfolder.dll if need be..
    SearchPath $R4 "shfolder.dll"
    StrCmp $R4 "" 0 got_shfolder
      SetOutPath "$SYSDIR"
      File "${GAIM_INSTALLER_DEPS}\shfolder.dll"
      SetOutPath "$INSTDIR"
    got_shfolder:

    ; Check if Perl is installed, If not remove perl plugin
    ReadRegStr $R2 HKLM ${PERL_REG_KEY} ""
    StrCmp $R2 "" 0 perl_exists
      ReadRegStr $R2 HKCU ${PERL_REG_KEY} ""
      StrCmp $R2 "" perl_remove perl_exists

      perl_remove:
        Delete "$INSTDIR\plugins\perl.dll"
        RMDir /r "$INSTDIR\perlmod"
        Goto perl_done
 
      perl_exists:
        IfFileExists "$R2\bin\${PERL_DLL}" 0 perl_remove
        StrCmp $R0 "HKLM" 0 perl_done
          ReadRegStr $R3 HKLM "${HKLM_APP_PATHS_KEY}" "Path"
          WriteRegStr HKLM "${HKLM_APP_PATHS_KEY}" "Path" "$R3;$R2\bin"

    perl_done:

    CreateDirectory "$SMPROGRAMS\Gaim"
    CreateShortCut "$SMPROGRAMS\Gaim\Gaim.lnk" "$INSTDIR\gaim.exe"
    CreateShortCut "$DESKTOP\Gaim.lnk" "$INSTDIR\gaim.exe"
    SetOutPath "$INSTDIR"

    ; If we don't have install rights.. we're done
    StrCmp $R0 "NONE" done
    CreateShortCut "$SMPROGRAMS\Gaim\Uninstall.lnk" "$INSTDIR\${GAIM_UNINST_EXE}"
    SetOverwrite off

    ; write out uninstaller
    SetOverwrite on
    WriteUninstaller "$INSTDIR\${GAIM_UNINST_EXE}"
    SetOverwrite off

  done:
SectionEnd ; end of default Gaim section

;--------------------------------
;GTK+ Themes

SubSection /e $(GTK_THEMES_SECTION_TITLE) SecGtkThemes
  Section $(GTK_NOTHEME_SECTION_TITLE) SecGtkNone
    Call CanWeInstallATheme
    Pop $R1
    StrCmp $R1 "" done

    SetOverwrite on
    Rename $R1\${GTK_DEFAULT_THEME_GTKRC_DIR}\gtkrc $R1\${GTK_DEFAULT_THEME_GTKRC_DIR}\gtkrc.old
    CopyFiles $R1\${GTK_DEFAULT_THEME_GTKRC_DIR}\gtkrc.plain $R1\${GTK_DEFAULT_THEME_GTKRC_DIR}\gtkrc
    SetOverwrite off

    done:
  SectionEnd

  Section $(GTK_WIMP_SECTION_TITLE) SecGtkWimp
    Call CanWeInstallATheme
    Pop $R1
    StrCmp $R1 "" done

    SetOverwrite on
    Rename $R1\${GTK_DEFAULT_THEME_GTKRC_DIR}\gtkrc $R1\${GTK_DEFAULT_THEME_GTKRC_DIR}\gtkrc.old
    SetOutPath $R1\${GTK_DEFAULT_THEME_ENGINE_DIR}
    File ${GTK_THEME_DIR}\engines\libwimp.dll
    SetOutPath $R1\${GTK_DEFAULT_THEME_GTKRC_DIR}
    File ${GTK_THEME_DIR}\themes\gtkrc.gtkwimp
    File /oname=gtkrc ${GTK_THEME_DIR}\themes\gtkrc.gtkwimp
    SetOverwrite off
    
    done:
  SectionEnd

  Section $(GTK_BLUECURVE_SECTION_TITLE) SecGtkBluecurve
    Call CanWeInstallATheme
    Pop $R1
    StrCmp $R1 "" done

    SetOverwrite on
    Rename $R1\${GTK_DEFAULT_THEME_GTKRC_DIR}\gtkrc $R1\${GTK_DEFAULT_THEME_GTKRC_DIR}\gtkrc.old
    SetOutPath $R1\${GTK_DEFAULT_THEME_ENGINE_DIR}
    File ${GTK_THEME_DIR}\engines\libbluecurve.dll
    SetOutPath $R1\${GTK_DEFAULT_THEME_GTKRC_DIR}
    File ${GTK_THEME_DIR}\themes\gtkrc.bluecurve
    File /oname=gtkrc ${GTK_THEME_DIR}\themes\gtkrc.bluecurve
    SetOverwrite off

    done:
  SectionEnd

  Section $(GTK_LIGHTHOUSEBLUE_SECTION_TITLE) SecGtkLighthouseblue
    Call CanWeInstallATheme
    Pop $R1
    StrCmp $R1 "" done

    SetOverwrite on
    Rename $R1\${GTK_DEFAULT_THEME_GTKRC_DIR}\gtkrc $R1\${GTK_DEFAULT_THEME_GTKRC_DIR}\gtkrc.old
    SetOutPath $R1\${GTK_DEFAULT_THEME_ENGINE_DIR}
    File ${GTK_THEME_DIR}\engines\liblighthouseblue.dll
    SetOutPath $R1\${GTK_DEFAULT_THEME_GTKRC_DIR}
    File ${GTK_THEME_DIR}\themes\gtkrc.lighthouseblue
    File /oname=gtkrc ${GTK_THEME_DIR}\themes\gtkrc.lighthouseblue
    SetOverwrite off

    done:
  SectionEnd
SubSectionEnd

;--------------------------------
;Uninstaller Section


Section Uninstall
  Call un.CheckUserInstallRights
  Pop $R0
  StrCmp $R0 "NONE" no_rights
  StrCmp $R0 "HKCU" try_hkcu try_hklm

  try_hkcu:
    ReadRegStr $R0 HKCU ${GAIM_REG_KEY} ""
    StrCmp $R0 $INSTDIR 0 cant_uninstall
      ; HKCU install path matches our INSTDIR.. so uninstall
      DeleteRegKey HKCU ${GAIM_REG_KEY}
      DeleteRegKey HKCU "${GAIM_UNINSTALL_KEY}"
      Goto cont_uninstall

  try_hklm:
    ReadRegStr $R0 HKLM ${GAIM_REG_KEY} ""
    StrCmp $R0 $INSTDIR 0 try_hkcu
      ; HKLM install path matches our INSTDIR.. so uninstall
      DeleteRegKey HKLM ${GAIM_REG_KEY}
      DeleteRegKey HKLM "${GAIM_UNINSTALL_KEY}"
      DeleteRegKey HKLM "${HKLM_APP_PATHS_KEY}"
      ; Sets start menu and desktop scope to all users..
      SetShellVarContext "all"

  cont_uninstall:
    ; The WinPrefs plugin may have left this behind..
    DeleteRegValue HKCU "${GAIM_STARTUP_RUN_KEY}" "Gaim"
    DeleteRegValue HKLM "${GAIM_STARTUP_RUN_KEY}" "Gaim"
    ; Remove Language preference info
    DeleteRegKey ${MUI_LANGDLL_REGISTRY_ROOT} ${MUI_LANGDLL_REGISTRY_KEY}

    RMDir /r "$INSTDIR\locale"
    RMDir /r "$INSTDIR\pixmaps"
    RMDir /r "$INSTDIR\perlmod"
    Delete "$INSTDIR\plugins\autorecon.dll"
    Delete "$INSTDIR\plugins\docklet.dll"
    Delete "$INSTDIR\plugins\history.dll"
    Delete "$INSTDIR\plugins\iconaway.dll"
    Delete "$INSTDIR\plugins\idle.dll"
    Delete "$INSTDIR\plugins\libgg.dll"
    Delete "$INSTDIR\plugins\libirc.dll"
    Delete "$INSTDIR\plugins\libjabber.dll"
    Delete "$INSTDIR\plugins\libmsn.dll"
    Delete "$INSTDIR\plugins\liboscar.dll"
    Delete "$INSTDIR\plugins\libtoc.dll"
    Delete "$INSTDIR\plugins\libyahoo.dll"
    Delete "$INSTDIR\plugins\perl.dll"
    Delete "$INSTDIR\plugins\spellchk.dll"
    Delete "$INSTDIR\plugins\ssl-nss.dll"
    Delete "$INSTDIR\plugins\ssl.dll"
    Delete "$INSTDIR\plugins\statenotify.dll"
    Delete "$INSTDIR\plugins\ticker.dll"
    Delete "$INSTDIR\plugins\timestamp.dll"
    Delete "$INSTDIR\plugins\win2ktrans.dll"
    Delete "$INSTDIR\plugins\winprefs.dll"
    RMDir "$INSTDIR\plugins"
    Delete "$INSTDIR\sounds\gaim\arrive.wav"
    Delete "$INSTDIR\sounds\gaim\leave.wav"
    Delete "$INSTDIR\sounds\gaim\receive.wav"
    Delete "$INSTDIR\sounds\gaim\redalert.wav"
    Delete "$INSTDIR\sounds\gaim\send.wav"
    RMDir "$INSTDIR\sounds\gaim"
    RMDir "$INSTDIR\sounds"
    Delete "$INSTDIR\gaim.dll"
    Delete "$INSTDIR\gaim.exe"
    Delete "$INSTDIR\idletrack.dll"
    Delete "$INSTDIR\libgtkspell.dll"
    Delete "$INSTDIR\nspr4.dll"
    Delete "$INSTDIR\nss3.dll"
    Delete "$INSTDIR\nssckbi.dll"
    Delete "$INSTDIR\plc4.dll"
    Delete "$INSTDIR\plds4.dll"
    Delete "$INSTDIR\softokn3.dll"
    Delete "$INSTDIR\ssl3.dll"
    Delete "$INSTDIR\${GAIM_UNINST_EXE}"
    !ifdef DEBUG
    Delete "$INSTDIR\exchndl.dll"
    !endif

    ;Try to remove Gaim install dir .. if empty
    RMDir "$INSTDIR"

    ; Shortcuts..
    RMDir /r "$SMPROGRAMS\Gaim"
    Delete "$DESKTOP\Gaim.lnk"

    Goto done

  cant_uninstall:
    MessageBox MB_OK $(un.GAIM_UNINSTALL_ERROR_1) IDOK
    Quit

  no_rights:
    MessageBox MB_OK $(un.GAIM_UNINSTALL_ERROR_2) IDOK
    Quit

  done:
  ;Display the Finish header
  !insertmacro MUI_UNFINISHHEADER
SectionEnd ; end of uninstall section

;--------------------------------
;Descriptions
!insertmacro MUI_FUNCTIONS_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecGaim} \
	$(GAIM_SECTION_DESCRIPTION)
!ifdef WITH_GTK
  !insertmacro MUI_DESCRIPTION_TEXT ${SecGtk} \
	$(GTK_SECTION_DESCRIPTION)
!endif
  !insertmacro MUI_DESCRIPTION_TEXT ${SecGtkThemes} \
	$(GTK_THEMES_SECTION_DESCRIPTION)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecGtkNone} \
        $(GTK_NO_THEME_DESC)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecGtkWimp} \
	$(GTK_WIMP_THEME_DESC)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecGtkBluecurve} \
        $(GTK_BLUECURVE_THEME_DESC)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecGtkLighthouseblue} \
        $(GTK_LIGHTHOUSEBLUE_THEME_DESC)
!insertmacro MUI_FUNCTIONS_DESCRIPTION_END

;--------------------------------
;Functions

;
; Usage:
;
; Call CanWeInstallATheme
; Pop $R0
;
; Return:
;   "" - If no
;   "root path of GTK+ installation" - if yes
;
Function CanWeInstallATheme
    Push $1
    Push $0

    ; Did we install GTK+ to the Gaim dir?
    IfFileExists "$INSTDIR\lib" 0 check_keys
      StrCpy $1 $INSTDIR
      Goto done

    check_keys:
    ; First see if we can install a theme..
    Call CheckUserInstallRights
    Pop $0

    StrCmp $0 "HKCU" hkcu
    StrCmp $0 "HKLM" hklm no_rights

    hkcu:
      ReadRegStr $1 HKCU ${GTK_REG_KEY} "Path"
      StrCmp $1 "" no_rights done

    hklm:
      ReadRegStr $1 HKLM ${GTK_REG_KEY} "Path"
      StrCmp $1 "" no_rights done

    no_rights:
      MessageBox MB_OK $(GTK_NO_THEME_INSTALL_RIGHTS) IDOK done
      StrCpy $1 ""

    done:
      Pop $0
      Exch $1
FunctionEnd


Function CheckUserInstallRights
	ClearErrors
	UserInfo::GetName
	IfErrors Win9x
	Pop $0
	UserInfo::GetAccountType
	Pop $1

	StrCmp $1 "Admin" 0 +3
                StrCpy $1 "HKLM"
		Goto done
	StrCmp $1 "Power" 0 +3
                StrCpy $1 "HKLM"
		Goto done
	StrCmp $1 "User" 0 +3
		StrCpy $1 "HKCU"
		Goto done
	StrCmp $1 "Guest" 0 +3
		StrCpy $1 "NONE"
		Goto done
	; Unknown error
	StrCpy $1 "NONE"
        Goto done

	Win9x:
		StrCpy $1 "HKLM"

	done:
        Push $1
FunctionEnd

Function un.CheckUserInstallRights
	ClearErrors
	UserInfo::GetName
	IfErrors Win9x
	Pop $0
	UserInfo::GetAccountType
	Pop $1
	StrCmp $1 "Admin" 0 +3
                StrCpy $1 "HKLM"
		Goto done
	StrCmp $1 "Power" 0 +3
                StrCpy $1 "HKLM"
		Goto done
	StrCmp $1 "User" 0 +3
		StrCpy $1 "HKCU"
		Goto done
	StrCmp $1 "Guest" 0 +3
		StrCpy $1 "NONE"
		Goto done
	; Unknown error
	StrCpy $1 "NONE"
        Goto done

	Win9x:
		StrCpy $1 "HKLM"

	done:
        Push $1
FunctionEnd

;
; Usage:
;   Push $0 ; Path string
;   Call VerifyDir
;   Pop $0 ; 0 - Bad path  1 - Good path
;
Function VerifyDir
  Pop $0
  Loop:
    IfFileExists $0 dir_exists
    StrCpy $1 $0 ; save last
    Push $0
    Call GetParent
    Pop $0
    StrLen $2 $0
    ; IfFileExists "C:" on xp returns true and on win2k returns false
    ; So we're done in such a case..
    StrCmp $2 "2" loop_done
    Goto Loop

  loop_done:
    StrCpy $1 "$0\GaImFooB"
    ; Check if we can create dir on this drive..
    ClearErrors
    CreateDirectory $1
    IfErrors DirBad DirGood

  dir_exists:
    ClearErrors
    FileOpen $1 "$0\gaimfoo.bar" w
    IfErrors PathBad PathGood

    DirGood:
      RMDir $1
      Goto PathGood1

    DirBad:
      RMDir $1
      Goto PathBad1

    PathBad:
      FileClose $1
      Delete "$0\gaimfoo.bar"
      PathBad1:
      StrCpy $0 "0"
      Push $0
      Return

    PathGood:
      FileClose $1
      Delete "$0\gaimfoo.bar"
      PathGood1:
      StrCpy $0 "1"
      Push $0
FunctionEnd

Function .onVerifyInstDir
  Push $INSTDIR
  Call VerifyDir
  Pop $0
  StrCmp $0 "0" 0 dir_good
    Abort
  dir_good:
FunctionEnd

; GetParent
; input, top of stack  (e.g. C:\Program Files\Poop)
; output, top of stack (replaces, with e.g. C:\Program Files)
; modifies no other variables.
;
; Usage:
;   Push "C:\Program Files\Directory\Whatever"
;   Call GetParent
;   Pop $R0
;   ; at this point $R0 will equal "C:\Program Files\Directory"
Function GetParent
   Exch $0 ; old $0 is on top of stack
   Push $1
   Push $2
   StrCpy $1 -1
   loop:
     StrCpy $2 $0 1 $1
     StrCmp $2 "" exit
     StrCmp $2 "\" exit
     IntOp $1 $1 - 1
   Goto loop
   exit:
     StrCpy $0 $0 $1
     Pop $2
     Pop $1
     Exch $0 ; put $0 on top of stack, restore $0 to original value
FunctionEnd


; CheckGtkVersion
; inputs: Push 2 GTK+ version strings to check. The major and minor values
; need to be equal, for success.  If the micro val to check is equal or greater
; to the refrence micro value, then we have success.
;
; Usage:
;   Push "2.2.0"  ; Refrence version
;   Push "2.2.1"  ; Version to check
;   Call CheckGtkVersion
;   Pop $R0
;   $R0 will now equal "0", because 2.2.0 is less than 2.2.1
;
Function CheckGtkVersion
  ; Version we want to check
  Pop $6 
  ; Reference version
  Pop $8 

  ; Check that the string to check is at least 5 chars long (i.e. x.x.x)
  StrLen $7 $6
  IntCmp $7 5 0 bad_version

  ; Major version check
  StrCpy $7 $6 1
  StrCpy $9 $8 1
  IntCmp $7 $9 check_minor
    Goto bad_version

  check_minor:
    StrCpy $7 $6 1 2
    StrCpy $9 $8 1 2
    IntCmp $7 $9 check_micro
      Goto bad_version

  check_micro:
    StrCpy $7 $6 1 4
    StrCpy $9 $8 1 4
    IntCmp $7 $9 good_version bad_version good_version

  bad_version:
    StrCpy $6 "0"
    Push $6
    Goto done

  good_version:
    StrCpy $6 "1"
    Push $6
  done:
FunctionEnd

;
; Usage:
; Call DoWeNeedGtk
; First Pop:
;   0 - We have the correct version
;       Second Pop: Key where Version was found
;   1 - We have an old version that needs to be upgraded
;       Second Pop: HKLM or HKCU depending on where GTK was found.
;   2 - We don't have Gtk+ at all
;       Second Pop: "NONE, HKLM or HKCU" depending on our rights..
;
Function DoWeNeedGtk
  ; Logic should be:
  ; - Check what user rights we have (HKLM or HKCU)
  ;   - If HKLM rights..
  ;     - Only check HKLM key for GTK+
  ;       - If installed to HKLM, check it and return.
  ;   - If HKCU rights..
  ;     - First check HKCU key for GTK+
  ;       - if good or bad exists stop and ret.
  ;     - If no hkcu gtk+ install, check HKLM
  ;       - If HKLM ver exists but old, return as if no ver exits.
  ;   - If no rights
  ;     - Check HKLM

  Call CheckUserInstallRights
  Pop $3
  StrCmp $3 "HKLM" check_hklm
  StrCmp $3 "HKCU" check_hkcu check_hklm
    check_hkcu:
      ReadRegStr $0 HKCU ${GTK_REG_KEY} "Version"
      StrCpy $5 "HKCU"
      StrCmp $0 "" check_hklm have_gtk

    check_hklm:
      ReadRegStr $0 HKLM ${GTK_REG_KEY} "Version"
      StrCpy $5 "HKLM"
      StrCmp $0 "" no_gtk have_gtk


  have_gtk:
    ; GTK+ is already installed.. check version.
    StrCpy $1 ${GTK_VERSION} ; Minimum GTK+ version needed
    Push $1
    Push $0
    Call CheckGtkVersion
    Pop $2
    StrCmp $2 "1" good_version bad_version
    bad_version:
      ; Bad version. If hklm ver and we have hkcu or no rights.. return no gtk
      StrCmp $3 "NONE" no_gtk  ; if no rights.. can't upgrade
      StrCmp $3 "HKCU" 0 upgrade_gtk ; if HKLM can upgrade..
        StrCmp $5 "HKLM" no_gtk upgrade_gtk ; have hkcu rights.. if found hklm ver can't upgrade..
  
      upgrade_gtk:
        StrCpy $2 "1"
        Push $5
        Push $2
        Goto done

  good_version:
    StrCmp $5 "HKLM" have_hklm_gtk have_hkcu_gtk
      have_hkcu_gtk:
        ; Have HKCU version
        ReadRegStr $4 HKCU ${GTK_REG_KEY} "Path"
        Goto good_version_cont

      have_hklm_gtk:
        ReadRegStr $4 HKLM ${GTK_REG_KEY} "Path"
        Goto good_version_cont

    good_version_cont:
      StrCpy $2 "0"
      Push $4  ; The path to existing GTK+
      Push $2
      Goto done

  no_gtk:
    StrCpy $2 "2"
    Push $3 ; our rights
    Push $2
    Goto done

  done:
FunctionEnd

Function .onInit
  ; If this installer dosn't have GTK, check whether we need it.
  !ifndef WITH_GTK
    Call DoWeNeedGtk
    Pop $0
    Pop $1

    StrCmp $0 "0" have_gtk need_gtk
    need_gtk:
      MessageBox MB_OK $(GTK_INSTALLER_NEEDED) IDOK
      Quit
    have_gtk:
  !else
  ;Extract InstallOptions INI Files
  !insertmacro MUI_INSTALLOPTIONS_EXTRACT_AS "${GAIM_NSIS_INCLUDE_PATH}\gtkInstall.ini" "gtkInstall.ini"
  !endif

  Call CheckUserInstallRights
  Pop $0

  StrCmp $0 "HKLM" 0 user_dir
    StrCpy $INSTDIR "$PROGRAMFILES\Gaim"
    Goto instdir_done
  user_dir:
    StrCpy $2 "$SMPROGRAMS"
    Push $2
    Call GetParent
    Call GetParent
    Pop $2
    StrCpy $INSTDIR "$2\Gaim"

  instdir_done:

  ; Set up Theme sections..
  StrCpy $1 ${SecGtkNone} ; Sets global to remember which theme is set.
  !insertmacro SelectSection ${SecGtkNone}
  !insertmacro UnselectSection ${SecGtkWimp}
  !insertmacro UnselectSection ${SecGtkBluecurve}
  !insertmacro UnselectSection ${SecGtkLighthouseblue}

  ; Display Language selection dialog
  !insertmacro MUI_LANGDLL_DISPLAY

FunctionEnd

Function un.onInit

  ; Get stored language prefrence
  !insertmacro MUI_UNGETLANGUAGE
  
FunctionEnd

Function .onSelChange
  Push $0
  Push $2

  StrCpy $2 ${SF_SELECTED}
  SectionGetFlags ${SecGtkNone} $0
  IntOp $2 $2 & $0
  SectionGetFlags ${SecGtkWimp} $0
  IntOp $2 $2 & $0
  SectionGetFlags ${SecGtkBluecurve} $0
  IntOp $2 $2 & $0
  SectionGetFlags ${SecGtkLighthouseblue} $0
  IntOp $2 $2 & $0
  StrCmp $2 0 skip
    SectionSetFlags ${SecGtkNone} 0
    SectionSetFlags ${SecGtkWimp} 0
    SectionSetFlags ${SecGtkBluecurve} 0
    SectionSetFlags ${SecGtkLighthouseblue} 0
  skip:

  !insertmacro UnselectSection $1
 
  ; Remember old selection
  StrCpy $2 $1

  ; Now go through and see who is checked..
  SectionGetFlags ${SecGtkNone} $0
  IntOp $0 $0 & ${SF_SELECTED}
  IntCmp $0 ${SF_SELECTED} 0 +2 +2
    StrCpy $1 ${SecGtkNone}
  SectionGetFlags ${SecGtkWimp} $0
  IntOp $0 $0 & ${SF_SELECTED}
  IntCmp $0 ${SF_SELECTED} 0 +2 +2
    StrCpy $1 ${SecGtkWimp}
  SectionGetFlags ${SecGtkBluecurve} $0
  IntOp $0 $0 & ${SF_SELECTED}
  IntCmp $0 ${SF_SELECTED} 0 +2 +2
    StrCpy $1 ${SecGtkBluecurve}
  SectionGetFlags ${SecGtkLighthouseblue} $0
  IntOp $0 $0 & ${SF_SELECTED}
  IntCmp $0 ${SF_SELECTED} 0 +2 +2
    StrCpy $1 ${SecGtkLighthouseblue}

  StrCmp $2 $1 0 +2 ; selection hasn't changed
    !insertmacro SelectSection $1

  Pop $2
  Pop $0
FunctionEnd

Function ShowGtkInstallDirChooser
  Call DoWeNeedGtk
  Pop $0
  Pop $1

  StrCmp $0 "0" have_gtk
  StrCmp $0 "1" upgrade_gtk
  StrCmp $0 "2" no_gtk no_gtk

  ; Don't show dir selector.. Upgrades are done to existing path..
  have_gtk:
  upgrade_gtk:
    Abort

  no_gtk:
    StrCmp $1 "NONE" 0 no_gtk_cont
      ; Got no install rights..
      Abort
    no_gtk_cont:
      ; Suggest path..
      StrCmp $1 "HKCU" 0 hklm1
        StrCpy $2 "$SMPROGRAMS"
        Push $2
        Call GetParent
        Call GetParent
        Pop $2
        StrCpy $2 "$2\GTK\2.0"
        Goto got_path
      hklm1:
        StrCpy $2 "${GTK_DEFAULT_INSTALL_PATH}"

  got_path:
    !insertmacro MUI_INSTALLOPTIONS_WRITE "gtkInstall.ini" "Field 4" "State" $2

  !insertmacro MUI_INSTALLOPTIONS_WRITE "gtkInstall.ini" "Field 1" "Text" $(GTK_PAGE_INSTALL_MSG1)
  !insertmacro MUI_INSTALLOPTIONS_WRITE "gtkInstall.ini" "Field 2" "Text" $(GTK_PAGE_INSTALL_MSG2)
  !insertmacro MUI_HEADER_TEXT "$(GTK_PAGE_TITLE)" "$(GTK_PAGE_SUBTITLE)"
  !insertmacro MUI_INSTALLOPTIONS_DISPLAY "gtkInstall.ini"
FunctionEnd

Function GtkInstallDirVerify
  !insertmacro MUI_INSTALLOPTIONS_READ $0 "gtkInstall.ini" "Field 4" "State"
  Push $0
  Call VerifyDir
  Pop $0
  StrCmp $0 "0" 0 done
    MessageBox MB_OK $(GTK_BAD_INSTALL_PATH) IDOK
    Abort
  done:
FunctionEnd

