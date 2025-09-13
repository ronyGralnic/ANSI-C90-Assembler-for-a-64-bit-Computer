/*the IDs the preprocessors uses:*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define AS ".as"
#define AM ".am"

#define TRUE 1
#define FALSE 0
#define MAXLINESIZE 81/*80 chars, plus 1 for '\n'*/


typedef struct macro{
    char name[MAXLINESIZE];
    fpos_t location;
}macro;


/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

static int macroNameProcessing(char* lineFromAS, char* macroNameBuffer );
static int validCommandName(char* lineFromAS);
static int endmcroProcessing(char* lineFromAS );
static int keyWords(char* macroNameBuffer);

