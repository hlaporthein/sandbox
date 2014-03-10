#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "pwd.h"

/* Not thread safe */
static struct passwd pw;
static char name[256];

static char *getlogin() {
	DWORD dw = 256;
	GetUserName(name, &dw);
	return name;
}

struct passwd *getpwuid(int uid) {
	pw.pw_name = getlogin();
	pw.pw_dir = ".";
	pw.pw_shell = "unknown";
	pw.pw_uid = uid;

	return &pw;
}

struct passwd *getpwnam(char *name) {
	return (struct passwd *) NULL;
}

int getpwnam_r(const char *nam, struct passwd *pwd, char *buffer, size_t bufsize, struct passwd **result) {
	return 0;
}
int getpwuid_r(uid_t uid, struct passwd *pwd, char *buffer, size_t bufsize, struct passwd **result) {
	return 0;
}
void endpwent(void) {
};
struct passwd *getpwent(void) {
	return NULL;
};
void setpwent(void) {
};
