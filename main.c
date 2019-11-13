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

        for (int i = 0; i < 256; i++)
    {
        static int y = 0;
        if (tab_caractere[i]!=0)
        {
            arbre_huffman[y]=creer_feuille(tab_caractere,i);
            y++;
        }
    }

    for (size_t i = 0; i < sizeof(arbre_huffman)/sizeof(struct noeud); i++)
    {
        printf("%c - %d\n",(*arbre_huffman[i]).c,(*arbre_huffman[i]).occurence);
    }
    
    

}