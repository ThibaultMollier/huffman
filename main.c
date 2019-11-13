#include <stdio.h>
#include "filemanager.h"
#include <stdlib.h>

int main(){
    FILE* pfile;
    int tab_caractere[256];



    puts("Compression en codage de Huffman");
    puts("--------------------------------");

    pfile=fopen("file.txt","r");
    occurence(pfile,tab_caractere);

    struct noeud
    {
        unsigned char c;
        unsigned int occurence;
        int code;
        int bits;
        struct noeud *gauche, *droite;   
    };

    struct noeud* arbre_huffman[256];

    for (int i = 0; i < 256; i++)
    {
        static int y = 0;
        if (tab_caractere[i]!=0)
        {
            arbre_huffman[y] = (struct noeud*)malloc(sizeof(struct noeud));
            (*arbre_huffman[y]).c=i;
            (*arbre_huffman[y]).occurence=tab_caractere[i];
            (*arbre_huffman[y]).gauche=NULL;
            (*arbre_huffman[y]).droite=NULL;
            (*arbre_huffman[y]).bits=0;
            (*arbre_huffman[y]).code=0;
            y++;
        }
        
    }

    for (size_t i = 0; i < sizeof(arbre_huffman)/sizeof(struct noeud); i++)
    {
        printf("%c - %d\n",(*arbre_huffman[i]).c,(*arbre_huffman[i]).occurence);
    }
    
    

}