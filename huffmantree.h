#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <stdlib.h>
#include <stdio.h>

#define DEBUG

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
void creer_code(struct noeud* element, int code, int niveau, struct noeud* tab[256]);
void affichage_code(int nbr_bits, int code);

#endif