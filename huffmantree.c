#include "huffmantree.h"

struct noeud* creer_feuille(int* tab,int index){
    struct noeud* feuille;

    feuille = (struct noeud*)malloc(sizeof(struct noeud));
    (*feuille).c=index;
    (*feuille).occurence=tab[index];
    (*feuille).gauche=NULL;
    (*feuille).droite=NULL;
    (*feuille).bits=0;
    (*feuille).code=0;

    return feuille;
}
