#ifdef _WIN32
#include <windows.h>
#endif

#include "memory_tracker.h"
#include <stdio.h>
#include <stdlib.h>
#include "qm_run.h"
#include "display_qm.h"

static char* get_input();

int main()
{
	#ifdef _WIN32
	system("cls");
    SetConsoleOutputCP(CP_UTF8);
    #endif

	char *input = get_input();
	if(!input){
		puts("Failed to read input");
		return 1;
	}

//	char *json_text = qm_run(input);
//	puts(json_text);
//	free(json_text);
	display_qm(input);
	free(input);
	return 0;
}

static char* get_input(){
	size_t capacity = 200;
	size_t size = 0;
	char *buffer = malloc(capacity * sizeof(*buffer));
	if(buffer == NULL) return NULL;

	int ch;
	while((ch = getchar()) != '\n' && ch != EOF)
	{
		if (size >= capacity-1)
		{
			capacity = 2;
			char* temp = realloc(buffer , capacity * sizeof(*temp));
			if(!temp)
			{
				while ((ch = getchar()) != EOF);
				free(buffer);
				return NULL;
			}
			buffer = temp;
		}
		buffer[size++] = (char)ch;
	}

	buffer[size] = '\0';
	if(!size){ free(buffer); return NULL; }

	return buffer;
}

