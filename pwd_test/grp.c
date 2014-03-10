#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "grp.h"

static struct group grp;

struct group *getgrgid(gid_t gid) {
	grp.gr_name = "unknown";
	grp.gr_gid = gid;

	char * c = "unknown";
	grp.gr_mem = &c;

	return &grp;
}

int getgrgid_r(gid_t gid, struct group *grp, char *buffer, size_t bufsize, struct group **result) {
	return 0;
}

struct group *getgrnam(const char *name) {
	return NULL;
}

int getgrnam_r(const char *name, struct group *grp, char *buffer, size_t bufsize, struct group **result) {
	return 0;
}

struct group *getgrent(void) {
	return NULL;
}

void setgrent(void) {
}