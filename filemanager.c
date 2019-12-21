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