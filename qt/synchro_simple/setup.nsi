!define NAME "Synchro Simple"
!define EXECUTABLE_NAME "synchro_simple"
!define INPUT_DIR "setup\${NAME}"


Name "${NAME}"

InstallDir "$PROGRAMFILES\${NAME}"

outFile "${EXECUTABLE_NAME}_setup.exe"
RequestExecutionLevel admin

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

Section "${NAME} (required)"
  SectionIn RO

  SetOutPath $INSTDIR
  File /r "${INPUT_DIR}\*"

  ; Write the installation path into the registry
  WriteRegStr HKLM "SOFTWARE\${NAME}" "Install_Dir" "$INSTDIR"

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "DisplayIcon" "$INSTDIR\${EXECUTABLE_NAME}.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "DisplayName" "${NAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

SectionEnd

Section "Start Menu Shortcuts"
  CreateDirectory "$SMPROGRAMS\${NAME}"
  CreateShortCut "$SMPROGRAMS\${NAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\${NAME}\${NAME}.lnk" "$INSTDIR\${EXECUTABLE_NAME}.exe" "" "$INSTDIR\${EXECUTABLE_NAME}.exe" 0
SectionEnd

Section "Desktop shortcut"
  CreateShortCut "$DESKTOP\${NAME}.lnk" "$INSTDIR\${EXECUTABLE_NAME}.exe" "" "$INSTDIR\${EXECUTABLE_NAME}.exe" 0
SectionEnd

;--------------------------------
; Uninstaller
Section "Uninstall"
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}"
  DeleteRegKey HKLM "SOFTWARE\${NAME}"

  ; Remove files and uninstaller
  RMDir /r /REBOOTOK "$INSTDIR"

  Delete "$SMPROGRAMS\${NAME}\*.*"
  Delete "$DESKTOP\${NAME}.lnk"
  RMDir "$SMPROGRAMS\${NAME}"
SectionEnd
