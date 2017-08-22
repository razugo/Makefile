#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "parse.h"
#include "print.h"
#include "exec.h"
int OPERATION = 0;

void run(int argc, char ** argv)
{
	if(argc == 1)
	{
		char temp[] = {'.', '\0'};
		OPERATION = 1;
		print(temp, NULL);
	}
	else
	{
		filename(argc, argv);
		processflag(argc, argv);
	}
}
char * filename(int argc, char** argv)
{
	if(argv[1][0] == '-')
	{
		if(strcmp(argv[1], "-name") == 0 || strcmp(argv[1], "-exec") == 0)
		{
			fprintf(stderr, "find: missing argument to `%s'\n", argv[1]);
			exit(1);
		}
		else if(strcmp(argv[1], "-print") == 0)
		{
			char temp[] = {'.', '\0'};
			OPERATION = 1;
			print(temp, NULL);
			exit(0);
		}
		else
		{
			fprintf(stderr, "find: unknown predicate `%s'\n", argv[1]);
			exit(1);
		}
	}
	if(argc == 2)
	{
		OPERATION = 1;
		print(argv[1], NULL);
		exit(0);
	}
	return argv[1];
}
void processflag(int argc, char ** argv)
{
	if(strcmp(argv[2], "-name") == 0)
	{
		processName(argc, argv);
	}
	else if(strcmp(argv[2], "-print") == 0)
	{
		OPERATION = 1;
		print(argv[1], NULL);
	}
	else if(strcmp(argv[2], "-exec") == 0)
	{
		OPERATION = 2;
		processcommand(argv[1], NULL, 3, argv);
	}
	else if(argv[2][0] == '-')
	{
		fprintf(stderr, "find: unknown predicate `%s'\n", argv[2]);
		exit(1);
	}
	else
	{
		fprintf(stderr, "find: multiple names not supported\n");
		exit(1);
	}
}
void processName(int argc, char ** argv)
{
	if(argv[4] == '\0')
	{
		OPERATION = 1;
		print(argv[1], argv[3]);
		exit(0);
	}
	if(argv[3] == '\0')
	{
		fprintf(stderr, "find: missing argument to `-name'\n");
		exit(1);
	}
	processflag2(argc, argv);
}
void processflag2(int argc, char ** argv)
{
	if(strcmp(argv[4], "-print") == 0)
	{
		OPERATION = 1;
		print(argv[1], argv[3]);
	}
	else if(strcmp(argv[4], "-exec") == 0)
	{
		OPERATION = 2;
		processcommand(argv[1], argv[3], 5, argv);
	}
	else
	{
		fprintf(stderr, "find: paths must precede expression: %s\n", argv[4]);
		exit(1);
	}
}
