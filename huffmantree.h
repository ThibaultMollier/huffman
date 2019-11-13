#include <stdlib.h>

struct noeud
{
    unsigned char c;
    unsigned int occurence;
    int code;
    int bits;
    struct noeud *gauche, *droite;   
};

struct noeud* creer_feuille(int tab[256],int index);
size_t creer_arbre(int tab[256], struct noeud* arbre[]);