#!/usr/bin/env sh
set -eau

set +e
(
set -e

SS_BUILD_DIR=build-synchro_simple-Desktop_Qt_5_2_0_MinGW_32bit-Release

check_var() {
	echo -n "checking $1..."
	if ! eval "(echo \${$1})"; then
		echo "Please define $1 in shell environment variables."
		echo ${2}
		exit 1
	fi 2>/dev/null
}

my_convert() {
	(
		cd "$1"
		pwd
	)
}

title() {
	echo '*************************'
	echo "${@:-}"
	echo '*************************'
}

check_dir() {
	echo -n "checking $1..."
	if eval "(cd $1)"; then
		echo "OK"
	else
		echo "$2"
		exit 1
	fi 2>/dev/null
}

check_cmd() {
	echo -n "checking $1..."
	if ! eval "(type $1)"; then
		echo "$2"
		exit 1
	fi 2>/dev/null
}

# Code starts here.
title "Check prerequisites"
# regex
check_dir "/usr/local/include/tre" "tre is not installed (http://laurikari.net/tre/)"
# makensis
check_cmd makensis "makensis is not installed (http://nsis.sourceforge.net/Main_Page)"
# Qt
check_var QT_MINGW_BIN "Dir where Qt MinGW binaries are installed (ex: C:/Qt/Qt5.2.0/5.2.0/mingw48_32/bin)."
check_var QT_TOOLS_BIN "Dir where Qt Tools binaries are installed (ex: C:/Qt/Qt5.2.0/Tools/mingw48_32/bin)."


title "Compiling"
(
	cd ../../utf8_fs
	make clean all install
)

(
	cd ../../synchro_test
	make clean all install
)

mkdir -p ${SS_BUILD_DIR}

(

	PATH="`my_convert "${QT_MINGW_BIN}"`:`my_convert "${QT_TOOLS_BIN}"`:$PATH"
	type g++
	cd ${SS_BUILD_DIR}
	"${QT_MINGW_BIN}/qmake.exe" ../synchro_simple/synchro_simple.pro -r -spec win32-g++
	"${QT_TOOLS_BIN}/mingw32-make.exe"
)

title "Build the setup"

SETUP_DIR="./setup"
EXECUTABLE_NAME=synchro_simple

PROGRAM_NAME="Synchro Simple"
DEST_DIR="${SETUP_DIR}/${PROGRAM_NAME}"

mkdir -p "${DEST_DIR}"
rm -f "${DEST_DIR}/*"



BUILD_DIR=`find . -type d -name 'build-*_MinGW_32bit-Release'`

cp "${BUILD_DIR}/release/${EXECUTABLE_NAME}.exe" "${DEST_DIR}"
"${QT_MINGW_BIN}/windeployqt.exe" "${DEST_DIR}"
cp "${QT_TOOLS_BIN}/libgcc_s_dw2-1.dll" "${DEST_DIR}"
cp "${QT_TOOLS_BIN}/libstdc++-6.dll" "${DEST_DIR}"
cp "${QT_TOOLS_BIN}/libwinpthread-1.dll" "${DEST_DIR}"
cp "/mingw/bin/libiconv-2.dll" "${DEST_DIR}"
cp "/mingw/bin/libintl-8.dll" "${DEST_DIR}"
cp "../../synchro_test/synchro.dll" "${DEST_DIR}"
cp "${QT_MINGW_BIN}/libtre-5.dll" "${DEST_DIR}"

# Run the NSIS script to do the setup.exe
makensis setup.nsi

echo "Successfully finished.".

# Code ends here.
)
STATUS=$?
if [ $STATUS != 0 ]; then
	echo "Error (code=${STATUS})"
fi
