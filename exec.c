#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include "exec.h"
#include "print.h"

void processcommand(char * filename, char * name, int c, char ** argv)
{
	char command[] = {' ', '\0'};
	char space[] = {' ', '\0'};
	char semiColon[] = {';', '\0'};
	if(argv[c] == '\0')
	{
		fprintf(stderr, "find: missing argument to `-exec'\n");
		exit(1);
	}
	while(argv[c] != '\0' && strcmp(argv[c], semiColon) != 0)
	{
		if(argv[c] == '\0')
		{
			fprintf(stderr, "find: missing argument to `-exec'\n");
			exit(1);
		}
		strcat(command, space);
		strcat(command, argv[c]);
		c++;
	}
	if(argv[c] == '\0')
	{
		fprintf(stderr, "find: missing argument to `-exec'\n");
		exit(1);
	}
	if(argv[c+1] != '\0' )
	{
		fprintf(stderr, "find: paths must precede expression: %s\n", argv[c+1]);
		exit(1);
	}
	listdir(filename, name, trimwhitespace(command));
}
char *trimwhitespace(char *str)
{
	char *end;

	while(isspace((unsigned char)*str)) str++;

	if(*str == 0)
		return str;


	end = str + strlen(str) - 1;
	while(end > str && isspace((unsigned char)*end)) end--;

	*(end+1) = 0;

	return str;
}
char * replace_str(char *target, char *needle, char *replacement)
{
	static char buffer[sizeof(target)];
	char *insert_point = &buffer[0];
	const char *tmp = target;
	size_t needle_len = strlen(needle);
	size_t repl_len = strlen(replacement);

	while (1) 
	{
		const char *p = strstr(tmp, needle);
		if (p == NULL) 
		{
			strcpy(insert_point, tmp);
			break;
		}
		strncpy(insert_point, tmp, p - tmp);
		insert_point += p - tmp;

		strncpy(insert_point, replacement, repl_len);
		insert_point += repl_len;

		tmp = p + needle_len;
	}
	return buffer;
}
