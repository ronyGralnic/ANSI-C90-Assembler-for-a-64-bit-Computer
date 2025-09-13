/*those IDs stuff the middle end based of*/
/*                          includes                   */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*------------------------------------------------------*/
/*                          DEFINITIONS                   */
#define MAXLABELNAMELENGTH 32
#define MAXIDENTIFIERNAMELERNGTH 7 /*mayne string, extern, mov*/
#define MAXLINESIZE 82 /*81 chars plus 1 for '\n'*/
#define TRUE 1
#define FALSE 0
#define BASE 100


/*                       enums,structures && unions                */
enum{mov,cmp,add,sub,not,clr,lea,inc,dec,jmp,bne,red,prn,jsr,rts,stop};

/*-------------------------------------------------------------------*/

typedef struct debt{
    char labelName[MAXLABELNAMELENGTH];
    int* debtLocation;

    int iForInstruction;

    int entryBit;
    int externBit;

}debt;

/*-------------------------------------------------------------------*/

typedef struct labelInfo{
    char labelName[MAXLABELNAMELENGTH];

    int declarationBit; /*if the label is entry, extern, it should be declared AND defined: if declared: the bit wil light up, and the defintions will be trune it off. if if didnt declared at all: turned off forever*/

    int entryBit;
    int externBit;

    int definitionAddress;

    int isDataOrString;
}labelInfo;

/*-------------------------------------------------------------------*/

typedef struct entryOrExternPackage{
    char identifier[7];/*type: extern or entry*/
    char labelName[MAXLABELNAMELENGTH];
}entryOrExternPackage;


typedef struct commandPackage{

    char identifier[8];/*"command"*/

    int labelBit;
    char labelName[MAXLABELNAMELENGTH];

    char commandName[5]; /*stop command has the longest name*/
    int sourceBit;
    char sourceIdentifier[9];/*"register" at max*/
    char sourceContent[MAXLINESIZE];
    int destinationBit;
    char destinationIdentifier[9];/*"register" at max*/
    char destinationContent[MAXLINESIZE];

    int numOfParameters;

}commandPackage;

typedef struct dataOrString{
    char identifier[7];/*string or data*/
    char labelName[MAXLABELNAMELENGTH];
    char parameters[MAXLINESIZE];
    int numOfParameters;
}dataOrStringPackage;

/*gather all structs above into a union of packages:*/
union packages{
    entryOrExternPackage entryOrExternEntry;
    commandPackage commandEntry;
    dataOrStringPackage dataOrStringEntry;

};

/*                          DECLARATIONS                   */
int preprocessor(char* fileName);

int frontend(char* fileName,union packages** packagesArr,int *numOfPackages);

int backend(char* fileName,int* dataSection, int numOfDataSection, int* instructionSection, int numOfInstructionSection,labelInfo *labelsArray,int numOfLabels, int numOfEntry,int numOfExtern, debt* debtTable, int numOfDebt);


static int ASinTheName(char* argv);

static int processingEntry(union packages entryOrExternPackage,labelInfo** labelsArray,int* numOfLabelsInLabelsArr, int* numOfEntry);

static int processingExtern(union packages entryOrExternPackage,labelInfo** labelsArray,int* numOfLabelsInLabelsArr, int* numOfExtern);

static int processingString(union packages dataOrStringPackage,labelInfo** labelsArray,int* numOfLabelsInLabelsArr, int** dataSection, int* numOfDataSection);

static int processingData(union packages dataOrStringPackage,labelInfo** labelsArray,int* numOfLabelsInLabelsArr,int** dataSection, int* numOfDataSection);

static int processingCommand(union packages commandPackage,labelInfo** labelsArray,int* numOfLabelsInLabelsArr,int** instructionSection,int* numOfInstructionSection , debt** debtsTable, int* numOfDebts);
/*------------------------------------------------------*/

