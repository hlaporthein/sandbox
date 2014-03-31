#include <string.h>

size_t strlcat(char *dst, char *src, size_t size) {
	size_t l = strlen(dst);
	size_t e = strlen(src) + 1;
	if (e + l > size) {
		e = size - l;
	}
	memcpy(dst + l, src, e);
	dst[size - 1] = 0;
	return l + e;
}