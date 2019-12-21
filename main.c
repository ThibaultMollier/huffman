#include "main.h"

int main(int argc, char* argv[]){
    FILE* pInputFile;
    FILE* pOutputFile;
    char* inFileName = "file.txt";
    char* outFileName = "out.txt";
    int tab_caractere[256];
    struct noeud* arbre_huffman[256] = {};
    struct noeud* alphabet[256] = {};

    
    puts(" _______  _______  _______  _______  _______  _______  _______ ");
    puts("|   |   ||   |   ||    ___||    ___||   |   ||   _   ||    |  |");
    puts("|       ||   |   ||    ___||    ___||       ||       ||       |");
    puts("|___|___||_______||___|    |___|    |__|_|__||___|___||__|____|\n");


    pInputFile=fopen(inFileName,"r");

    occurence(pInputFile,tab_caractere);

    uint8_t s = creer_feuille(tab_caractere,arbre_huffman);
    uint8_t nbr_char = s;

    while (s!=0)
    {
        creer_noeud(arbre_huffman,s);
        s--;
    }

    fclose(pInputFile);     

    printf("INFO - Input file : %d bytes \n",arbre_huffman[0]->occurence);
    puts("-------------------------------------------------");
    puts("\tchar.\tocc.\tcode");
    creer_code(arbre_huffman[0],0,0,alphabet);
    puts("-------------------------------------------------");

    uint32_t sizeCompressed = sizeof(int)+1;

    pOutputFile = fopen(outFileName,"w");

    fwrite(&(arbre_huffman[0]->occurence),sizeof(int16_t),1,pOutputFile);

    sizeCompressed += writeHeader(arbre_huffman[0],pOutputFile);

    pInputFile = fopen(inFileName,"r");
    
    sizeCompressed += compressFile(pInputFile,pOutputFile,alphabet);

    fclose(pInputFile);
    fclose(pOutputFile);

    #ifdef DEBUG
        printf("INFO - Compressed file : %d bytes \n",sizeCompressed);
    #endif   
    
    puts("\n---------------DECOMPRESSING---------------");

    FILE* pCompressedFile = fopen(outFileName,"r");
    struct noeud* arbre_dec[256]={};

    uint8_t buffDec = 0; 
    int16_t i =0;
    uint8_t c = 0;
    uint8_t shift = 0;
    uint16_t nbr_Char_Comp = 0;


    fread(&nbr_Char_Comp,sizeof(int16_t),1,pCompressedFile);
    #ifdef DEBUG
        puts("INFO - Reconstructed tree :");
    #endif
    fread(&buffDec,sizeof(uint8_t),1,pCompressedFile);
    while (i >= 0)
    {    
        #ifdef DEBUG
            printf("%d ",(buffDec & (1<<(7-shift)) > 0));
        #endif

        if (buffDec & (1<<(7-shift)))
        {   
            if (i < 2) break;
            struct noeud *pNoeud = (struct noeud*)malloc(sizeof(struct noeud)); 
            pNoeud->gauche=arbre_dec[i-2];
            pNoeud->droite=arbre_dec[i-1];
            pNoeud->c='!';
            pNoeud->code=0;
            pNoeud->occurence=0;
            pNoeud->bits=i;

            arbre_dec[i-2]=pNoeud;
            i--;
            shift=(shift+1) % 8;

        }
        else
        {
            
            c = buffDec << (shift+1);
            fread(&buffDec,sizeof(uint8_t),1,pCompressedFile);
            c |= buffDec >> (7-shift);
            
            #ifdef DEBUG
                printf("%c ",c);
            #endif   

            arbre_dec[i]=(struct noeud*)malloc(sizeof(struct noeud));
            arbre_dec[i]->c=c;
            arbre_dec[i]->droite=NULL;
            arbre_dec[i]->gauche=NULL;
            arbre_dec[i]->bits=0;
            arbre_dec[i]->occurence=0;
            arbre_dec[i]->code=0;

            i++;
            
            shift=(shift+1) % 8;
        }
    }  

    fclose(pCompressedFile);


    puts("\n---------------END---------------");
    return 0;
}