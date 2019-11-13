#include <stdlib.h>

struct noeud
{
    unsigned char c;
    unsigned int occurence;
    int code;
    int bits;
    struct noeud *gauche, *droite;   
};

size_t creer_feuille(int tab[256], struct noeud* arbre[]);
void creer_noeud(struct noeud* tab[],int taille);