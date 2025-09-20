#include<string.h>
#include<stdlib.h>
#include "helper.h"

char** make_line(int width[] , int widthcount , const char* style , int byte){
	char **line = malloc(widthcount * sizeof(*line));
	if(line == NULL) return NULL;

	for(int i = 0; i < widthcount; i++){
		int n = width[i];
		char *str = malloc(((n*byte)+1) * sizeof(*str));
		if(str == NULL) return NULL;

		for (int j = 0; j < n; j++) {
			memcpy(&str[j*byte], style, byte);
		}
		str[n*byte] = '\0';
		line[i] = str;
	}
	return line;
}

