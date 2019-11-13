#include <stdio.h>
#include <stdlib.h>

#include "filemanager.h"
#include "huffmantree.h"

int main(){
    FILE* pfile;
    int tab_caractere[256];

    puts("Compression en codage de Huffman");
    puts("--------------------------------");
    
    pfile=fopen("file.txt","r");
    
    occurence(pfile,tab_caractere);

    struct noeud* arbre_huffman[256];

    size_t s = creer_arbre(tab_caractere,arbre_huffman);
    

    for (int i = 0; i < s; i++)
    {
        printf("%c - %d\n",(*arbre_huffman[i]).c,(*arbre_huffman[i]).occurence);
    }
    
    for (int i = 0; i < 256; i++)
    {
        free(arbre_huffman[i]);
    }
}