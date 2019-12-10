#include "main.h"

#define DEBUG 1

int main(int argc, char* argv[]){
    FILE* pfile;
    FILE* pfileOut;

    int tab_caractere[256];
    struct noeud* arbre_huffman[256] = {NULL};
    struct noeud* alphabet[256] = {NULL};

    
    puts(" _______  _______  _______  _______  _______  _______  _______ ");
    puts("|   |   ||   |   ||    ___||    ___||   |   ||   _   ||    |  |");
    puts("|       ||   |   ||    ___||    ___||       ||       ||       |");
    puts("|___|___||_______||___|    |___|    |__|_|__||___|___||__|____|\n");

    if (argc>1)
    {
        pfile=fopen(argv[1],"r");
    }else
    {
        pfile=fopen("file.txt","r");
    }   
    
    occurence(pfile,tab_caractere);


    uint8_t s = creer_feuille(tab_caractere,arbre_huffman);
    uint8_t nbr_char = s;

    while (s!=0)
    {
        creer_noeud(arbre_huffman,s);
        s--;
    }

    fclose(pfile);     

    printf("INFO - %d bytes \n",arbre_huffman[0]->occurence);
    puts("-------------------------------------------------");
    creer_code(arbre_huffman[0],0,0,alphabet);
    puts("-------------------------------------------------");


    pfileOut = fopen("out.txt","w");
    fwrite(&nbr_char,sizeof(uint8_t),1,pfileOut);
    fwrite(&arbre_huffman[0]->occurence,sizeof(arbre_huffman[0]->occurence),1,pfileOut);

    for (uint16_t i = 0; i < 256; i++)
    {
        if (alphabet[i]!=NULL)
        {
            uint32_t entete = alphabet[i]->code << 16 | (uint8_t)alphabet[i]->bits << 8 | alphabet[i]->c;
            fwrite(&entete,sizeof(uint32_t),1,pfileOut);
        }

    }

    char buff ; 
    int8_t shift = 8;
    int8_t write = 0;

    FILE* pInFile = fopen("file.txt","r");

    while(buff!=EOF){
        buff = getc(pInFile);

        if (buff==-1)
        {
            break;
        }
        
        shift = shift - alphabet[buff]->bits;
        
        if (shift < 0)
        {
            write |= alphabet[buff]->code >> abs(shift);
            fwrite(&write,sizeof(int8_t),1,pfileOut);
            shift = 8 + shift;
            write = 0;
            write |= alphabet[buff]->code << shift;
        }else{
            write |= alphabet[buff]->code << shift;
        } 
        
    }
    fwrite(&write,sizeof(int8_t),1,pfileOut);
    fclose(pInFile);
    fclose(pfileOut);
    

    
    #ifdef DEBUG
    FILE * p = fopen("out.txt","r");

    char realphabet[256];
    char bufftest ; 
    int t =0;
    while(bufftest!=EOF){
        bufftest = getc(p);
        if (bufftest==-1) break;
        
        realphabet[t++]=bufftest;
    }
    
    printf("%d\n",t);

    while (t)
    {
        printf("%d\n",realphabet[--t]);
        //affichage_code(8,realphabet[--t]);
    }
    
        

    fclose(p);
    


    

    #endif
    puts("\nEND");
    return 0;
}