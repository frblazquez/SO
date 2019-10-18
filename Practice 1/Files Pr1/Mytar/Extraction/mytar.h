#ifndef _MYTAR_H
#define _MYTAR_H

#include <limits.h>

#define CP_BLOCK_SIZE 4096

typedef enum{
  NONE,
  ERROR,
  CREATE,
  EXTRACT
} flags;

typedef struct {
  char* name;
  unsigned int size;
} stHeaderEntry;

int createTar(int nFiles, char *fileNames[], char tarName[]);
int extractTar(char tarName[]);


#endif /* _MYTAR_H */
