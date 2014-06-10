!define NAME "Synchro Simple"
!define EXECUTABLE_NAME "synchro_simple"
!define INPUT_DIR "setup\Synchro Simple"


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
  File "/oname=Qt5Core.dll" "${INPUT_DIR}\Qt5Core.dll"
  File "/oname=Qt5Gui.dll"  "${INPUT_DIR}\Qt5Gui.dll"
  File "/oname=Qt5Widgets.dll" "${INPUT_DIR}\Qt5Widgets.dll"
  File "/oname=icudt51.dll" "${INPUT_DIR}\icudt51.dll"
  File "/oname=icuin51.dll" "${INPUT_DIR}\icuin51.dll"
  File "/oname=icuuc51.dll" "${INPUT_DIR}\icuuc51.dll"
  File "/oname=libgcc_s_dw2-1.dll" "${INPUT_DIR}\libgcc_s_dw2-1.dll"
  File "/oname=libstdc++-6.dll" "${INPUT_DIR}\libstdc++-6.dll"
  File "/oname=libwinpthread-1.dll" "${INPUT_DIR}\libwinpthread-1.dll"
  File "/oname=synchro.dll" "${INPUT_DIR}\synchro.dll"
  File "/oname=libtre-5.dll" "${INPUT_DIR}\libtre-5.dll"
  File "/oname=${EXECUTABLE_NAME}.exe" "${INPUT_DIR}\${EXECUTABLE_NAME}.exe"

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
  Delete "$INSTDIR\*"
  Delete "$INSTDIR"

  Delete "$SMPROGRAMS\${NAME}\*.*"
  Delete "$DESKTOP\${NAME}.lnk"
  RMDir "$SMPROGRAMS\${NAME}"
  RMDir "$INSTDIR"
SectionEnd
