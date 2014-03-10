#include <stdio.h>
#include "pwd.h"
#include "grp.h"

int main() {
	struct passwd *pw = getpwuid(13);
	printf("login: %s\n", pw->pw_name);

	struct group *grp = getgrgid(13);
	printf("grp_name: %s\n", grp->gr_name);
	return 0;
}