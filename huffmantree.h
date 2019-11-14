#include <stdlib.h>
#include <stdio.h>

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
void creer_code(struct noeud* element, int code, int niveau);
void affichage_code(int code);