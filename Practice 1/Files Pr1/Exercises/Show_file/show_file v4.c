#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define CHARS_PER_BLOCK 5

int main(int argc, char* argv[]) {
	FILE* file=NULL;
	int ret, k; 
    char c[CHARS_PER_BLOCK];

	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	/* Read file byte by byte */
    k=fread(c, 1,CHARS_PER_BLOCK,file);

	while (k > 0) {
		/* Print byte to stdout */
		fwrite(c, 1,k,stdout);

        k=fread(c, 1,CHARS_PER_BLOCK,file); 
	}

	fclose(file);
	return 0;
}
