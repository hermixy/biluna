; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "Biluna"
!define PRODUCT_VERSION "0.9.12"
!define PRODUCT_PUBLISHER "Biluna"
!define PRODUCT_WEB_SITE "http://www.biluna.com/jcms"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\bil.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "license.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\bil.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Setup_Bil_peng_0_9_12.exe"
InstallDir "$PROGRAMFILES\Biluna"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "..\bil\release\bil.exe"
  CreateDirectory "$SMPROGRAMS\Biluna"
  CreateShortCut "$SMPROGRAMS\Biluna\Biluna.lnk" "$INSTDIR\bil.exe"
  CreateShortCut "$DESKTOP\Biluna.lnk" "$INSTDIR\bil.exe"
  File "..\bil\release\db.dll"
;  File "..\bil\release\gv.dll"
SectionEnd

Section "QtLibrarySection" SEC02
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\lib\QtXmlPatterns4.dll"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\lib\QtXml4.dll"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\lib\QtWebKit4.dll"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\lib\QtSvg4.dll"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\lib\QtSql4.dll"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\lib\QtScript4.dll"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\lib\QtNetwork4.dll"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\lib\QtHelp4.dll"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\lib\QtGui4.dll"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\lib\QtCore4.dll"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\lib\QtCLucene4.dll"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\lib\phonon4.dll"
  File "C:\QtSDK\mingw\bin\mingwm10.dll"
  File "C:\QtSDK\mingw\bin\libgcc_s_dw2-1.dll"
  ; only assistant (MSVC) that worked 4.7.4 size: 1.278 KB
  File "assistant.exe"
  File "C:\MySQL\5.1\bin\libmySQL.dll"
  File "C:\OpenSSL-Win32\libeay32.dll"
  File "C:\OpenSSL-Win32\libssl32.dll"
  ; old now libssl32.dll but required for webkit
  File "C:\OpenSSL-Win32\ssleay32.dll" 
SectionEnd

Section "QtImageFormatsSection" SEC03
  SetOutPath "$INSTDIR\imageformats"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\plugins\imageformats\qtiff4.dll"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\plugins\imageformats\qsvg4.dll"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\plugins\imageformats\qmng4.dll"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\plugins\imageformats\qjpeg4.dll"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\plugins\imageformats\qico4.dll"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\plugins\imageformats\qgif4.dll"
SectionEnd

Section "QtSqlDriversSection" SEC04
  SetOutPath "$INSTDIR\sqldrivers"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\plugins\sqldrivers\qsqlite4.dll"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\plugins\sqldrivers\qsqlmysql4.dll"
  File "C:\QtSDK\Desktop\Qt\4.8.1\mingw\plugins\sqldrivers\qsqlodbc4.dll"
SectionEnd

Section "PluginsSection" SEC05
  SetOutPath "$INSTDIR\plugins"
;  File "..\bil\release\plugins\acc_actionplugin.dll"
;  File "..\bil\release\plugins\crm_actionplugin.dll"
  File "..\bil\release\plugins\peng_actionplugin.dll"
;  File "..\bil\release\plugins\sail_actionplugin.dll"
;  File "..\bil\release\plugins\scan_actionplugin.dll"
SectionEnd

Section "DocumentationSection" SEC06
  SetOutPath "$INSTDIR\doc"
  File "..\bil\doc\doc.qch"
  File "..\bil\doc\doc.qhc"
SectionEnd

Section "Data" SEC07
  SetOutPath "$INSTDIR\data"
  File "..\acc\data\acc_chartmaster_syssetting_nl.xml"
;  ...
SectionEnd

Section -AdditionalIcons
  SetOutPath $INSTDIR
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\Biluna\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\Biluna\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\bil.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\bil.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
;  Delete "$INSTDIR\doc\html\images\acc_accountcontroldialog.png" example only
;  ...
  Delete "$INSTDIR\data\acc_chartmaster_syssetting_nl.xml"
;  ...
  Delete "$INSTDIR\doc\doc.qhc"
  Delete "$INSTDIR\doc\doc.qch"
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
;  Delete "$INSTDIR\plugins\acc_actionplugin.dll"
;  Delete "$INSTDIR\plugins\crm_actionplugin.dll"
  Delete "$INSTDIR\plugins\peng_actionplugin.dll"
;  Delete "$INSTDIR\plugins\sail_actionplugin.dll"
;  Delete "$INSTDIR\plugins\scan_actionplugin.dll"
  Delete "$INSTDIR\sqldrivers\qsqlite4.dll"
  Delete "$INSTDIR\sqldrivers\qsqlmysql4.dll"
  Delete "$INSTDIR\sqldrivers\qsqlodbc4.dll"
  Delete "$INSTDIR\imageformats\qgif4.dll"
  Delete "$INSTDIR\imageformats\qico4.dll"
  Delete "$INSTDIR\imageformats\qjpeg4.dll"
  Delete "$INSTDIR\imageformats\qmng4.dll"
  Delete "$INSTDIR\imageformats\qsvg4.dll"
  Delete "$INSTDIR\imageformats\qtiff4.dll"
  
  Delete "$INSTDIR\assistant.exe"
  Delete "$INSTDIR\libeay32.dll"
  Delete "$INSTDIR\libssl32.dll"
  Delete "$INSTDIR\ssleay32.dll"
  Delete "$INSTDIR\libmySQL.dll"
  Delete "$INSTDIR\libgcc_s_dw2-1.dll"
  Delete "$INSTDIR\mingwm10.dll"
  Delete "$INSTDIR\phonon4.dll"
  Delete "$INSTDIR\QtCLucene4.dll"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtHelp4.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  Delete "$INSTDIR\QtScript4.dll"
  Delete "$INSTDIR\QtSql4.dll"
  Delete "$INSTDIR\QtSvg4.dll"
  Delete "$INSTDIR\QtWebKit4.dll"
  Delete "$INSTDIR\QtXml4.dll"
  Delete "$INSTDIR\QtXmlPatterns4.dll"
;  Delete "$INSTDIR\gv.dll"
  Delete "$INSTDIR\db.dll"
  Delete "$INSTDIR\bil.exe"

  Delete "$SMPROGRAMS\Biluna\Uninstall.lnk"
  Delete "$SMPROGRAMS\Biluna\Website.lnk"
  Delete "$DESKTOP\Biluna.lnk"
  Delete "$SMPROGRAMS\Biluna\Biluna.lnk"

  RMDir "$INSTDIR\sqldrivers"
  RMDir "$INSTDIR\plugins"
  RMDir "$INSTDIR\imageformats"
  RMDir "$INSTDIR\doc"
  RMDir "$INSTDIR\data"
  RMDir "$SMPROGRAMS\Biluna"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd