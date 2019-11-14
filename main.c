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

    size_t s = creer_feuille(tab_caractere,arbre_huffman);

    while (s!=1)
    {
        creer_noeud(arbre_huffman,s);
        s--;
    }    

    struct noeud* alphabet[256];
    creer_code(arbre_huffman[0],0,0,alphabet);
    puts("-------------------------------------------------");

    
    
    for (int i = 0; i < 256; i++)
    {
        free(arbre_huffman[i]);
    }
    return 0;
}