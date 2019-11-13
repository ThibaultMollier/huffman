#include "filemanager.h"

void occurence(FILE* file, int tab[256]){
    char buffer;
    
    for (int i = 0; i < 256; i++)
    {
        tab[i]=0;
    }
    
    file=fopen("file.txt","r");
    if (file)
    {
        while (buffer!=EOF)
        {
            buffer=fgetc(file);
            tab[(int)buffer]++;
        }
        
    }
    
    fclose(file);
}