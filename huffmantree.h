#include <stdlib.h>

struct noeud
{
    unsigned char c;
    unsigned int occurence;
    int code;
    int bits;
    struct noeud *gauche, *droite;   
};

struct noeud* creer_feuille(int* tab,int index);