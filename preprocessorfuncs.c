/*all the ammo needed for the preprocessor:
 * mainly dealing with validation of the mcro, endmcro input lines, or key words recognising a file name
 * */

/*validation a endmcro line*/
static int endmcroProcessing(char* lineFromAS ){
    int iForLine=0;
    int endmcroTime=TRUE;
    int cleanTime=FALSE;
    while (iForLine<MAXLINESIZE ){
        while(endmcroTime==TRUE){
            if((lineFromAS[iForLine]==' ') || (lineFromAS[iForLine]=='\t')){
                iForLine++;
            }
            else if (strncmp(&lineFromAS[iForLine], "endmcro",7 )==0){
                iForLine+=5;
                endmcroTime=FALSE;
                cleanTime=TRUE;
            }
            else{
                fprintf(stdout,"endmcro declaration not on time in %s\n", lineFromAS);

                return FALSE;

            }

        }
        while(cleanTime==TRUE&&iForLine<MAXLINESIZE){
            if(lineFromAS[iForLine]==' ' || lineFromAS[iForLine]=='\t'){
                iForLine++;
            }
            else{
                fprintf(stdout,"we notice a redundant char after endmcro, him and other after him, if exist, will be neglected\n");
                cleanTime=FALSE;
                return TRUE;
            }


        }




    }

    return TRUE;

}
/*validating a mcro line*/
static int macroNameProcessing(char* lineFromAS, char* macroNameBuffer ){
    /*3 phases expected on a right input: white chars-> "mcro" -> white chars -> name -> white chars */
    int iForLine=0;
    int iForMacroNameBuffer=0;
    int mcroTime=TRUE;
    int nameTime=FALSE;
    int cleanTime=FALSE;


    /*"mcro" phase*/
    while (mcroTime==TRUE){
        if((lineFromAS[iForLine]==' ') || (lineFromAS[iForLine]=='\t')){
            iForLine++;
        }
        else if (strncmp(&lineFromAS[iForLine], "mcro "/*mind the space*/,5 )==0){
            iForLine+=5;
            mcroTime=FALSE;
            nameTime=TRUE;
        }
        else{
            fprintf(stdout,"mcro declaration not on time in %s\n", lineFromAS);
            return FALSE;
        }

    }
    /*name phase*/
    while(nameTime==TRUE){
        if(lineFromAS[iForLine]==' ' || lineFromAS[iForLine]=='\t'){
            iForLine++;
        }
        else if (iForMacroNameBuffer==0 && isalpha(lineFromAS[iForLine])){
            macroNameBuffer[iForMacroNameBuffer]=lineFromAS[iForLine];
            iForMacroNameBuffer++;
            iForLine++;

        }
        else if (iForMacroNameBuffer>0 && (isalpha(lineFromAS[iForLine])|| isdigit(lineFromAS[iForLine]))) {
            macroNameBuffer[iForMacroNameBuffer] = lineFromAS[iForLine];
            iForMacroNameBuffer++;
            iForLine++;
        }
        else if ((iForMacroNameBuffer>0  && (lineFromAS[iForLine]==' ' || lineFromAS[iForLine]=='\t' || lineFromAS[iForLine]=='\0' || lineFromAS[iForLine]=='\n')) || (iForLine==MAXLINESIZE)){

            nameTime=FALSE;
            cleanTime=TRUE;
            iForLine++;
            macroNameBuffer[iForMacroNameBuffer]='\0';
            if (keyWords(macroNameBuffer)==TRUE){
                fprintf(stdout,"do not use this word a macro name %s\n", macroNameBuffer);
                return FALSE;
            }

        }
        else{
            fprintf(stdout,"invalid mcro name in %s\n", lineFromAS);
            return FALSE;
        }


    }
    while(cleanTime==TRUE&&iForLine<MAXLINESIZE){
        if(lineFromAS[iForLine]==' ' || lineFromAS[iForLine]=='\t'){
            iForLine++;
        }
        else{
            fprintf(stdout,"we notice a reduandant char after %s, him and other after him, if exist, will be neglected\n", macroNameBuffer);
            cleanTime=FALSE;

        }


    }
    return TRUE;


}
/*command names are great opportunity to recognize the lines' purpose*/
static int validCommandName(char* lineFromAS){
    int iForLine=0;
    while(iForLine<MAXLINESIZE){
        if (lineFromAS[iForLine]==' ' || lineFromAS[iForLine]=='\t'){
            iForLine++;
        }
        else{

            if(strncmp(&lineFromAS[iForLine],"mov",3)==0){
                return TRUE;
            }
            else if(strncmp(&lineFromAS[iForLine],"cmp",3)==0){
                return TRUE;
            }
            else if(strncmp(&lineFromAS[iForLine],"add",3)==0){
                return TRUE;
            }
            else if(strncmp(&lineFromAS[iForLine],"sub",3)==0){
                return TRUE;
            }
            else if(strncmp(&lineFromAS[iForLine],"not",3)==0){
                return TRUE;
            }else if(strncmp(&lineFromAS[iForLine],"clr",3)==0){
                return TRUE;
            }

            else if(strncmp(&lineFromAS[iForLine],"lea",3)==0){
                return TRUE;
            }else if(strncmp(&lineFromAS[iForLine],"inc",3)==0){
                return TRUE;
            }
            else if(strncmp(&lineFromAS[iForLine],"dec",3)==0){
                return TRUE;
            }
            else if(strncmp(&lineFromAS[iForLine],"jmp",3)==0){
                return TRUE;
            }
            else if(strncmp(&lineFromAS[iForLine],"bne",3)==0){
                return TRUE;
            }
            else if(strncmp(&lineFromAS[iForLine],"red",3)==0){
                return TRUE;
            }
            else if(strncmp(&lineFromAS[iForLine],"prn",3)==0){
                return TRUE;
            }
            else if(strncmp(&lineFromAS[iForLine],"jsr",3)==0){
                return TRUE;
            }
            else if(strncmp(&lineFromAS[iForLine],"stop",4)==0){
                return TRUE;
            }

            else{
                return FALSE;
            }
        }

    }
    return FALSE;
}


