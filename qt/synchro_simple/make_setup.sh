#!/usr/bin/env sh
set -eau

QTDIR="${QTDIR:-C:/Qt}"
QTVER="${QTVER:-5.2.1}"
QT_MINGW_BIN="${QTDIR}/${QTVER}/mingw48_32/bin"
QT_TOOLS_BIN="${QTDIR}/Tools/mingw48_32/bin"
SETUP_DIR="./setup"
EXECUTABLE_NAME=synchro_simple

PROGRAM_NAME="Synchro Simple"
DEST_DIR="${SETUP_DIR}/${PROGRAM_NAME}"

mkdir -p "${DEST_DIR}"
rm -f "${DEST_DIR}/*"

BUILD_DIR=`find . -type d -name 'build-*_MinGW_32bit-Release'`

cp "${BUILD_DIR}/release/${EXECUTABLE_NAME}.exe" "${DEST_DIR}"
cp "${QT_MINGW_BIN}/Qt5Core.dll" "${DEST_DIR}"
cp "${QT_MINGW_BIN}/Qt5Gui.dll" "${DEST_DIR}"
cp "${QT_MINGW_BIN}/Qt5Widgets.dll" "${DEST_DIR}"
cp "${QT_MINGW_BIN}/icudt51.dll" "${DEST_DIR}"
cp "${QT_MINGW_BIN}/icuin51.dll" "${DEST_DIR}"
cp "${QT_MINGW_BIN}/icuuc51.dll" "${DEST_DIR}"
cp "${QT_TOOLS_BIN}/libgcc_s_dw2-1.dll" "${DEST_DIR}"
cp "${QT_TOOLS_BIN}/libstdc++-6.dll" "${DEST_DIR}"
cp "${QT_TOOLS_BIN}/libwinpthread-1.dll" "${DEST_DIR}"
cp "../../copy_struct_test/synchro.dll" "${DEST_DIR}"

# Run the NSIS script to do the setup.exe
makensis setup.nsi

echo "Successfully finished.".