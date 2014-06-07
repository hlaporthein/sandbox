#include <stdio.h>
#include <regex.h>

#define TRY(statement, condition, error_message, ...) \
	statement; \
	if (condition) { \
		fprintf(stderr, error_message, ##__VA_ARGS__); \
		result = -1; \
		goto cleanup; \
	}

int main() {
	int result = 0;

	// Compile regular expression
	regex_t regex;
	int res = TRY(regcomp(&regex, "^a[[:alnum:]]", 0), res , "Could not compile regex\n");

	// Execute regular expression
	res = regexec(&regex, "abc", 0, NULL, 0);
	if (res == 0) {
		puts("Match");
	} else if (res == REG_NOMATCH) {
		puts("No match");
	} else {
		char msgbuf[1024];
		regerror(res, &regex, msgbuf, 1024);
		fprintf(stderr, "Regex match failed: %s\n", msgbuf);
		result = -1;
		goto cleanup;
	}

	// Free compiled regular expression if you want to use the regex_t again
    regfree(&regex);

cleanup:
	return result;
}




