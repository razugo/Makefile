#ifndef PRINT_H
#define PRINT_H
#include <dirent.h>
#include <sys/types.h>

void listdir(char *name, char * filename, char * command);
void determine(char * name, char * filename, struct dirent *entry, char * command);
DIR * processDirectory(DIR * dir, char * name, char * filename, char * command, struct dirent * entry);
void printFIle(char * name, struct dirent* entry);
struct dirent * findEntry(int oldID, char*oldName, DIR *dir);
int compareDirent(int oldID, char *oldName, struct dirent * second);
void operation(	char * name, char * filename, struct dirent *entry, char * command);
char * directoryName(char * name);
void print(char * filename, char * name);

#endif
