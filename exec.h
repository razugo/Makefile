#ifndef EXEC_H
#define EXEC_H

#include <string.h>

void processcommand(char * filename, char * name, int c, char ** argv);
char *trimwhitespace(char *str);
char *replace_str(char *str, char *orig, char *rep);
char ** parseCommand(char * command);

#endif
