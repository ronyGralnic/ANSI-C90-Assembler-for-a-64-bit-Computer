/*packaging the .am lines:
 * entry/extern, command, data/string: each get a builtd struct repressenting valueable info he can bring to the table
 * moreover: valdiataion checks are spotlighted! valdi names, the tight amount of parameters, that closing ' " ' on .string
 * all of the packages will be chronically organzied in a union array*/
#include "frontend.h"
#include "frontendfuncs.c"
int frontend(char* fileName,union packages** packages,int *numOfPackages) {

    FILE *fdAM;
    char *fileNameCopyAM = NULL;

    char* lineFromAM=NULL;

    char* labelBuffer=NULL;

    char* parametersBuffer=NULL;
    int numOfParameters=0;

    char* sourceBuffer=NULL;
    char* destinationBuffer=NULL;
    char* commandBuffer=NULL;




    int identifier;

    union packages *packagesTemp = NULL;









    /*open the .am:*/
    fileNameCopyAM = (char *) malloc(strlen(fileName) + strlen(AM) + 1);
    if (fileNameCopyAM == NULL) {
        fprintf(stdout, "\nInner problem: malloc failed for file name copy\n");
        return FALSE;
    }
    strcpy(fileNameCopyAM, fileName);
    strcat(fileNameCopyAM, AM);
    fdAM = fopen(fileNameCopyAM, "r");
    if (fdAM == NULL) {
        fprintf(stdout, "can't open .as file somewhat for %s, try again\n", fileName);
        return FALSE;
    }
    /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/





    /*packaging .am file:*/
    while (!feof(fdAM)) {
        /*memory allocation for the line buffer:*/
        lineFromAM=(char*) malloc(MAXLINESIZE* sizeof(char));
        if(lineFromAM==NULL){
            fprintf(stdout,"\nInner problem: malloc failure for lineFromAm buffer\n");
            return FALSE;
        }
        /*working per line:*/
        fgets(lineFromAM, MAXLINESIZE, fdAM);

        /*potentially entry/extern declaration:*/
        if (strstr(lineFromAM, ".entry") || (strstr(lineFromAM, ".extern"))) {

            if (strstr(lineFromAM, ".entry")) {
                identifier = ENTRY;
            } else {
                identifier = EXTERN;
            }
            /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
            /*memory allocation for buffering the label:*/
            labelBuffer=(char*) malloc(MAXLABELNAMELENGTH* sizeof(char));
            if(labelBuffer==NULL){
                fprintf(stdout,"\nInner problem: malloc failure for labelBuffer buffer\n");
                return FALSE;
            }
            /*validation:*/
            if (processingEntryOrExternLine(lineFromAM, labelBuffer) == TRUE) {

                /*lets package:*/

                /*new package:*/
                packagesTemp = (union packages *) realloc(*packages, ((*numOfPackages) + 1) * sizeof(union packages));
                if (packagesTemp == NULL) {
                    fprintf(stdout, "\nFailed to create a new place fot the package for %s\n", lineFromAM);
                    return FALSE;/*crucial to the future*/
                }

                /*identifier:*/
                if (identifier == ENTRY) {
                    strncpy(packagesTemp[(*numOfPackages)].entryOrExternEntry.identifier, "ENTRY\0", 6);

                } else if (identifier == EXTERN) {
                    strncpy(packagesTemp[(*numOfPackages)].entryOrExternEntry.identifier, "EXTERN\0", 7);
                }
                /*label stuff:*/
                strncpy(packagesTemp[(*numOfPackages)].entryOrExternEntry.labelName, labelBuffer, strlen(labelBuffer)+1);

                /*meta-data updates, for the future*/
                (*numOfPackages)++;
                /*thanks for your service:*/
                *packages = packagesTemp;
                packagesTemp = NULL;
                free(labelBuffer);
                free(lineFromAM);
                labelBuffer=NULL;
                lineFromAM=NULL;


            }/*-----------------------------------------------------------------------------------------*/
            /*entry/extern validation failed:*/
            else {
                fprintf(stdout, "\nInvalid entry-structure for %s\n", lineFromAM);
                return FALSE;
            }
        }
            /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

        /*potentially data/string definition:*/
        else if (strstr(lineFromAM, ".string") || (strstr(lineFromAM, ".data"))) {

            if (strstr(lineFromAM, ".string")) {
                identifier = STRING;
            } else {
                identifier = DATA;
            }
            /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
            /*memory allocation for the buffers: label AND parameters: "saergdff" or 5,6,7 */
            labelBuffer=(char*) malloc(MAXLABELNAMELENGTH* sizeof(char));
            if(labelBuffer==NULL){
                fprintf(stdout,"\nInner problem: malloc failure for labelBuffer buffer\n");
                return FALSE;
            }
            parametersBuffer=(char*) malloc(MAXLINESIZE* sizeof(char));
            if(parametersBuffer==NULL){
                fprintf(stdout,"\nInner problem: malloc failure for parametersBuffer buffer\n");
                return FALSE;
            }
            /*validation check:*/
            if (processingDataOrStringLine(lineFromAM, labelBuffer,parametersBuffer, &numOfParameters) == TRUE) {
                /*lets package:*/
                /*new package:*/
                packagesTemp = (union packages *) realloc(*packages, ((*numOfPackages) + 1) * sizeof(union packages));
                if (packagesTemp == NULL) {
                    fprintf(stdout, "\nFailed to create a new place fot the package for %s\n", lineFromAM);
                    return FALSE;/*crucial to the future*/
                }

                /*identifier:*/
                if (identifier == STRING) {
                    strncpy(packagesTemp[(*numOfPackages)].dataOrStringEntry.identifier, "STRING\0", 7);

                } else if (identifier == DATA) {
                    strncpy(packagesTemp[(*numOfPackages)].dataOrStringEntry.identifier, "DATA\0", 5);
                }

                /*label stuff:*/
                strncpy(packagesTemp[(*numOfPackages)].dataOrStringEntry.labelName, labelBuffer, strlen(labelBuffer)+1);

                /*parameters stuff:*/
                strncpy(packagesTemp[(*numOfPackages)].dataOrStringEntry.parameters, parametersBuffer,
                        strlen(parametersBuffer)+1);
                packagesTemp[(*numOfPackages)].dataOrStringEntry.numOfParameters = numOfParameters;

                /*meta-data updates, for the future*/
                (*numOfPackages)++;

                *packages = packagesTemp;
                packagesTemp = NULL;
                free(labelBuffer);
                free(lineFromAM);
                free(parametersBuffer);
                labelBuffer= NULL;
                lineFromAM= NULL;
                parametersBuffer= NULL;

            }
            /*validation failure:*/
            else {
                fprintf(stdout, "\nInvalid data/string-structure for %s\n", lineFromAM);
                return FALSE;
            }


        }
        /*potentially command, with label or not:*/
        else{
            if(strstr(lineFromAM, ":")){
                identifier=COMMANDANDLABEL;
            }
            else{
                identifier=COMMANDWITHOUTLABEL;
            }
            /*memory allocation for buffers: label, command(valid), source and dest(register/num/label): */
            labelBuffer=(char*) malloc(MAXLABELNAMELENGTH* sizeof(char));
            if(labelBuffer==NULL){
                fprintf(stdout,"\nInner problem: malloc failure for labelBuffer buffer\n");
                return FALSE;
            }
            commandBuffer=(char*) malloc(5* sizeof(char));
            if(commandBuffer==NULL){
                fprintf(stdout,"\nInner problem: malloc failure for commandBuffer buffer\n");
                return FALSE;
            }
            sourceBuffer=(char*) malloc(MAXLABELNAMELENGTH* sizeof(char));
            if(sourceBuffer==NULL){
                fprintf(stdout,"\nInner problem: malloc failure for sourceBuffer buffer\n");
                return FALSE;
            }
            destinationBuffer=(char*) malloc(MAXLABELNAMELENGTH* sizeof(char));
            if(destinationBuffer==NULL){
                fprintf(stdout,"\nInner problem: malloc failure for destinationBuffer buffer\n");
                return FALSE;
            }
            /*-------------------------------------------------------------------------------------------------------*/
            /*validation check:*/
            if(processingCommand(lineFromAM,labelBuffer,commandBuffer,sourceBuffer,destinationBuffer)==TRUE){
                /*lets package:*/
                /*new one:*/
                packagesTemp = (union packages *) realloc(*packages, ((*numOfPackages) + 1) * sizeof(union packages));
                if (packagesTemp == NULL) {
                    fprintf(stdout, "\nFailed to create a new place fot the package for %s\n", lineFromAM);
                    return FALSE;/*crucial to the future*/
                }

                /*identifier*/
                strncpy(packagesTemp[*numOfPackages].commandEntry.identifier, "COMMAND\0", 8);


                /*command name*/
                strncpy(packagesTemp[*numOfPackages].commandEntry.commandName, commandBuffer, strlen(commandBuffer)+1);

                if(identifier==COMMANDANDLABEL){
                    packagesTemp[*numOfPackages].commandEntry.labelBit = TRUE;
                    strncpy(packagesTemp[*numOfPackages].commandEntry.labelName, labelBuffer, strlen(labelBuffer)+1);

                }
                else{
                    packagesTemp[*numOfPackages].commandEntry.labelBit =FALSE;
                }


                /*sourcing:*/
                if(commandsWithSource(packagesTemp[*numOfPackages].commandEntry.commandName)==TRUE) {
                    if (sourceBuffer[0] == '@') {
                        strncpy(packagesTemp[*numOfPackages].commandEntry.sourceIdentifier, "REGISTER", 8);
                        packagesTemp[*numOfPackages].commandEntry.numOfParameters++;
                        packagesTemp[*numOfPackages].commandEntry.sourceBit = TRUE;
                        strncpy(packagesTemp[*numOfPackages].commandEntry.sourceContent, sourceBuffer,
                                strlen(sourceBuffer) + 1);


                    } else if (isdigit(sourceBuffer[0]) || sourceBuffer[0] == '-') {
                        strncpy(packagesTemp[*numOfPackages].commandEntry.sourceIdentifier, "NUMBER", 6);
                        packagesTemp[*numOfPackages].commandEntry.numOfParameters++;
                        packagesTemp[*numOfPackages].commandEntry.sourceBit = TRUE;
                        strncpy(packagesTemp[*numOfPackages].commandEntry.sourceContent, sourceBuffer,
                                strlen(sourceBuffer) + 1);

                    } else {
                        strncpy(packagesTemp[*numOfPackages].commandEntry.sourceIdentifier, "LABEL", 5);
                        packagesTemp[*numOfPackages].commandEntry.numOfParameters++;
                        packagesTemp[*numOfPackages].commandEntry.sourceBit = TRUE;
                        strncpy(packagesTemp[*numOfPackages].commandEntry.sourceContent, sourceBuffer,
                                strlen(sourceBuffer) + 1);


                    }
                }
                else{
                    packagesTemp[*numOfPackages].commandEntry.sourceBit = FALSE;
                }
                /*destinationing:*/
                if(commandsWithDestination(packagesTemp[*numOfPackages].commandEntry.commandName)==TRUE) {
                    if (destinationBuffer[0] == '@') {
                        strncpy(packagesTemp[*numOfPackages].commandEntry.destinationIdentifier, "REGISTER", 8);
                        packagesTemp[*numOfPackages].commandEntry.numOfParameters++;
                        packagesTemp[*numOfPackages].commandEntry.destinationBit = TRUE;
                        strncpy(packagesTemp[*numOfPackages].commandEntry.destinationContent, destinationBuffer,
                                strlen(destinationBuffer) + 1);


                    } else if (isdigit(destinationBuffer[0]) || destinationBuffer[0] == '-') {
                        strncpy(packagesTemp[*numOfPackages].commandEntry.destinationIdentifier, "NUMBER", 6);
                        packagesTemp[*numOfPackages].commandEntry.numOfParameters++;
                        packagesTemp[*numOfPackages].commandEntry.destinationBit = TRUE;
                        strncpy(packagesTemp[*numOfPackages].commandEntry.destinationContent, destinationBuffer,
                                strlen(destinationBuffer) + 1);

                    } else  {
                        strncpy(packagesTemp[*numOfPackages].commandEntry.destinationIdentifier, "LABEL", 5);
                        packagesTemp[*numOfPackages].commandEntry.numOfParameters++;
                        packagesTemp[*numOfPackages].commandEntry.destinationBit = TRUE;
                        strncpy(packagesTemp[*numOfPackages].commandEntry.destinationContent, destinationBuffer,
                                strlen(destinationBuffer) + 1);


                    }
                }
                else{
                    packagesTemp[*numOfPackages].commandEntry.destinationBit =FALSE;
                }
                /*meta-data updates, for the future*/
                (*numOfPackages)++;
                *packages = packagesTemp;
                packagesTemp = NULL;
                free(lineFromAM);
                free(labelBuffer);
                free(commandBuffer);
                free(sourceBuffer);
                free(destinationBuffer);
                lineFromAM= NULL;
                labelBuffer= NULL;
                commandBuffer= NULL;
                sourceBuffer= NULL;
                destinationBuffer= NULL;


            }
            /*validation failed*/
            else{
                fprintf(stdout, "\nFail packaging a command for %s\n", lineFromAM);
                return FALSE;
            }

        }


    }
    free(fileNameCopyAM);
    fclose(fdAM);
    return TRUE;
}
