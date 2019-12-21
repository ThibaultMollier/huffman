#include "filemanager.h"

void occurence(FILE* file, int tab[256]){
    int16_t buffer;

    for (int i = 0; i < 256; i++)
    {
        tab[i]=0;
    }
    
    if (file)
    {
        while (buffer!=EOF)
        {
            buffer=fgetc(file);
            if (buffer<0) break; 
            tab[(uint8_t)buffer]++;
        }
    }else
    {
        perror("ERROR filemanager.c occurence ");
        exit(1);
    }  
}

size_t writeHeader(struct noeud* element,FILE* p){
    uint8_t header = 0;
    uint8_t shift = 0;

    size_t headerSize = npi(element,p,&header,&shift);
    header |= (1<<(7-shift));
    fwrite(&header,1,1,p);
    
    return headerSize;
}

size_t npi(struct noeud* element,FILE* p,uint8_t* pheader,uint8_t* pshift){
    static size_t headerSize = 0;
    static uint8_t shift = 0;
    static uint8_t header = 0;

    if (element->gauche == NULL && element->droite == NULL)
    {
        header &= ~(1<<(7-shift));
        shift=(shift+1) % 8;
        header |= (element->c >> shift);
        fwrite(&header,1,1,p);
        header = (element->c << (8-shift));
        headerSize++;        
    }else
    {
        npi(element->gauche,p,pheader,pshift);
        npi(element->droite,p,pheader,pshift);
        header |= (1<<(7-shift));
        if (shift == 7)
        {
            fwrite(&header,1,1,p);
        }
        
        shift=(shift+1) % 8;
    }
    *pheader = header;
    *pshift = shift;
    return headerSize;
}

size_t compressFile(FILE* pIn, FILE* pOut, struct noeud* alphabet[256]){
    size_t fileSize = 0;
    int16_t buff =0 ; 
    int8_t shift = 8;
    int8_t write = 0;

    if (pIn == NULL || pOut == NULL) 
    {
        perror("ERROR filemanager.c compressFile ");
        exit(1);
    }
    

    while(buff!=EOF){
        buff = getc(pIn);
        if (buff<0) break;        
        shift = shift - alphabet[buff]->bits;
        
        if (shift < 0)
        {
            write |= alphabet[buff]->code >> abs(shift);
            fwrite(&write,sizeof(int8_t),1,pOut);
            fileSize++;
            shift = 8 + shift;
            write = 0;
            write |= alphabet[buff]->code << shift;
        }else{
            write |= alphabet[buff]->code << shift;
        } 
        
    }
    fwrite(&write,sizeof(int8_t),1,pOut);

    return fileSize;
}

void readHeader(struct noeud* arbre_dec[256],FILE* pCompressedFile){
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
}