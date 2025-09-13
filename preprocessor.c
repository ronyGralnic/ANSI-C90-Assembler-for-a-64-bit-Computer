/*here we will translate a .as file, to .am:
 * macro PRISSOT, no documentation, no empty lines
 * in a matter of fact in this phase, the user can cheat:
 * 1. entry X and then extern X
 * 2. .entry .entry
 * and more
 *
 * and we dont care, for now
 * the frontent job is to block it, if occured
 *
 * shoutout to the strstr function
 *
 * macro table used. and some files tricks: saving places, jumping.
 *
 * mainly you can think about it like this:
 * [   [mcro def..... endmcro]              [mcro def....endmcro]    ]
 * 2 phases, 2 worlds
 *
 * when we face a macro DEFINITION: the content gets ignored, the name and place saved
 * when we face its name, namely PRISSA-time : we are jumping into the releveant defintion
 *
 * pay attention to the SEDER the different strstrs funcs are used, namely the SEDER  of cases, implemented
 *
 * */

#include "preprocessor.h"
#include "preprocessorfuncs.c"

int preprocessor(char* fileName) {


    char* fileNameCopyAS=NULL;
    char* fileNameCopyAM=NULL;

    FILE *fdAS;
    FILE *fdAM;

    char* lineFromAS;

    macro* macros=NULL;
    macro* macrosTemp=NULL;
    int numOfMacros=0;

    int iForLine;/*will help us take a char form a line into a char form a struct*/
    int iForMacrosStructure=0;



    int inMacroBit=FALSE;/*turned off*/
    fpos_t currentLocation;

    char* macroNameBuffer;

    /*---------- end of intializations---*/


    /*---- files and memory allocations:------*/




    /*open .as:*/
    /*buffering the name:*/
    fileNameCopyAS=(char* )malloc(strlen(fileName)+strlen(AS)+1);
    if(fileNameCopyAS==NULL){
        fprintf(stdout,"\nInner problem: malloc failed for file name copy\n");
        return FALSE;
    }
    strcpy(fileNameCopyAS,fileName);
    strcat(fileNameCopyAS,AS);

    /*open:*/
    fdAS=fopen(fileNameCopyAS,"r");
    if (fdAS == NULL) {
        fprintf(stdout, "can't open .as file somewhat for %s, try again\n",fileName);
        return FALSE;
    }
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    /*create .am:*/
    /*buffering the name:*/
    fileNameCopyAM=(char* )malloc(strlen(fileName)+strlen(AM)+1);
    if(fileNameCopyAM==NULL){
        fprintf(stdout,"\nInner problem: malloc failed for file name copy\n");
        return FALSE;
    }
    strcpy(fileNameCopyAM,fileName);
    strcat(fileNameCopyAM,AM);
    /*create:*/
    fdAM=fopen(fileNameCopyAM,"w");
    if (fdAM == NULL) {
        fprintf(stdout, "can't open .as file somewhat for %s, try again\n",fileName);
        return FALSE;
    }
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    /*macro table:*/
    macros=(macro*)malloc(sizeof(macro));
    if(macros==NULL){
        fprintf(stdout,"\nmacro table intialization failure\n");
        return FALSE;
    }

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

    /*per .as line:*/
    while ( !(feof(fdAS))   ){
        iForLine=0;
        /*#######################################################################*/
        /*memory allocation for the .as line:*/
        lineFromAS=(char* )malloc(MAXLINESIZE);
        if(lineFromAS==NULL){
            fprintf(stdout,"\nInner problem: malloc failed for line buffer copy\n");
            return FALSE;
        }
        fgets(lineFromAS,MAXLINESIZE,fdAS);
        /*#######################################################################*/

        if (inMacroBit==FALSE) {
            /*1*/
            if (strstr(lineFromAS, ".entry "/*mind the space*/) || strstr(lineFromAS, ".extern "/*mind the space*/) ||
                strstr(lineFromAS, ": ")) {
                if (strstr(lineFromAS, ".entry "/*mind the space*/)&&strstr(lineFromAS, ": ")){

                    if(  (   (strstr(lineFromAS, ".entry "))     -       (strstr(lineFromAS, ": "))      )   >0){
                        fprintf(stdout,"\nthe label before .entry is neglected\n");
                        fputs(strstr(lineFromAS, ".entry "), fdAM);
                        /*done with line*/
                        free(lineFromAS);
                        lineFromAS = NULL;

                    }/*BTW if ":" comes AFTER .entry, it NOT our job now*/

                }
                else  if (strstr(lineFromAS, ".extern "/*mind the space*/)&&strstr(lineFromAS, ": ")){

                    if((strstr(lineFromAS, ".extern ")-(strstr(lineFromAS, ": ")))>0){
                        fprintf(stdout,"\nthe label before .entry is neglected\n");
                        fputs(strstr(lineFromAS, ".extern "), fdAM);
                        /*done with line*/
                        free(lineFromAS);
                        lineFromAS = NULL;

                    }/*BTW if ":" comes AFTER .extern, it NOT our job now*/

                }
                else{
                    fputs(lineFromAS, fdAM);
                    /*done with line*/
                    free(lineFromAS);
                    lineFromAS = NULL;
                }

            }
            /*2*/
            else if (strstr(lineFromAS, "mcro "/*mind the space*/)) {
                macroNameBuffer=(char* )malloc(MAXLINESIZE);
                if(macroNameBuffer==NULL){
                    fprintf(stdout,"\nInner problem: malloc failed for line buffer copy\n");
                    return FALSE;
                }
                if(macroNameProcessing(lineFromAS,macroNameBuffer)==TRUE){
                    macrosTemp=(macro*)realloc(macros,(numOfMacros+1)* sizeof(macro));
                    if(macrosTemp==NULL){
                        fprintf(stdout,"\nInner problem: malloc failed for line buffer copy\n");
                        return FALSE;
                    }
                    /*assignments:*/

                    strncpy(macrosTemp[numOfMacros].name,macroNameBuffer, strlen(macroNameBuffer)+1);
                    fgetpos(fdAS,&macrosTemp[numOfMacros].location);
                    macros=macrosTemp;
                    macrosTemp=NULL;

                    inMacroBit=TRUE;
                    numOfMacros++;

                }
                else{
                    fprintf(stdout,"\ninvalid input in %s\n",lineFromAS);
                    free(lineFromAS);
                    lineFromAS=NULL;
                    return FALSE;
                }

                free(macroNameBuffer);
                macroNameBuffer=NULL;
                free(lineFromAS);
                lineFromAS=NULL;
            }
            else if (strcmp(&lineFromAS[iForLine], ";")==0){
                free(lineFromAS);
                lineFromAS=NULL;
            }

            /*3*/
            else if (validCommandName(lineFromAS)==TRUE) {
                fputs(lineFromAS,fdAM);
            }



            /*5*/
            else{/*potentially PORSIM mcro*/
                while (iForLine<MAXLINESIZE){
                    if (lineFromAS[iForLine]=='\n'){/*empty line*/
                        free(lineFromAS);
                        lineFromAS=NULL;
                    }

                    /*those white chars: */
                    if((lineFromAS[iForLine]==' ') || (lineFromAS[iForLine]=='\t')){
                        iForLine++;

                    }
                    /*the line is macro calling?*/
                    else{
                        for(iForMacrosStructure=0;iForMacrosStructure<numOfMacros;iForMacrosStructure++){

                            if (strncmp(&lineFromAS[iForLine],macros[iForMacrosStructure].name,strlen(macros[iForMacrosStructure].name))==0){

                                fgetpos(fdAS,&currentLocation);
                                fsetpos(fdAS,&macros[iForMacrosStructure].location);

                                while(  !(strstr(lineFromAS,"endmcro")) ) {
                                    if( !(strstr(lineFromAS,macros[iForMacrosStructure].name)|| strstr(lineFromAS,"mcro"))){
                                        fputs(lineFromAS,fdAM);
                                    }
                                    /*equivalent to "i++":*/
                                    free(lineFromAS);
                                    lineFromAS = NULL;
                                    lineFromAS=(char* )malloc(MAXLINESIZE);
                                    if(lineFromAS==NULL){
                                        fprintf(stdout,"\nInner problem: malloc failed for line buffer copy\n");
                                        return FALSE;
                                    }
                                    fgets(lineFromAS,MAXLINESIZE,fdAS);
                                    /*end of "i++"*/

                                }
                                /*endmcro on the buffer:*/
                                fsetpos(fdAS,&currentLocation);
                                free(lineFromAS);
                                lineFromAS = NULL;
                                 iForLine++;
                                break;

                                }

                        }
                        if(iForMacrosStructure==numOfMacros){
                            fprintf(stdout,"\n%s is junk\n",lineFromAS);
                            return FALSE;
                        }
                    }/*END of the line is macro calling?*/
                    break;
                }/*end of running on the line*/

            }/*end of potentially PRISSA*/





        }
        else if (inMacroBit==TRUE){
            if (strstr(lineFromAS, "endmcro")) {
                /*structure: white chars-> "endmcro" -> white chars*/
                if (endmcroProcessing(lineFromAS)==TRUE){
                    inMacroBit=FALSE;
                }
                else{
                    fprintf(stdout,"\ninvalid endmcro line for: %s\n",lineFromAS);
                    return FALSE;
                }

            }
            free(lineFromAS);
            lineFromAS=NULL;

        }


    }
    free(macros);
    free(fileNameCopyAS);
    free(fileNameCopyAM);

    fclose(fdAM);
    fclose(fdAS);
    return TRUE;
}

