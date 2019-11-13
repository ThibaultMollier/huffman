#include "huffmantree.h"

struct noeud* creer_feuille(int tab[256],int index){
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

size_t creer_arbre(int tab[256], struct noeud* arbre[]){
    int y=0;
    for (int i = 0; i < 256; i++)
    {
        if (tab[i]!=0)
        {
            arbre[y]=creer_feuille(tab,i);
            y++;
        }
    }
    return y;
}