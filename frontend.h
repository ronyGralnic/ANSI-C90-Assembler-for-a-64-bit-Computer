/*frontend ammo in short:*/

/*                          includes                   */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
/*------------------------------------------------------*/


/*                         DEFINITIONS                   */
#define MAXLABELNAMELENGTH 32
#define MAXIDENTIFIERNAMELERNGTH 7 /*mayne string, extern, mov*/
#define MAXLINESIZE 82 /*81 chars plus 1 for '\n'*/
#define TRUE 1
#define FALSE 0

#define AM ".am"

/*------------------------------------------------------*/

/*                          enums, structures && unions                */
enum {
    ENTRY, EXTERN, COMMANDANDLABEL, COMMANDWITHOUTLABEL, STRING, DATA
};
enum{ONLYTHREE=3,THREEANDFIVE=35,EVERYTHING=135 };/*for comannd validtation parameters: used for the main function AND command validation func*/

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
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
static int processingEntryOrExternLine(char* lineFromAm, char* labelBuffer);
static int processingDataOrStringLine(char* lineFromAM,char* labelBuffer, char* parametersBuffer,int* numOfParameters);
static int processingCommand(char* lineFromAM, char* labelBuffer, char* commandBuffer, char* sourceBuffer, char* destinationBuffer );
static int threeLettersCommandValidation(char* commandBuffer,int* sourceBit, int* destinationBit, int* commaBit);
static int labeling(char lineFromAM[MAXLINESIZE], int iForLine ,char* buffer, int *toProceed);
static int numbering(char lineFromAM[MAXLINESIZE], int iForLine,char *buffer, int *toProceed);
static int registering(char* lineFromAM, int iForLine, char* buffer, int *toProceed);
static int commandsWithSource(char commandName[4]);
static int commandsWithDestination(char commandName[4]);

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


