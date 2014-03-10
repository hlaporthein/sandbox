#ifndef PWD_H
#define PWD_H
typedef int uid_t;

struct passwd {
	char *pw_name;				/* user name */
	int pw_uid;					/* user uid */
	int pw_gid;					/* user gid */
	char *pw_dir;				/* home directory */
	char *pw_shell;				/* default shell */
};

struct passwd *getpwuid(int uid);

struct passwd *getpwnam(char *name);
int getpwnam_r(const char *nam, struct passwd *pwd, char *buffer, size_t bufsize, struct passwd **result);
int getpwuid_r(uid_t uid, struct passwd *pwd, char *buffer, size_t bufsize, struct passwd **result);
void endpwent(void);
struct passwd *getpwent(void);
void setpwent(void);

#endif