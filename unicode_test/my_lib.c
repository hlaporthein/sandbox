#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "my_lib.h"

LPCTSTR FTEXT(coucou1) = _T("abc.txt");
LPCTSTR FTEXT(coucou2) = _T("coucou2");

int FTEXT(do_things)(LPCTSTR s) {
	int result = 0;
	FILE* f = NULL;
	f = _tfopen(FTEXT(coucou1), TEXT("rb"));
	if (f == NULL) {
		printf("Error(%d): %s\n", errno, strerror(errno));
		result = -1;
		goto cleanup;
	}

#ifdef _UNICODE
	printf("UNICODE\n");
#endif // _UNICODE

	_tprintf(_T("%s\n"), s);

cleanup:
	if (f) {
		fclose(f);
	}
	return result;
}