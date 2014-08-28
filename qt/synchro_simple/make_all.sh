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

# Code starts here.
echo "Check prerequisites"
# regex

echo "Compiling"
(
	cd ../../utf8_fs
	make clean all install
)

(
	cd ../../synchro_test
	make clean all install
)

check_var QT_MINGW_BIN "Dir where Qt MinGW binaries is installed (ex: C:/Qt/Qt5.2.0/5.2.0/mingw48_32/bin)."
check_var QT_TOOLS_BIN "Dir where Qt Tools binaries is installed (ex: C:/Qt/Qt5.2.0/Tools/mingw48_32/bin)."

mkdir -p ${SS_BUILD_DIR}

(

	PATH="`my_convert "${QT_MINGW_BIN}"`:`my_convert "${QT_TOOLS_BIN}"`:$PATH"
	type g++
	cd ${SS_BUILD_DIR}
	"${QT_MINGW_BIN}/qmake.exe" ../synchro_simple/synchro_simple.pro -r -spec win32-g++
	"${QT_TOOLS_BIN}/mingw32-make.exe"
)

make_setup.sh

# Code ends here.
)
STATUS=$?
if [ $STATUS != 0 ]; then
	echo "Error (code=${STATUS})"
fi
