#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "huffmantree.h"
void occurence(FILE* file, int tab[256]);
size_t writeHeader(struct noeud* element,FILE* p);
size_t npi(struct noeud* element,FILE* p,uint8_t* pheader,uint8_t* pshift);
size_t compressFile(FILE* pIn, FILE* pOut, struct noeud* alphabet[256]);
void readHeader(struct noeud* arbre_dec[256],FILE* pCompressedFile);

#endif