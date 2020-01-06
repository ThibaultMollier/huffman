#include "main.h"

int main(int argc, char* argv[]){

    FILE* pInputFile;
    FILE* pOutputFile;
    char* inFileName = argv[2];
    char* outFileName = "out.txt";
    int tab_caractere[256];
    struct noeud* arbre_huffman[256] = {};
    struct noeud* alphabet[256] = {};

    puts(" _______  _______  _______  _______  _______  _______  _______ ");
    puts("|   |   ||   |   ||    ___||    ___||   |   ||   _   ||    |  |");
    puts("|       ||   |   ||    ___||    ___||       ||       ||       |");
    puts("|___|___||_______||___|    |___|    |__|_|__||___|___||__|____|\n");

    int opt = getopt(argc,argv,"cd");
    switch (opt) {
        case 'c':
            inFileName = argv[2];
            outFileName = "compressed.txt";

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
            
            break;
        case 'd':
            puts("\n---------------DECOMPRESSING---------------");
            inFileName = argv[2];
            outFileName = "decompressed.txt";

            FILE* pCompressedFile = fopen(inFileName,"r");
            FILE* pDecompressedFile = fopen(outFileName,"w");
            uint16_t nbr_c = 0;
            struct noeud* arbre_dec[256]={};

            fread(&nbr_c,sizeof(uint16_t),1,pCompressedFile);

            readHeader(arbre_dec,pCompressedFile);

            decompressFile(arbre_dec[0],pCompressedFile,pDecompressedFile,nbr_c);

            fclose(pCompressedFile);
            fclose(pDecompressedFile);
            break;
        default:
            fprintf(stderr, "Usage: %s [-c] [-d] name\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }

    puts("\n---------------END---------------");
    return 0;
}