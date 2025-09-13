/*welcome to the heart of the project!
 * here you will find the winner on the TIZMORET, it goes like this:
 * 1. letting the preprocessor do his thing, giving a file name
 * so we have the .am
 * 2. every .am line will be packaged into structs, build epcifically to what each can offer:
 * entry/extern, command, data/string
 * all those packaged wii be chronologically ordered into a union array of these structs
 * so weve got packaged-.am array
 * 3. every packaged will be "unpacked" into other data structures, depending on what the package is bringing into the table:
 * a.data sec, MEKUDAD
 * b.instruction sec, MEKUDAD
 * c.labels array: every declaration(entry/extern)/definition(NEKUDOTIME) will be be here.
 * pay attention to the SGIRAT MAAGAL:
 * an entry .LENGTH
 * and
 * LENGTH: BLAH BLAH BLAH
 * will accommodate the same entry, like a SGIRAT MAAGAL
 * d. debtTable: anytime a command will use label as a parameter, this data structure will be come to life to document the debt, and later, pay it
 *
 * so we have data sturcte filled with info flew from the packages
 * 4. backend: it's time to show some results: .ent,.ext, .ob
 *
 * info about that "debt pay day" phase below:
 * run int the debt array -> for every debt entry: find the same label name the debt entry hold, in the label array -> in that same label entry, you will find the info relevent to pay it:
 * mainly defintion address relevant to that label name: when did the ":' stuff happened
 *
 * */
#include "middlend.h"
#include "middlendfuncs.c"

int dataCounter=0;
int instructionCounter=0;
int fatalError=FALSE;



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

