#include <expat.h>
#include <stdio.h>
#include <strings.h>

#define TRY(statement, condition, error_message, ...) \
	statement; \
	if (condition) { \
		fprintf(stderr, error_message, ##__VA_ARGS__); \
		fprintf(stderr, "\n"); \
		return_status = 1; \
		goto cleanup; \
	}

//#define DEBUG_SWITCH 0

#ifdef DEBUG_SWITCH
#define DEBUG(format, ...) printf(format, ##__VA_ARGS__)
#else
#define DEBUG(format, ...)
#endif

#define BUFFSIZE 8192

typedef struct {
	int print;
} user_data_t;

void XMLCALL start(void *userData, const char *name, const char **attr) {
	DEBUG("START %s\n", name);
	DEBUG("userData=0x%x\n", userData);
	DEBUG("name=%s\n", name);
	DEBUG("attr=[");
	for (int i = 0; attr[i]; i += 2) {
		DEBUG(" %s='%s'", attr[i], attr[i + 1]);
	}
	DEBUG("]\n\n");

	if (strcmp(name, "toto") == 0) {
		((user_data_t *) userData)->print = 1;
	} else {
		((user_data_t *) userData)->print = 0;
	}
}

void XMLCALL end(void *userData, const char *name) {
	DEBUG("END %s\n", name);
	DEBUG("userData=0x%x\n", userData);
	DEBUG("name=%s\n\n", name);
	((user_data_t *) userData)->print = 0;
}

void XMLCALL character(void *userData, const char *s, int len) {
	if (((user_data_t *) userData)->print) {
		char buf[len + 1];
		snprintf(buf, len + 1, "%s", s);
		puts(buf);
	}
}

int main() {
	int return_status = 0;
	XML_Parser p = TRY(XML_ParserCreate(NULL), !p, "Couldn't allocate memory for parser");

	user_data_t userData;
	userData.print = 0;
	XML_SetUserData(p, &userData);

	XML_SetElementHandler(p, start, end);
	XML_SetCharacterDataHandler(p, character);

	FILE *fd = NULL;
	fd = TRY(fopen("test.xml", "rb"), fd == NULL, "fopen ERROR: %s\n", strerror(errno));

	char buff[BUFFSIZE];

	while(1) {
		int done;

		size_t len = TRY(fread(buff, 1, BUFFSIZE, fd), ferror(fd), "FREAD ERROR: \n", strerror(errno));
		done = feof(fd);

		int res = TRY(XML_Parse(p, buff, len, done), res == XML_STATUS_ERROR,
			"Parse error at line %lu:\n%s\n",
			XML_GetCurrentLineNumber(p),
			XML_ErrorString(XML_GetErrorCode(p))
		);

		if (done) {
			break;
		}
	}

cleanup:
	if (fd) {
		fclose(fd);
	}

	if (p) {
		XML_ParserFree(p);
	}
	return return_status;
}
