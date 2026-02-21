#ifdef _WIN32
#include <windows.h>
#endif

#include "memory_tracker.h"
#include <stdio.h>
#include <stdlib.h>
#include "qm_run.h"

static char *read_stdin();

int main()
{
	#ifdef _WIN32
	system("cls");
    SetConsoleOutputCP(CP_UTF8);
    #endif

	char *input = read_stdin();
	if(!input){
		puts("Failed to read input");
		return 1;
	}

	char *json_text = qm_run(input);
	puts(json_text);
	free(json_text);
	free(input);
	return 0;
}

static char *read_stdin() {
    size_t cap = 4096;
    size_t len = 0;
    char *buf = malloc(cap);
    if (!buf) return NULL;

    int c;
    while ((c = getchar()) != EOF) {
        buf[len++] = (char)c;

        if (len + 1 >= cap) {
            cap *= 2;
            char *tmp = realloc(buf, cap);
            if (!tmp) {
                free(buf);
                return NULL;
            }
            buf = tmp;
        }
    }

    buf[len] = '\0';
    return buf;
}

