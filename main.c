#include <stdio.h>


int main(){
    FILE *pfile;
    char buffer[20];

    pfile=fopen("file.txt","r");
    fread(buffer,sizeof(char)*20,1,pfile);
    printf("%s\n",buffer);
    fclose(pfile);

}