int main(int argc, char **argv) {
    union packages* packages=NULL;
    int numOfPackages = 0;
    int iForPackages = 0;

    int *dataSection=NULL;

    int *instructionSection=NULL;

    labelInfo *labelsArray=NULL;
    int iForLabelsArray = 0;
    int numOfLabelsInLabelsArr = 0;
    int numOfEntry=0;
    int numOfExtern=0;

    debt* debtsTable=NULL;
    int numOfDebts=0;
    int iForDebtsTable=0;

    int iForCommandLine = 1;/*the zero saved for "assembler*/


    /*                          status holders              */
    int AMstatus;
    int packageStatus;
    /*------------------------------------------------------*/

    /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

    /*                         initializations                   */
    packages = (union packages*)malloc(sizeof(union packages));
    if (packages == NULL) {
        fprintf(stdout, "\nFail to create dynamic array that will hold organized info about .am lines\n");
        exit(1);
    }
    dataSection = (int *) malloc((sizeof(int)));
    if (dataSection == NULL) {
        fprintf(stdout, "\nFail to create dynamic array, for the data section\n");
        exit(1);
    }
    instructionSection = (int *) malloc((sizeof(int)));
    if (instructionSection == NULL) {
        fprintf(stdout, "\nFail to create dynamic array, for the instruction section\n");
        exit(1);
    }
    labelsArray = (labelInfo *) malloc(sizeof(labelInfo));
    if (labelsArray == NULL) {
        fprintf(stdout, "\nFail to create dynamic array, for the labels array\n");
        exit(1);
    }

    debtsTable = (debt *) malloc(sizeof(debt));
    if (debtsTable == NULL) {
        fprintf(stdout, "\nFail to create dynamic array for the debts\n");
        exit(1);
    }

    /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

    /*no file names given:*/
    if(argc==1) {
        fprintf(stdout,"\nPlease don't forget to add some file names for us the assemble\n");
        return FALSE;
    }

    /*--------------------------------------------------------------------------------------------*/
    /*for every .as file:*/
    while (iForCommandLine < argc){

        /*dont .as me:*/
        if(ASinTheName(argv[iForCommandLine])==TRUE){
            fprintf(stdout, "\nYou should've input %s without .as, don't expect any output\n", argv[iForCommandLine]);
            iForCommandLine++;
            continue;/*demanding stiff structure here*/
        }
        /*preprocessor time: creating .am file from a given .as file*/
        else{
            AMstatus = preprocessor(argv[iForCommandLine]);
        }
        /*the .am traslation process failed:*/
        if (AMstatus == FALSE) {/*if found there any fatal error*/
            fprintf(stdout, "\nFail to create a .am file for %s\n", argv[iForCommandLine]);
            /*To the next file name:*/
            free(packages);
            packages=NULL;
            free(dataSection);
            dataSection=NULL;
            free(instructionSection);
            instructionSection=NULL;
            free(labelsArray);
            labelsArray=NULL;
            free(debtsTable);
            debtsTable=NULL;
            iForCommandLine++;
            continue;
        }
            /*----------------------------------------------------------------------------------------------------------*/
            /*got the .am file, lets validate the input and package it:*/
        else {
            packageStatus = frontend(argv[iForCommandLine], &packages, &numOfPackages);
            /*packaging failed:*/
            if (packageStatus == FALSE) {
                fprintf(stdout, "\nFail to package the .am file of %s\n", argv[iForCommandLine]);
                /*To the next file name:*/
                free(packages);
                packages=NULL;
                free(dataSection);
                dataSection=NULL;
                free(instructionSection);
                instructionSection=NULL;
                free(labelsArray);
                labelsArray=NULL;
                free(debtsTable);
                debtsTable=NULL;
                iForCommandLine++;
                continue;
            }

        }
        /*got an array of chronology-ordered-packages representing the .am*/
        if (packageStatus==TRUE) {

            /*each package content will be delivered to some data structures */
            for (iForPackages = 0; iForPackages < numOfPackages; iForPackages++) {
                /*running on the array filled by the frontend:full of packages*/
                if (strncmp(packages[iForPackages].entryOrExternEntry.identifier, "ENTRY", 5) == 0) {
                    if(processingEntry(packages[iForPackages], &labelsArray, &numOfLabelsInLabelsArr, &numOfEntry)==FALSE){
                        fprintf(stdout,"\nFail to package %s\n",packages[iForPackages].entryOrExternEntry.labelName );
                        fatalError=TRUE;
                    }
                }
                else if (strncmp(packages[iForPackages].entryOrExternEntry.identifier, "EXTERN", 6) == 0) {
                    if(processingExtern(packages[iForPackages], &labelsArray, &numOfLabelsInLabelsArr, &numOfExtern)==FALSE){
                        fprintf(stdout,"\nFail to package %s\n",packages[iForPackages].entryOrExternEntry.labelName );
                        fatalError=TRUE;
                    }
                }
                else if (strncmp(packages[iForPackages].dataOrStringEntry.identifier, "STRING", 6) == 0) {
                    if(processingString( packages[iForPackages], &labelsArray, &numOfLabelsInLabelsArr, &dataSection,&dataCounter)==FALSE){
                        fprintf(stdout,"\nFail to package %s\n",packages[iForPackages].dataOrStringEntry.labelName );
                        fatalError=TRUE;
                    }
                }
                else if (strncmp(packages[iForPackages].dataOrStringEntry.identifier, "DATA", 4) == 0) {
                    if(processingData(packages[iForPackages], &labelsArray, &numOfLabelsInLabelsArr, &dataSection,&dataCounter)==FALSE){
                        fprintf(stdout,"\nFail to package %s\n",packages[iForPackages].dataOrStringEntry.labelName );
                        fatalError=TRUE;
                    }
                }
                else{
                    if(processingCommand(packages[iForPackages],&labelsArray,&numOfLabelsInLabelsArr,&instructionSection,&instructionCounter,&debtsTable,&numOfDebts)==FALSE){
                        fprintf(stdout,"\nFail to package %s\n",packages[iForPackages].commandEntry.labelName );
                        fatalError=TRUE;

                    }
                }
            }
            /*all the packages have been delivered to different arrays*/

            /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
            for(iForLabelsArray=0;iForLabelsArray<numOfLabelsInLabelsArr;iForLabelsArray++){
                if((labelsArray[iForLabelsArray].entryBit==TRUE) &&(labelsArray[iForLabelsArray].declarationBit==TRUE) ){
                    fprintf(stdout,"\n%s had entry-declared but never defined\n",labelsArray[iForLabelsArray].labelName);
                    /*fatal because if used as parameters, we dont have its definition */
                    fatalError=TRUE;
                }
            }
            /*debts pay day:*/
            for(iForDebtsTable=0;iForDebtsTable<numOfDebts;iForDebtsTable++) {

                for (iForLabelsArray = 0; iForLabelsArray < numOfLabelsInLabelsArr; iForLabelsArray++) {

                    if(strncmp(debtsTable[iForDebtsTable].labelName,labelsArray[iForLabelsArray].labelName, strlen(debtsTable[iForDebtsTable].labelName))==0) {

                        if (labelsArray[iForLabelsArray].entryBit==TRUE){
                            *(debtsTable[iForDebtsTable].debtLocation)=(labelsArray[iForLabelsArray].definitionAddress<<2) | 2;
                        }
                        else if (labelsArray[iForLabelsArray].externBit==TRUE){
                            *(debtsTable[iForDebtsTable].debtLocation)=1;
                        }
                        else{
                            *(debtsTable[iForDebtsTable].debtLocation)=labelsArray[iForLabelsArray].definitionAddress;
                        }

                        /*these will serve us when building .EXT, .ENT files:*/
                        debtsTable[iForDebtsTable].entryBit=labelsArray[iForLabelsArray].entryBit;
                        debtsTable[iForDebtsTable].externBit=labelsArray[iForLabelsArray].externBit;
                        break;
                    }

                }
            }


        }
        if(fatalError==FALSE) {

            backend(argv[iForCommandLine],dataSection,dataCounter,instructionSection,instructionCounter,labelsArray,numOfLabelsInLabelsArr,numOfEntry,numOfExtern,debtsTable,numOfDebts);
        }
        else {
            fprintf(stdout,"\nSince there has been some failure in the way, no output files will be created, for the chance of being invalid\n ");
        }
        free(packages);
        packages=NULL;
        free(dataSection);
        dataSection=NULL;
        free(instructionSection);
        instructionSection=NULL;
        free(labelsArray);
        labelsArray=NULL;
        free(debtsTable);
        debtsTable=NULL;
        iForCommandLine++;
    }
    return 0;
}
