#include "huffmantree.h"

size_t creer_feuille(int tab[256], struct noeud* arbre[]){
    int y=0;
    for (int i = 0; i < 256; i++)
    {
        if (tab[i]!=0)
        {
            arbre[y] = (struct noeud*)malloc(sizeof(struct noeud));
            arbre[y]->c=i;
            arbre[y]->occurence=tab[i];
            arbre[y]->gauche=NULL;
            arbre[y]->droite=NULL;
            arbre[y]->bits=0;
            arbre[y]->code=0;
            y++;
        }
    }
    return y;
}

void creer_noeud(struct noeud* tab[],int taille){
    struct noeud* temp;
    for (size_t i = 0; i < taille; i++)
    {
        for (size_t j = i+1; j < taille; j++)
        {
            if (tab[j]->occurence < tab[i]->occurence)
            {
                temp = tab[j];
                tab[j]=tab[i];
                tab[i]=temp;
            }   
        }
    }
    temp=(struct noeud*)malloc(sizeof(struct noeud));
    temp->gauche = tab[0];
    temp->droite =tab[1];
    temp->c='!';
    temp->occurence = (*tab[0]).occurence + (*tab[1]).occurence;
    temp->code = 0;
    temp->bits = 0;
    tab[1] = temp;

    //decale tout le tableau de 1
    for (int i = 0; i < taille-1; i++)
    {
        tab[i]=tab[i+1];
    }
    
    
}