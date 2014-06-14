#include <stdio.h>
#include <dirent.h>

FILE *utf8_fopen(const char *path, const char *mode);

int utf8_mkdir(const char *path);
int utf8_rmdir(const char *path);

int utf8_unlink(const char *path);

typedef struct {
	int type;
	union {
		int dir;
		int wdir;
	};
} UTF8_DIR;

struct utf8_dirent {
	char *d_name;
};

UTF8_DIR *utf8_opendir(const char *dirname);
struct utf8_dirent *utf8_readdir(UTF8_DIR *dirp);
