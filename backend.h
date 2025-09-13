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

#define ENT ".ent"
#define EXT ".ext"
#define OB ".ob"


#define BASE64CHARS 64
#define MASK 63




/*------------------------------------------------------*/
typedef struct labelInfo{
    char labelName[MAXLABELNAMELENGTH];

    int declarationBit; /*if the label is entry, extern, it should be declared AND defined: if declared: the bit wil light up, and the defintions will be trune it off. if if didnt declared at all: turned off forever*/

    int entryBit;
    int externBit;

    int definitionAddress;

    int isDataOrString;
}labelInfo;

typedef struct debt{
    char labelName[MAXLABELNAMELENGTH];
    int* debtLocation;

    int iForInstruction;

    int entryBit;
    int externBit;

}debt;


static void toOB(FILE* fdOB,int* array, int numOfEntries);
