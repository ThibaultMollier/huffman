#include <stdio.h>


int main(){
    FILE *pfile;
    char buffer;

    puts("Compression en codage de Huffman");
    puts("--------------------------------");

    pfile=fopen("file.txt","r");
    if (pfile)
    {
        while (buffer!=EOF)
        {
            buffer=fgetc(pfile);
            printf("%c",buffer);
        }
        
    }
    
    fclose(pfile);

}