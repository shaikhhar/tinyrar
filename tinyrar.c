#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
   char filename[100];
   long filesize;
   char fileContent[1024*1024*5]; //max file 5MB
} structArchive;

/* struct with file to archive ftoa */
structArchive ftoa;

char *getFileNameFromfilePath(char *);
long getFileSizeFromfilePath(char *);
void readFile(FILE *);
void readBinaryFile(char *);
void appendToArchivedPath(structArchive, char *);
void listArchivedFile(char *);
char *getFileContentFromFilePath(char *);
void extractSelectedFile(char *, char *);
char *getDirFromfilePath(char *);

int main(int argc, char *argv[])
{
   char *option = argv[1];                   //option chosen 1st argument after ./tinytar
   char *archivedPath = argv[2];             //filefilePath of archivedPathd file
   char *fileToExtract = argv[3]; // filefilePath of file to be added or extracted
   char *filePath = argv[3];
   // printf("%s\n", option);
   // readFile(ftoa.fp);
   // readBinaryFile(filePath);

   if (strcmp(option, "-a") == 0)
   {
      // printf("Option chosen is a\n");
      strcpy(ftoa.filename, getFileNameFromfilePath(filePath));
      ftoa.filesize = getFileSizeFromfilePath(filePath);
      strcpy(ftoa.fileContent, getFileContentFromFilePath(filePath));
      printf("%s\n", ftoa.filename);
      printf("%ld\n", ftoa.filesize);
      appendToArchivedPath(ftoa, archivedPath);
   }
   else if (strcmp(option, "-l") == 0)
   {
      // printf("Option chosen is l \n");
      listArchivedFile(archivedPath);
   }
   else if (strcmp(option, "-e") == 0)
   {
      // printf("Option chosen is e\n");
      extractSelectedFile(archivedPath, fileToExtract);
   }
   return 0;
}
char *getFileNameFromfilePath(char *filePath)
{
   for (int i = strlen(filePath) - 1; i; i--)
   {
      if (filePath[i] == '/')
      {
         return &filePath[i + 1];
      }
   }
   return filePath;
}
char *getDirFromfilePath(char *filePath)
{
   int j;
   for (int i = strlen(filePath) - 1; i; i--)
   {
      if (filePath[i] == '/')
      {
         j = i;
         break;
      }
   }
   char *dir = (char *)malloc(sizeof(char) * j);
   for (int k = 0; k <= j; k++)
   {
      dir[k] = filePath[k];
   }
   return dir;
}

long getFileSizeFromfilePath(char *filePath)
{
   FILE *fp1 = fopen(filePath, "r");
   long prev = ftell(fp1);
   // printf("prev = %ld\n",prev);
   fseek(fp1, 0L, SEEK_END);
   long sz = ftell(fp1);
   fseek(fp1, prev, SEEK_SET);
   fclose(fp1);
   return sz;
}
void readFile(FILE *fp)
{
   char c;
   while ((c = fgetc(fp)) != EOF)
   {
      printf("%c", c);
   }
   fclose(fp);
}
void readBinaryFile(char *filePath)
{
   char c;
   FILE *fp2 = fopen(filePath, "rb");
   while ((c = fgetc(fp2) != EOF))
   {
      printf("%c", c);
   }
   fclose(fp2);
}
void appendToArchivedPath(structArchive ftoa, char *path)
{
   FILE *fp = fopen(path, "a");                                               
   if (fwrite(&ftoa, sizeof(ftoa), 1, fp))
   {
      printf("\nFile Successfully added\n");
   };
   fclose(fp);
}
void listArchivedFile(char *path)
{
   FILE *fp = fopen(path, "r");
   structArchive ftoa1;
   // printf("size of ftoa1 =%ld", sizeof(ftoa1));

   while (fread(&ftoa1, sizeof(ftoa1), 1, fp))
   {
      printf("%s\t%ld\n", ftoa1.filename, ftoa1.filesize);
   }
   fclose(fp);
}
char *getFileContentFromFilePath(char *path)
{
   FILE *fp = fopen(path, "r");
   int i = 0;
   char ch;
   int size = getFileSizeFromfilePath(path);
   char *fileContent = (char *)malloc(sizeof(char) * size+1);
   while ((ch = fgetc(fp)) != EOF)
   {
      fileContent[i] = ch;
      i++;
   }
   // printf("The File Content is %s", fileContent);
   return fileContent;
   fclose(fp);
}

void extractSelectedFile(char *path, char *name)
{
   FILE *fp = fopen(path, "r");
   structArchive ftoa1;
   while (fread(&ftoa1, sizeof(ftoa1), 1, fp))
   {
      char *filename = ftoa1.filename;
      if (strcmp(filename, name) == 0)
      {
         // printf("match found %s \n", name);
         char *dir = getDirFromfilePath(path);         
         char * fullPath = (char *) malloc(strlen(dir)+strlen(name)+1);
         strcpy(fullPath, dir);
         strcat(fullPath, name);
         // create file by name on path dir
         FILE *fp = fopen(fullPath, "w");
         char *fileContent = ftoa1.fileContent;
         fputs(fileContent, fp);
         printf("%s extracted to %s\n", name, dir);
         break;
      }
   }
   fclose(fp);
}