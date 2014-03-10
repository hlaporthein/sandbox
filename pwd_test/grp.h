#ifndef GRP_H
#define GRP_H
typedef int gid_t;

struct group {
	char *gr_name;
	int gr_gid;
	char **gr_mem;
};

struct group *getgrgid(gid_t gid);
struct group *getgrnam(const char *name);
int getgrgid_r(gid_t gid, struct group *grp, char *buffer, size_t bufsize, struct group **result);
int getgrnam_r(const char *name, struct group *grp, char *buffer, size_t bufsize, struct group **result);
struct group *getgrent(void);
struct group *getgrent(void);
void setgrent(void);

#endif