#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linux/limits.h>
#include "print.h"
#include "exec.h"
int DIRECTORY = 0;
int RECURSION = 0;
int compareDirent(int oldID, char *oldName, struct dirent * second)
{
	if(oldID!= second->d_ino)
	{
		return 0;
	}
	if(strcmp(oldName, second->d_name) != 0)
	{
		return 0;
	}
	return 1;
}
DIR * recursiveCall(DIR *dir, char* name, char * path, char * filename, char * command)
{
	closedir(dir);
	listdir(path, filename, command);
	return opendir(name);
}
DIR * processDirectory(DIR * dir, char * name, char * filename, char * command, struct dirent * entry)
{
	char path[PATH_MAX];
	int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
	path[len] = '\0';
	return recursiveCall(dir, name, path, filename, command);
}
void listdir(char *name, char * filename, char * command)
{
	DIR *dir;
	struct dirent *entry;
	struct stat temp;
	if (!(dir = opendir(name)))
	{
		RECURSION = 1;
		dir = opendir(".");
		if(stat(name, &temp) == -1)
		{
			fprintf(stderr, "find: '%s': No such file or directory\n", name);
			exit(-1);
		}
	}
	if (!(entry = readdir(dir)))
		return;
	DIRECTORY = 0;
	determine(name, filename, entry, command);
	do 
	{
		if (entry->d_type == 4 && RECURSION == 0) 
		{
			int oldID = entry->d_ino;
			char oldName[sizeof(entry->d_name)];
			strcpy(oldName, entry->d_name);
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				continue;
			dir = processDirectory(dir, name, filename, command, entry);
			entry = findEntry(oldID, oldName, dir);
		}
		else
		{
			DIRECTORY = 1;
			determine(name, filename, entry, command);
		} 
	}while ((entry = readdir(dir)));
	closedir(dir);
}
struct dirent * findEntry(int oldID, char*oldName, DIR *dir)
{
	struct dirent * entry = readdir(dir);
	while(!compareDirent(oldID, oldName, entry))
	{
		entry = readdir(dir);
	}
	return entry;
}


	
void printFile(char * name, struct dirent* entry)
{
	if(strcmp(name, entry->d_name) == 0 && RECURSION == 1)
	{
		printf("%s\n", directoryName(name));
	}
	else if(DIRECTORY == 0 && RECURSION == 0)
	{
		printf("%s\n", name);
	}
	else if(DIRECTORY == 1 && RECURSION == 0)
	{
		printf("%s/%s\n", name, entry->d_name);
	}
}
void determine(char * name, char * filename, struct dirent *entry, char * command)
{
	extern int OPERATION;
	if(filename == NULL || strstr(entry->d_name, filename) || (DIRECTORY == 0 && strstr(directoryName(name), filename)))
	{
		if(OPERATION == 1)
		{
				printFile(name, entry);
		}
		else if(OPERATION == 2)
		{
			if(strcmp(name, entry->d_name) == 0 && RECURSION == 1)
				operation(name, filename, entry, command);
			else if(RECURSION == 0)
				operation(name, filename, entry, command);
		}
	}
}

void operation(	char * name, char * filename, struct dirent *entry, char * command)
{		
	char tempCommand[sizeof(command)];
	char tempPath[PATH_MAX];
	char * runCommand;
	pid_t pid;
	int status;
	if(DIRECTORY == 0 || RECURSION == 1)
	{
		snprintf(tempPath, sizeof(tempPath) -1, "%s", name);
	}
	else if(DIRECTORY == 1)
	{
		snprintf(tempPath, sizeof(tempPath) -1, "%s/%s", name, entry->d_name);
	}
	strcpy(tempCommand, command);
	//printf("Command before: '%s'\n", tempCommand);
	runCommand = replace_str(tempCommand, "{}", tempPath);
	//printf("Command after: '%s'\n", runCommand);
	pid = fork();
	if(pid == -1)
	{
		fprintf(stderr, "failed to fork\n");
		exit(-1);
	}
	if(pid == 0)
	{
		
		execl("/bin/sh", "sh", "-c", runCommand, (char *) 0);
		_exit(-1);
	}
	else
	{
		while(waitpid(-1, &status, 0) < 0)
		{
			
		}
	}

}
char * directoryName(char * name)
{
	char * temp = strrchr(name, '/');
	if(temp != NULL)
	{
		return temp + 1;
	}
	return name;
}
void print(char * filename, char * name)
{
	listdir(filename, name, NULL);
}



