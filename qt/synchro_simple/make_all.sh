#!/usr/bin/env sh
set -eau

set +e
(
set -e

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

# Code ends here.
)
STATUS=$?
if [ $STATUS != 0 ]; then
	echo "Error (code=${STATUS})"
fi
#"C:\Qt\Qt5.2.0\5.2.0\mingw48_32\bin\qmake.exe" C:\Users\Yannis\Documents\Projects\sandbox\qt\synchro_simple\synchro_simple\synchro_simple.pro -r -spec win32-g++