#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>


FILE *utf8_fopen(const char *path, const char *mode);

int utf8_mkdir(const char *path);
int utf8_rmdir(const char *path);

int utf8_unlink(const char *path);

struct utf8_dirent {
	void *dir_entry;
	char *d_name;
	struct utf8_dirent *next;
};

typedef struct {
	void *dir;
	int type;
	struct utf8_dirent *first;
} UTF8_DIR;



UTF8_DIR *utf8_opendir(const char *dirname);
struct utf8_dirent *utf8_readdir(UTF8_DIR *dirp);
int utf8_closedir(UTF8_DIR *dirp);

int utf8_stat(const char *path, struct stat *buf);