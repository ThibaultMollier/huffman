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

void creer_noeud(struct noeud* tab[256],int taille){
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
    struct noeud* pnoeud=(struct noeud*)malloc(sizeof(struct noeud));
    pnoeud->gauche = tab[0];
    pnoeud->droite =tab[1];
    pnoeud->c='!';
    pnoeud->occurence = tab[0]->occurence + tab[1]->occurence;
    pnoeud->code = 0;
    pnoeud->bits = 0;
    tab[1] = pnoeud;

    //decale tout le tableau de 1
    for (int i = 0; i < taille-1; i++)
    {
        tab[i]=tab[i+1];
    }
}

void creer_code(struct noeud* element, int code, int niveau, struct noeud* tab[256]){
    if (element->droite==NULL && element->gauche==NULL){
        element->bits=niveau;
        element->code=code;
        tab[(int)element->c]=element;
        printf("%c\t%d\t",element->c,element->occurence);
        affichage_code(element->code);
    }
    else
    {
        creer_code(element->droite,code<<1,niveau+1,tab);
        creer_code(element->gauche,(code<<1)+1,niveau+1,tab);
    }
    
}

void affichage_code(int code){

    do
    {
        if (code & 1){
            printf("1");
        }
        else{
            printf("0");
        }
        code >>= 1;
    } while (code);
    
    printf("\n");
}