#include "main.h"

int main(int argc, char* argv[]){

    FILE* pInputFile;
    FILE* pOutputFile;
    char* inFileName  = argv[2];
    char* outFileName = NULL;
    int tab_caractere[256];
    struct noeud* arbre_huffman[256] = {};
    struct noeud* alphabet[256] = {};
    int8_t flag = -1;
    int opt = 0;

    puts(" _______  _______  _______  _______  _______  _______  _______ ");
    puts("|   |   ||   |   ||    ___||    ___||   |   ||   _   ||    |  |");
    puts("|       ||   |   ||    ___||    ___||       ||       ||       |");
    puts("|___|___||_______||___|    |___|    |__|_|__||___|___||__|____|\n");

    while ((opt = getopt(argc,argv,"cdo:"))!=-1)
    {
        switch (opt) {
        case 'o':
            inFileName  = argv[3];
            outFileName = argv[4];
            break;
        case 'c':
            flag=1;
            break;
        case 'd':
            flag=0;
            break;
        default:
            fprintf(stderr, "Usage: huffman [-c -d] [-o] inputfile outputfile\n");
            exit(EXIT_FAILURE);
        }
    }

    if (flag == 1)
    {
        puts("\n---------------COMPRESSING---------------");

        if(outFileName == NULL) outFileName = "compressed.txt";

        pInputFile=fopen(inFileName,"r");
        if(pInputFile == NULL){
            perror("ERROR main.c");
            exit(1);
        }

        occurence(pInputFile,tab_caractere);

        uint8_t s = creer_feuille(tab_caractere,arbre_huffman);

        while (s!=0)
        {
            creer_noeud(arbre_huffman,s);
            s--;
        }

        fclose(pInputFile);     

        uint32_t filesize = arbre_huffman[0]->occurence;

        printf("INFO - Input file : %d bytes \n",filesize);
        
        puts("-------------------------------------------------");
        puts("\tchar.\tocc.\tcode");
        creer_code(arbre_huffman[0],0,0,alphabet);
        puts("-------------------------------------------------");

        uint32_t sizeCompressed = sizeof(int16_t);

        pOutputFile = fopen(outFileName,"w");

        fwrite(&(arbre_huffman[0]->occurence),sizeof(int16_t),1,pOutputFile);

        sizeCompressed += writeHeader(arbre_huffman[0],pOutputFile);

        pInputFile = fopen(inFileName,"r");
        
        sizeCompressed += compressFile(pInputFile,pOutputFile,alphabet,arbre_huffman[0]->occurence);

        fclose(pInputFile);
        fclose(pOutputFile);

        #ifdef DEBUG
            float ratio = (float)filesize/sizeCompressed;
            printf("INFO - Compressed file : %d bytes \n\t Ratio : %f\n",sizeCompressed,ratio);
        #endif
    }else if(flag == 0)
    {
        puts("\n---------------DECOMPRESSING---------------");

        if(outFileName == NULL) outFileName = "decompressed.txt";

        uint16_t nbr_Char = 0;

        pInputFile = fopen(inFileName,"r");
        pOutputFile = fopen(outFileName,"w");

        if(pInputFile == NULL){
            perror("ERROR main.c");
            exit(1);
        }

        fread(&nbr_Char,sizeof(uint16_t),1,pInputFile);

        readHeader(arbre_huffman,pInputFile);

        decompressFile(arbre_huffman[0],pInputFile,pOutputFile,nbr_Char);

        fclose(pInputFile);
        fclose(pOutputFile);
    }else
    {
        fprintf(stderr, "Usage: huffman [-c | -d] [-o] inputfile outputfile\n");
        exit(EXIT_FAILURE);
    }
      

    puts("\n---------------END---------------");
    return 0;
}