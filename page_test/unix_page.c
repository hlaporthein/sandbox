#include <stdio.h>
#ifndef __MINGW32__
#include <unistd.h> /* sysconf(3) */
#endif

int main(void) {
#ifdef __MINGW32__
	printf("unistd is not well implemented on MINGW.\n");
#else
	printf("The page size for this system is %ld bytes.\n",
	       sysconf(_SC_PAGESIZE)); /* _SC_PAGE_SIZE is OK too. */
#endif
	return 0;
}