static int keyWords(char* macroNameBuffer){


    if(strncmp(macroNameBuffer,"mov",3)==0){
        return TRUE;
    }
    else if(strncmp(macroNameBuffer,"cmp",3)==0){
        return TRUE;
    }
    else if(strncmp(macroNameBuffer,"add",3)==0){
        return TRUE;
    }
    else if(strncmp(macroNameBuffer,"sub",3)==0){
        return TRUE;
    }
    else if(strncmp(macroNameBuffer,"not",3)==0){
        return TRUE;
    }else if(strncmp(macroNameBuffer,"clr",3)==0){
        return TRUE;
    }
    else if(strncmp(macroNameBuffer,"lea",3)==0){
        return TRUE;
    }else if(strncmp(macroNameBuffer,"inc",3)==0){
        return TRUE;
    }
    else if(strncmp(macroNameBuffer,"dec",3)==0){
        return TRUE;
    }
    else if(strncmp(macroNameBuffer,"jmp",3)==0){
        return TRUE;
    }
    else if(strncmp(macroNameBuffer,"bne",3)==0){
        return TRUE;
    }
    else if(strncmp(macroNameBuffer,"red",3)==0){
        return TRUE;
    }
    else if(strncmp(macroNameBuffer,"prn",3)==0){
        return TRUE;
    }
    else if(strncmp(macroNameBuffer,"jsr",3)==0){
        return TRUE;
    }
    else if(strncmp(macroNameBuffer,"stop",4)==0){
        return TRUE;
    }
    else if(strncmp(macroNameBuffer,"entry",5)==0){
        return TRUE;
    }
    else if(strncmp(macroNameBuffer,"extern",6)==0){
        return TRUE;
    }
    else if(strncmp(macroNameBuffer,"data",4)==0){
        return TRUE;
    }
    else if(strncmp(macroNameBuffer,"string",6)==0){
        return TRUE;
    }
    else{
        return FALSE;
    }
    return FALSE;
}


