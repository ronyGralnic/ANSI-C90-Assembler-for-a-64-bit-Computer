/*file generator
 *
 * pay attention to the .ent/.ext:
 * .ent: WHEN  the .entry actually DEFINED
 *
 * .ext: WHEN an .extern actually USED*/

#include "backend.h"
int backend(char* fileName,int* dataSection, int numOfDataSection, int* instructionSection, int numOfInstructionSection,labelInfo *labelsArray,int numOfLabels, int numOfEntry,int numOfExtern, debt* debtTable, int numOfDebt) {

    FILE *fdOB;
    FILE *fdENT;
    FILE *fdEXT;

    char* fileNameCopyENT=NULL;
    char* fileNameCopyEXT=NULL;
    char* fileNameCopyOB=NULL;

    int iForDebt;

    int iForLabels;


    if (numOfEntry > 0) {
        /*open the .ent:*/
        fileNameCopyENT = (char *) malloc(strlen(fileName) + strlen(ENT) + 1);
        if (fileNameCopyENT == NULL) {
            fprintf(stdout, "\nInner problem: malloc failed for file name copy\n");
            return FALSE;
        }
        strcpy(fileNameCopyENT, fileName);
        strcat(fileNameCopyENT, ENT);
        fdENT = fopen(fileNameCopyENT, "w");
        if (fdENT == NULL) {
            fprintf(stdout, "can't open .as file somewhat for %s, try again\n", fileName);
            return FALSE;
        }
        for (iForLabels = 0; iForLabels < numOfLabels; iForLabels++) {

            if (labelsArray[iForLabels].entryBit == TRUE) {
                if (labelsArray[iForLabels].isDataOrString == TRUE) {
                    fprintf(fdENT, "%s\t\t%d\n", labelsArray[iForLabels].labelName,
                            BASE + numOfInstructionSection + labelsArray[iForLabels].definitionAddress);
                }
                else{
                    fprintf(fdENT, "%s\t\t%d\n", labelsArray[iForLabels].labelName,
                            BASE+labelsArray[iForLabels].definitionAddress);
                }

                
            }
        }

    }
    /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/





    if (numOfExtern > 0) {
        /*open the .ext:*/
        fileNameCopyEXT = (char *) malloc(strlen(fileName) + strlen(EXT) + 1);
        if (fileNameCopyEXT == NULL) {
            fprintf(stdout, "\nInner problem: malloc failed for file name copy\n");
            return FALSE;
        }
        strcpy(fileNameCopyEXT, fileName);
        strcat(fileNameCopyEXT, EXT);
        fdEXT = fopen(fileNameCopyEXT, "w");
        if (fdEXT == NULL) {
            fprintf(stdout, "can't open .as file somewhat for %s, try again\n", fileName);
            return FALSE;
        }
        for (iForDebt = 0; iForDebt < numOfDebt; iForDebt++) {
            if (debtTable[iForDebt].externBit == TRUE) {
                fprintf(fdEXT, "%s\t\t%d\n",debtTable[iForDebt].labelName,
                        BASE+debtTable[iForDebt].iForInstruction);
            }
        }
    }

    /*open the .ob:*/
    fileNameCopyOB = (char *) malloc(strlen(fileName) + strlen(OB) + 1);
    if (fileNameCopyOB == NULL) {
        fprintf(stdout, "\nInner problem: malloc failed for file name copy\n");
        return FALSE;
    }
    strcpy(fileNameCopyOB, fileName);
    strcat(fileNameCopyOB, OB);
    fdOB = fopen(fileNameCopyOB, "w");
    if (fdOB == NULL) {
        fprintf(stdout, "can't open .as file somewhat for %s, try again\n", fileName);
        return FALSE;
    }



    fprintf(fdOB, "\n%d\t%d\n", numOfDataSection, numOfInstructionSection);
    toOB(fdOB, instructionSection, numOfInstructionSection);
    toOB(fdOB, dataSection, numOfDataSection);

    free(fileNameCopyENT);
    free(fileNameCopyEXT);
    free(fileNameCopyOB);
    fclose(fdOB);
    fclose(fdENT);
    fclose(fdEXT);

    return TRUE;


}
/*****************************************functions*******************************************************************/
/*Strategy: local array creation of base64 array, and a correspondent enum: for readability
 * working per a 12-bits-word, in which will be divided to per 6-bits-word: top and bottom
 * every 12-bit-word, will be masked with 0..0111111 to isolate to top/bottom so in their turn we will scan the base64 array, to find the corresponded character
 * ATTENTION: OUR ENUM AND THE INDEXES IN THE ARRAY ARE REALLY GOOD FRIENDS */
static void toOB(FILE* fdOB,int* array, int numOfEntries){
    enum{A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,zero,one,two,three,four,five,six,seven,eight,nine,plus,slash};
    /*63=0..0111111 so we've got a mask*/
    unsigned int topSixIsolation;
    unsigned int bottomSixIsolation;
    int iForArray;
    int iForBase;
    char base64[64];
    base64[A]='A';
    base64[B]='B';
    base64[C]='C';
    base64[D]='D';
    base64[E]='E';
    base64[F]='F';
    base64[G]='G';
    base64[H]='H';
    base64[I]='I';
    base64[G]='G';
    base64[K]='K';
    base64[L]='L';
    base64[M]='M';
    base64[N]='N';
    base64[O]='O';
    base64[P]='P';
    base64[Q]='Q';
    base64[R]='R';
    base64[S]='S';
    base64[T]='T';
    base64[U]='U';
    base64[V]='V';
    base64[W]='W';
    base64[X]='X';
    base64[Y]='Y';
    base64[Z]='Z';
    base64[a]='a';
    base64[b]='b';
    base64[c]='c';
    base64[d]='d';
    base64[e]='e';
    base64[f]='f';
    base64[g]='g';
    base64[h]='h';
    base64[i]='i';
    base64[j]='j';
    base64[k]='k';
    base64[l]='l';
    base64[m]='m';
    base64[n]='n';
    base64[o]='o';
    base64[p]='p';
    base64[q]='q';
    base64[r]='r';
    base64[s]='s';
    base64[t]='t';
    base64[u]='u';
    base64[v]='v';
    base64[w]='w';
    base64[x]='x';
    base64[y]='y';
    base64[z]='z';
    base64[zero]='0';
    base64[one]='1';
    base64[two]='2';
    base64[three]='3';
    base64[four]='4';
    base64[five]='5';
    base64[six]='6';
    base64[seven]='7';
    base64[eight]='8';
    base64[nine]='9';
    base64[plus]='+';
    base64[slash]='/';
    /*---------------------------------------------------------------------------------------------------------*/
    for(iForArray=0;iForArray<numOfEntries;iForArray++){
        topSixIsolation=63;
        bottomSixIsolation=63;
        topSixIsolation = topSixIsolation&(array[iForArray]>>6);
        for(iForBase=0;iForBase<64;iForBase++){
            if(topSixIsolation==iForBase/*INDEX!!*/){
                fputc(base64[iForBase],fdOB);
                break;
            }
        }
        bottomSixIsolation = bottomSixIsolation & array[iForArray];
        for(iForBase=0;iForBase<64;iForBase++){
            if(bottomSixIsolation==iForBase/*INDEX!!*/){
                fputc(base64[iForBase],fdOB);
                fputc('\n',fdOB);
                break;
            }
        }
    }



}
	
