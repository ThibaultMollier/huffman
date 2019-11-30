#include <stdio.h>
#include <stdlib.h>

#include "filemanager.h"
#include "huffmantree.h"

int main(int argc, char* argv[]){
    FILE* pfile;
    int tab_caractere[256];
    
    puts(" _______  _______  _______  _______  _______  _______  _______ ");
    puts("|   |   ||   |   ||    ___||    ___||   |   ||   _   ||    |  |");
    puts("|       ||   |   ||    ___||    ___||       ||       ||       |");
    puts("|___|___||_______||___|    |___|    |__|_|__||___|___||__|____|");
    puts("");

    if (argc>1)
    {
        pfile=fopen(argv[1],"r");
    }else
    {
        pfile=fopen("file.txt","r");
    }   
    
    occurence(pfile,tab_caractere);

    struct noeud* arbre_huffman[256];

    size_t s = creer_feuille(tab_caractere,arbre_huffman);

    while (s!=0)
    {
        creer_noeud(arbre_huffman,s);
        s--;
    }    

    struct noeud* alphabet[256];

    printf("INFO - %d bytes \n",arbre_huffman[0]->occurence);
    puts("-------------------------------------------------");
    creer_code(arbre_huffman[0],0,0,alphabet);
    puts("-------------------------------------------------");

    
    /*
    for (int i = 0; i < 256; i++)
    {
        free(arbre_huffman[i]);
    }*/
    return 0;
}