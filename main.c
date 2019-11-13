#include <stdio.h>
#include "filemanager.h"

int main(){
    FILE* pfile;
    int tab_caractere[256];

    puts("Compression en codage de Huffman");
    puts("--------------------------------");

    occurence(pfile,tab_caractere);
    for (int i = 97; i < 123; i++)
    {
        printf("%c-%d\n",i,tab_caractere[i]);
    }
    

}