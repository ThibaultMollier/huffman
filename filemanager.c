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

    if (p == NULL)
    {
        perror("ERROR filemanager.c writeHeader ");
        exit(1);
    }
    

    size_t headerSize = npi(element,p,&header,&shift);
    shift--;
    if (shift < 0)
    {
        fwrite(&header,1,1,p);
        headerSize++; 
        header=0;
        shift=8+shift;
        header |= (1<<shift);
    }
    header |= (1<<shift);
    fwrite(&header,1,1,p);
    headerSize++;

    return headerSize;
}

size_t npi(struct noeud* element,FILE* p,uint8_t* pheader,uint8_t* pshift){
    static size_t headerSize = 0;
    static int8_t shift = 8;
    static uint8_t header = 0;

    if (element->gauche == NULL && element->droite == NULL)
    {
        shift--;
        if (shift < 0)
        {
            fwrite(&header,1,1,p);
            headerSize++; 
            header=0;
            shift=8+shift;
            header &= ~(1 << shift);
        }else
        {
            header &= ~(1 << shift);
        }
        
        shift-=8;

        if (shift < 0)
        {
            if(shift != -8) header |= (element->c >> (-shift));
            fwrite(&header,1,1,p);
            headerSize++; 
            header=0;
            shift = 8 + shift;
            header |= (element->c << shift);
        }else
        {
            header = element->c;
        }               
    }else
    {
        npi(element->droite,p,pheader,pshift);
        npi(element->gauche,p,pheader,pshift);
        shift--;
        if (shift < 0)
        {
            fwrite(&header,1,1,p);
            headerSize++; 
            header=0;
            shift=8+shift;
            header |= (1<<shift);
        }else
        {
            header |= (1<<shift);
        }
    }
    *pheader = header;
    *pshift = shift;
    return headerSize;
}

size_t compressFile(FILE* pIn, FILE* pOut, struct noeud* alphabet[256],int nbr_Char){
    size_t fileSize = 0;
    uint8_t buff =0 ; 
    int8_t shift = 7;
    uint8_t write = 0;

    if (pIn == NULL || pOut == NULL) 
    {
        perror("ERROR filemanager.c compressFile ");
        exit(1);
    }
    for (int i = 0; i < nbr_Char; i++)
    {    
        fread(&buff,1,1,pIn);

        for(int16_t h = (alphabet[buff]->bits - 1) ; h >= 0 ; h--){
            write |= ((alphabet[buff]->code & (1 << h)) !=0 ) << shift;
            shift--;
            if (shift < 0)
            {
                fwrite(&write,sizeof(uint8_t),1,pOut);
                fileSize++;
                shift=7;
                write=0;
            }
        }
    }
    fwrite(&write,sizeof(uint8_t),1,pOut);
    fileSize++;
    
    return fileSize;
}

void readHeader(struct noeud* arbre_dec[256],FILE* pCompressedFile){
    uint8_t buffDec = 0; 
    int16_t i =0;
    uint8_t c = 0;
    int8_t shift = 8;

    if (pCompressedFile == NULL) 
    {
        perror("ERROR filemanager.c readHeader ");
        exit(1);
    }

    #ifdef DEBUG
        puts("INFO - Reconstructed tree :");
    #endif
    fread(&buffDec,sizeof(uint8_t),1,pCompressedFile);
    while (i >= 0)
    {    

        shift--;
        if (shift < 0)
        {
            fread(&buffDec,sizeof(uint8_t),1,pCompressedFile);
            shift = 8 + shift;
        }

        #ifdef DEBUG
            printf("%d ",(buffDec & (1<<shift)) > 0);
        #endif

        if (buffDec & (1<<shift))
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
        }
        else
        {
            shift-=8;

            if (shift < 0)
            {
                c = buffDec << (-shift);
                fread(&buffDec,sizeof(uint8_t),1,pCompressedFile);
                shift = 8 + shift;
                c |= buffDec >> shift;
            }else
            {
                fread(&buffDec,sizeof(uint8_t),1,pCompressedFile);
                c = buffDec;
            }
            
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
        }
    } 
}

void decompressFile(struct noeud* arbre, FILE* pIn,FILE* pOut,uint16_t nbr_Char){
    uint8_t buff = 0;
    int8_t shift = -1;
    uint16_t i =0;
    struct noeud* element = arbre;

    if (pIn == NULL || pOut == NULL) 
    {
        perror("ERROR filemanager.c decompressFile ");
        exit(1);
    }

    puts("\n------TEXT------\n");

    while (i < nbr_Char)
    {
        if (shift < 0)
        {
            fread(&buff,1,1,pIn);
            shift=7;
        }

        if (buff & (1 << shift))
        {
            element = element->droite;
        }else
        {
            element = element->gauche;
        }

        if (element->gauche == NULL && element->droite == NULL)
        {
            putc(element->c,pOut);
            printf("%c",element->c);
            element = arbre;
            i++;
        }
        
        shift--;
    }

}