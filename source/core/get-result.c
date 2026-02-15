#include <stdlib.h>
#include "quine.h"
#include <string.h>

char* getResult(char* s1, char* s2)
{
	if (!s1 && !s2) return NULL;

	if(!s1) {
        size_t len = strlen(s2);
        char *out = malloc(len + 1);
        if (!out) return NULL;
        memcpy(out, s2, len + 1);
        return out;
	}

	if (!s2) {
        size_t len = strlen(s1);
        char *out = malloc(len + 1);
        if (!out) return NULL;
        memcpy(out, s1, len + 1);
        return out;
    }

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    int need_comma = (len1 > 0 && len2 > 0);

    char *out = malloc(len1 + len2 + need_comma + 1);
    if (!out) return NULL;

    memcpy(out, s1, len1);
    size_t pos = len1;
    if(need_comma) out[pos++] = ',';
    memcpy(out + pos, s2, len2);
    out[pos + len2] = '\0';

    return out;
}

