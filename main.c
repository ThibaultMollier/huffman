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

    readHeader(arbre_huffman,pCompressedFile);

    fclose(pCompressedFile);


    puts("\n---------------END---------------");
    return 0;
}