#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

extern char *use;

/** Copy nBytes bytes from the origin file to the destination file.
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
int copynFile(FILE * origin, FILE * destination, int nBytes)
{
	// Complete the function
    int k, j;
    char* bytes=malloc(nBytes);

    j = fread(bytes, 1, nBytes, origin);
    k = fwrite(bytes, 1, j, destination);

    free(bytes);

    if(j!=k)
	    return -1;
    else
        return k;
}

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor
 *
 * The loadstr() function must allocate memory from the heap to store
 * the contents of the string read from the FILE.
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc())
 *
 * Returns: !=NULL if success, NULL if error
 */
char* loadstr(FILE * file)
{
  int i = -1;
  char aux;
  char* string;

  do{
    fread(&aux, 1, 1, file);
    i++;
  } while(aux != '\0' && aux !=EOF);

  string = malloc(i);
  fseek(file, -(i+1), SEEK_CUR);
  fread(string, 1, i, file);
  fseek(file, 1, SEEK_CUR);     

	return string;
}

/** Read tarball header and store it in memory.
 *
 * tarFile: pointer to the tarball's FILE descriptor
 * nFiles: output parameter. Used to return the number
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */
stHeaderEntry* readHeader(FILE * tarFile, int *nFiles)
{
	// Complete the function
  fread(nFiles, sizeof(int), 1, tarFile);

  stHeaderEntry* header = malloc(sizeof(stHeaderEntry)*(*nFiles));
  char* auxPtr;

  int i;
  for(i = 0; i < *nFiles; i++){
    auxPtr = loadstr(tarFile);

    header[i].name = malloc(strlen(auxPtr)+1);
    strcpy(header[i].name, auxPtr);
    fread(&(header[i].size), sizeof(int), 1, tarFile);
  }

	return header;
}

/** Creates a tarball archive
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE.
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive.
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size)
 * pairs occupy strlen(name)+1 bytes.
 *
 */
int createTar(int nFiles, char *fileNames[], char tarName[])
{
    // Complete the function
    FILE* tarFile = fopen(tarName, "w+");

    // 1.- Reserve space for the header
    fseek(tarFile, 4, SEEK_SET);                                   //Space for the int with the file size

    int i;
    for(i = 0; i < nFiles; i++)
        fseek(tarFile, strlen(fileNames[i])+5, SEEK_CUR);           //Space for the name (and '\0') and an int with the size

    // 2.- Dump the contents of the source files in tarball
    stHeaderEntry* header = malloc(nFiles*sizeof(stHeaderEntry));

    for(i = 0; i < nFiles; i++){

        // 2.1.- Fill header structure in memory with the file name
        header[i].name = malloc(strlen(fileNames[i])+1);
        strcpy(header[i].name, fileNames[i]);
        header[i].size = 0;

        //2.2.- Copy the file and fill the file size
        FILE* inputFile = fopen(fileNames[i], "r");

        int nCopiedBytes = copynFile(inputFile, tarFile, CP_BLOCK_SIZE);

        while(nCopiedBytes==CP_BLOCK_SIZE){
          header[i].size += CP_BLOCK_SIZE;
          nCopiedBytes = copynFile(inputFile, tarFile, CP_BLOCK_SIZE);
        }
        header[i].size += nCopiedBytes;

        fclose(inputFile);
    }

    // 3.- Fill the header properly
    rewind(tarFile);
    //fseek(tarFile, 0, SEEK_SET);

    fwrite(&nFiles, sizeof(int), 1, tarFile);

    for(i = 0;i < nFiles; i++){
      fwrite(header[i].name, 1, strlen(header[i].name)+1, tarFile);
      fwrite(&header[i].size, sizeof(int), 1, tarFile);
    }

    // 4.- Close streams and free the memory used
    free(header);
    fclose(tarFile);

	return EXIT_SUCCESS;
}

/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathname
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE.
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the
 * tarball's data section. By using information from the
 * header --number of files and (file name, file size) pairs--, extract files
 * stored in the data section of the tarball.
 *
 */
int extractTar(char tarName[])
{
	// Complete the function
  FILE* tarFile = fopen(tarName, "r");

  int nFiles, i;
  stHeaderEntry* header = readHeader(tarFile, &nFiles);

  for(i=0; i < nFiles; i++){
      FILE* outFile = fopen(header[i].name, "w+");

      copynFile(tarFile, outFile, header[i].size);
      fclose(outFile);
  }

  free(header);
	return EXIT_SUCCESS;
}
