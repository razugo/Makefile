#ifndef PARSE_H
#define PARSE_H

void run(int argc, char ** argv);
char * filename(int argc, char ** argv);
void processflag(int argc, char ** argv);
void processName(int argc, char ** argv);
void processflag2(int argc, char ** argv);
void print(char * filename, char * name);
void processcommand(char * filename, char * name, int argc, char ** argv);

#endif
