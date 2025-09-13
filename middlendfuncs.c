/*Here you will find all the ammos needed for LENATZEAH AL HATIZMOERT:
 * mainly delver any packaged info to a specific data structure*/

/*just for readability, nothing special:
 * A helping hand to the preprocessor*/
static int ASinTheName(char* argv){
    if (strstr(argv, ".as") != NULL) {
        return TRUE;
    }
    else{
        return FALSE;
    }

}
/*-------------------------------------------------------------------*/


/*an .entry SOMETHING is on the line, packaged!!!
 * a uniqueness check OR a defintion check, then, if needed: a new entry phase in the label data structure:*/
static int processingEntry(union packages package,labelInfo** labelsArray,int* numOfLabelsInLabelsArr, int* numOfEntry){

    int iForLabelArray;
    labelInfo* labelsArrayTemp=NULL;



    /*do we already deal with this label before? :*/
    for (iForLabelArray = 0; iForLabelArray < (*numOfLabelsInLabelsArr ); iForLabelArray++) {


        if (strncmp((*labelsArray)[iForLabelArray].labelName, package.entryOrExternEntry.labelName, strlen(package.entryOrExternEntry.labelName))==0) {
            /*we've found a label with the same name: can be only its definition*/

            /* 2 entry, same label name:*/
            if ((*labelsArray)[iForLabelArray].entryBit == TRUE) {
                fprintf(stdout, "\nWe've already declared %s as .entry, neglecting\n",package.entryOrExternEntry.labelName);
                return TRUE;
            }

                /*1 entry, 1 extern, same label name*/
            else if((*labelsArray)[iForLabelArray].externBit==TRUE){
                fprintf(stdout,"\nWe've already declared %s as extern, couldn't declare it now as entry\n",package.entryOrExternEntry.labelName);
                return FALSE;
            }
                /*so no extern, no entry, but same name. We are his entry. that was a definition*/
            else{/* SOGRIM MAAGAL so declaration bit is staying turned off*/
                (*labelsArray)[iForLabelArray].entryBit=TRUE;
                
                return TRUE;
            }
        }
    }

    /*no defintion using its name found, so it's a new one:*/

    labelsArrayTemp = (labelInfo *) realloc(*labelsArray,sizeof(labelInfo) * (*numOfLabelsInLabelsArr + 1));
    if (labelsArrayTemp == NULL) {
        fprintf(stdout, "\nAllocation failure for new entry on labels array, for %s\n",package.entryOrExternEntry.labelName);
        return FALSE;
    }
    /*Assignment:*/

    /*label name:*/
    strncpy(labelsArrayTemp[*numOfLabelsInLabelsArr].labelName,package.entryOrExternEntry.labelName, strlen(package.entryOrExternEntry.labelName));

    /*".entry LABEL-NAME" case so:*/
    labelsArrayTemp[*numOfLabelsInLabelsArr].declarationBit=TRUE;
    labelsArrayTemp[*numOfLabelsInLabelsArr].entryBit=TRUE;
    labelsArrayTemp[*numOfLabelsInLabelsArr].externBit=FALSE;
    labelsArrayTemp[*numOfLabelsInLabelsArr].definitionAddress=0;
    
    
    *labelsArray = labelsArrayTemp;
    labelsArrayTemp = NULL;

    (*numOfLabelsInLabelsArr)++;
    (*numOfEntry)++;
    return TRUE;
}


/*-------------------------------------------------------------------------------------------------------------------------------------------*/

/*an .extern SOMETHING is on the line, packaged
 * a uniqueness check OR a defintion check, then, if needed: a new entry phase in the label data structure:*/
static int processingExtern(union packages package,labelInfo** labelsArray,int* numOfLabelsInLabelsArr, int* numOfExtern){

    int iForLabelArray=0;
    labelInfo* labelsArrayTemp=NULL;



    /*does our label name was already used before?:*/
    for (iForLabelArray = 0; iForLabelArray < (*numOfLabelsInLabelsArr ); iForLabelArray++) {



        if (strncmp((*labelsArray)[iForLabelArray].labelName, package.entryOrExternEntry.labelName,strlen(package.entryOrExternEntry.labelName))==0) {
            /*we've found a label with the same name: can be only its definition*/


            /*2 extern, same name*/
            if ((*labelsArray)[iForLabelArray].externBit == TRUE) {
                fprintf(stdout, "\nWe've already declared %s as .extern, neglecting\n",package.entryOrExternEntry.labelName);
                return TRUE;
            }

                /*1 entry, 1 extern, same name*/
            else if((*labelsArray)[iForLabelArray].entryBit==TRUE){
                fprintf(stdout,"\nWe've already declared %s as entry, couldn't declare it now as entry\n",package.entryOrExternEntry.labelName);
                return FALSE;
            }
            else{/*the only chance left is, we've found a TA declared because a definition has occurred before. disaster, we are extern declaration now!*/
                fprintf(stdout,"\nSo we've got a definition with the same label name, and now we declare extern? BIG NO NO\n");
                return FALSE;

            }
        }
    }


    /*All these iterations and can't find anything, so it's a new one:*/
    labelsArrayTemp = (labelInfo *) realloc(*labelsArray,
                                            sizeof(labelInfo) * (*numOfLabelsInLabelsArr + 1));
    if (labelsArrayTemp == NULL) {
        fprintf(stdout, "\nAllocation failure for new entry on labels array, for %s\n",package.entryOrExternEntry.labelName);
        return FALSE;
    }
    /*Assignment:*/

    /*label name:*/
    strncpy(labelsArrayTemp[*numOfLabelsInLabelsArr].labelName,package.entryOrExternEntry.labelName, strlen(package.entryOrExternEntry.labelName));

    /*".extern LABEL-NAME" case so:*/
    labelsArrayTemp[*numOfLabelsInLabelsArr].declarationBit=TRUE;
    labelsArrayTemp[*numOfLabelsInLabelsArr].entryBit=FALSE;
    labelsArrayTemp[*numOfLabelsInLabelsArr].externBit=TRUE;
    labelsArrayTemp[*numOfLabelsInLabelsArr].definitionAddress=0;
    *labelsArray = labelsArrayTemp;
    labelsArrayTemp = NULL;


    (*numOfLabelsInLabelsArr)++;
    (*numOfExtern)++;
    return TRUE;
}


/*-------------------------------------------------------------------------------------------------------------------------------------------*/


/*string: labelname->.string->"srdferq".
 * we are working on DEFINITION!!!!!
 * uniquenes check AND new entry is here:*/
static int processingString(union packages package,labelInfo** labelsArray,int* numOfLabelsInLabelsArr, int** dataSection, int* numOfDataSection){

    int newEntryBit=TRUE; /*The value needs to survive the first phase*/
    int iForLabelArray=0;
    int iForChars=0;
    labelInfo* labelsArrayTemp=NULL;
    int* dataSectionTemp=NULL;
    /*-----------------------------------------------------------------*/



    /*unique check*/
    for (iForLabelArray = 0; iForLabelArray < (*numOfLabelsInLabelsArr ); iForLabelArray++) {

        if(strncmp((*labelsArray)[iForLabelArray].labelName,package.dataOrStringEntry.labelName, strlen(package.dataOrStringEntry.labelName))==0){
            /*same name:*/

            /*we are holding a definition, and found an extern declaration with the same name:*/
            if ((*labelsArray)[iForLabelArray].externBit==TRUE){
                fprintf(stdout,"\nNo way an extern declaration gets a local definition, as like %s\n",package.dataOrStringEntry.labelName);
                return FALSE;
            }
            /*2 definition, same name:*/
            if((*labelsArray)[iForLabelArray].definitionAddress != 0){
                fprintf(stdout,"\nAnother definition have been made with %s already, can't use it again\n",package.dataOrStringEntry.labelName);
                return FALSE;
            }
            /*if we've met an .entry declaration before, with the same name, we are SOGRIM MAAGAL*/
            if(labelsArray[iForLabelArray]->declarationBit==TRUE){
                labelsArray[iForLabelArray]->declarationBit=FALSE;
                labelsArray[iForLabelArray]->isDataOrString=TRUE;
                newEntryBit=FALSE;
                break;/*NOT DONE! still need to put it into the data section*/
            }

        }
    }
    /*new entry, if needed, decided above:*/
    if (newEntryBit==TRUE) {


        labelsArrayTemp = (labelInfo *) realloc(*labelsArray,sizeof(labelInfo) * (*numOfLabelsInLabelsArr + 1));
        if (labelsArrayTemp == NULL) {
            fprintf(stdout, "\nAllocation failure for new entry on labels array, for %s\n", package.dataOrStringEntry.labelName);
            return FALSE;
        }
        /*Assignments:*/

        /*label stuff:*/
        strncpy(labelsArrayTemp[*numOfLabelsInLabelsArr].labelName, package.dataOrStringEntry.labelName,strlen(package.dataOrStringEntry.labelName));


        labelsArrayTemp[*numOfLabelsInLabelsArr].externBit = FALSE;
        labelsArrayTemp[*numOfLabelsInLabelsArr].entryBit = FALSE;
        labelsArrayTemp[*numOfLabelsInLabelsArr].declarationBit = FALSE;

        labelsArrayTemp[*numOfLabelsInLabelsArr].definitionAddress = (*numOfDataSection);
	
	labelsArrayTemp[*numOfLabelsInLabelsArr].isDataOrString = TRUE;
	
        *labelsArray = labelsArrayTemp;
        labelsArrayTemp = NULL;


        /*ATTENTION: *numOfDataSection YEKUDAM  in the next phase */
        (*numOfLabelsInLabelsArr)++;

    }
    /*REMINDRER : LABEL: .string "abcdef\0"
     * into the data section: all those "abcdef\0" chars in between, are sending into the data structure
     * each char get a TA:*/

    for(iForChars=0;iForChars<=package.dataOrStringEntry.numOfParameters;iForChars++){
        dataSectionTemp=(int*) realloc(*dataSection,((*numOfDataSection)+1)*sizeof (int));
        if (dataSectionTemp == NULL) {
            fprintf(stdout, "\nFail to create another entry on data section\n");
            return FALSE;
        }
        if(iForChars==package.dataOrStringEntry.numOfParameters) {
            dataSectionTemp[*numOfDataSection] = '\0';
        }
        else {
            dataSectionTemp[*numOfDataSection] = (int) package.dataOrStringEntry.parameters[iForChars];
        }
        *dataSection=dataSectionTemp;
        dataSectionTemp=NULL;

        (*numOfDataSection)++;

    }

    return TRUE;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------*/
/*data: labelname->.data->numbers, fluidi.
 * THIS IS A DEFINITION!!!!!
 * uniqueness check, new entry AND pushing into the data sec is here:*/
static int processingData(union packages package,labelInfo** labelsArray,int* numOfLabelsInLabelsArr,int** dataSection, int* numOfDataSection) {

    int newEntryBit = TRUE;/*The value needs to survive the first phase*/

    int iForLabelArray = 0;
    int iForChars = 0;

    char* numberBuffer=NULL;
    int iForNumberBuffer=0;

    labelInfo *labelsArrayTemp = NULL;

    int *dataSectionTemp = NULL;
    /*-----------------------------------------------------------*/


    for (iForLabelArray = 0; iForLabelArray < (*numOfLabelsInLabelsArr); iForLabelArray++) {


        if (strncmp((*labelsArray)[iForLabelArray].labelName, package.dataOrStringEntry.labelName, strlen(package.dataOrStringEntry.labelName)) == 0) {
            /*found TA with the same name:*/

            /*we are defintion, and have found an extern TA with the same name: */
            if ((*labelsArray)[iForLabelArray].externBit == TRUE) {
                fprintf(stdout, "\nNo way an extern declaration gets a local definition, as like %s\n", package.dataOrStringEntry.labelName);
                return FALSE;
            }
            /*2 defintions, same name:*/
            if ((*labelsArray)[iForLabelArray].definitionAddress != 0) {
                fprintf(stdout, "\nAnother definition have been made with %s already, can't use it again\n", package.dataOrStringEntry.labelName);
                return FALSE;
            }

            /*found its declaration, SOGRIM MAAGAlL:*/
            if ((*labelsArray)[iForLabelArray].declarationBit == TRUE) {
                (*labelsArray)[iForLabelArray].declarationBit = FALSE;
                
                (*labelsArray)[iForLabelArray].isDataOrString=TRUE;
                newEntryBit = FALSE;
                break;
            }

        }
    }
    /*new entry:*/

    if (newEntryBit==TRUE) {
        labelsArrayTemp = (labelInfo *) realloc(*labelsArray, sizeof(labelInfo) * (*numOfLabelsInLabelsArr + 1));
        if (labelsArrayTemp == NULL) {
            fprintf(stdout, "\nAllocation failure for new entry on labels array, for %s\n", package.dataOrStringEntry.labelName);
            return FALSE;
        }
        /*Assignments:*/

        /*label stuff:*/
        strncpy(labelsArrayTemp[*numOfLabelsInLabelsArr].labelName, package.dataOrStringEntry.labelName,strlen(package.dataOrStringEntry.labelName));


        labelsArrayTemp[*numOfLabelsInLabelsArr].externBit = FALSE;
        labelsArrayTemp[*numOfLabelsInLabelsArr].entryBit = FALSE;
        labelsArrayTemp[*numOfLabelsInLabelsArr].declarationBit = FALSE;

        labelsArrayTemp[*numOfLabelsInLabelsArr].definitionAddress = (*numOfDataSection);
        
        labelsArrayTemp[*numOfLabelsInLabelsArr].isDataOrString = TRUE;

        *labelsArray = labelsArrayTemp;
        labelsArrayTemp = NULL;


        (*numOfLabelsInLabelsArr)++;

    }
    /*into the data section:*/
    for(iForChars=0;iForChars<MAXLINESIZE&&package.dataOrStringEntry.numOfParameters!=0;iForChars++,package.dataOrStringEntry.numOfParameters--){



        dataSectionTemp=(int*) realloc(*dataSection,((*numOfDataSection)+1)*sizeof (int));
        if (dataSectionTemp == NULL) {
            fprintf(stdout, "\nFail to create another entry on data section\n");
            return FALSE;
        }
        numberBuffer=(char*)malloc(MAXLINESIZE*sizeof (char));
        if(numberBuffer==NULL){
            fprintf(stdout,"\ninner problem: fail to allocate a memory for the number buffer, in processing data for %s\n",package.dataOrStringEntry.labelName);
        }
        iForNumberBuffer=0;



        while(package.dataOrStringEntry.parameters[iForChars] !=','&&package.dataOrStringEntry.parameters[iForChars] !='\n'&&package.dataOrStringEntry.parameters[iForChars] !=' '&&package.dataOrStringEntry.parameters[iForChars] != '\0'){
            numberBuffer[iForNumberBuffer]=package.dataOrStringEntry.parameters[iForChars];
            iForNumberBuffer++;
            iForChars++;
        }
        numberBuffer[iForNumberBuffer]='\0';
        dataSectionTemp[*numOfDataSection]=atoi(numberBuffer);
        *dataSection=dataSectionTemp;
        dataSectionTemp=NULL;

        (*numOfDataSection)++;

    }
    return TRUE;

}


/*-------------------------------------------------------------------------------------------------------------------------------------------*/

/*structure: LABEL(maybe): command sourceParam(maybe) , destParam(maybe)
 * uniqueness check, new entry AND pushing into the instruction sec is here:
 * BTW, pay attention to the debt filling up(if label is "paramereterized"*/
static int processingCommand(union packages package,labelInfo** labelsArray,int* numOfLabelsInLabelsArr,int** instructionSection,int* numOfInstructionSection , debt** debtsTable, int* numOfDebts){

    int iForLabelArray = 0;
    int newEntryBit=TRUE;
    labelInfo* labelsArrayTemp=NULL;

    int recordBuffer=0;
    int theWordToSend=0;

    int* instructionSectionTemp=NULL;

    debt* debtTableTemp=NULL;

    /*if a command defined with a label:*/
    if (package.commandEntry.labelBit==TRUE) {
        /*did we meet this label before?*/
        for (iForLabelArray = 0; iForLabelArray < (*numOfLabelsInLabelsArr); iForLabelArray++) {



            if (strncmp((*labelsArray)[iForLabelArray].labelName, package.commandEntry.labelName,strlen(package.commandEntry.labelName)) == 0) {
                /*1 extern, 1 definition? no way:*/
                if ((*labelsArray)[iForLabelArray].externBit == TRUE) {
                    fprintf(stdout, "\nNo way an extern declaration gets a local definition, as like %s\n", package.commandEntry.labelName);
                    return FALSE;
                }
                /*2 definitions, same name:*/
                if ((*labelsArray)[iForLabelArray].definitionAddress != 0) {
                    fprintf(stdout, "\nAnother definition have been made with %s already, can't use it again\n", package.commandEntry.labelName);
                    return FALSE;
                }
                /*what we've met was an entry declaration, now we just SOGRIM MAAGAL*/
                if ((*labelsArray)[iForLabelArray].declarationBit == TRUE) {
                    (*labelsArray)[iForLabelArray].declarationBit = FALSE;
                    (*labelsArray)[iForLabelArray].isDataOrString= FALSE;
                    newEntryBit = FALSE;
                    break;
                }

            }
        }

        /*didn't find one with the same label name, so it's a new one, lets create it:*/
        if (newEntryBit==TRUE) {
            labelsArrayTemp = (labelInfo *) realloc(*labelsArray, sizeof(labelInfo) * (*numOfLabelsInLabelsArr + 1));
            if (labelsArrayTemp == NULL) {
                fprintf(stdout, "\nAllocation failure for new entry on labels array, for %s\n", package.commandEntry.labelName);
                return FALSE;
            }

            /*Assignments:*/
            strncpy(labelsArrayTemp[*numOfLabelsInLabelsArr].labelName, package.commandEntry.labelName,strlen(package.commandEntry.labelName)+1);


            labelsArrayTemp[*numOfLabelsInLabelsArr].externBit = FALSE;
            labelsArrayTemp[*numOfLabelsInLabelsArr].entryBit = FALSE;
            labelsArrayTemp[*numOfLabelsInLabelsArr].declarationBit = FALSE;

            labelsArrayTemp[*numOfLabelsInLabelsArr].definitionAddress = (*numOfInstructionSection);
            
            labelsArrayTemp[*numOfLabelsInLabelsArr].isDataOrString = FALSE;

            *labelsArray = labelsArrayTemp;
            labelsArrayTemp = NULL;



            (*numOfLabelsInLabelsArr)++;


        }
    }

    /*into the instruction section:*/

    /*for that table-of-content record:*/
    /*destination:*/

    if(package.commandEntry.destinationBit==TRUE) {
        if (strncmp(package.commandEntry.destinationIdentifier, "NUMBER", 6) == 0) {
            recordBuffer = 1 << 2;
        }
        else if (strncmp(package.commandEntry.destinationIdentifier, "LABEL", 5) == 0) {
            recordBuffer = 3 << 2;
        }
        else if (strncmp(package.commandEntry.destinationIdentifier, "REGISTER", 8) == 0) {
            recordBuffer = 5 << 2;
        }
        theWordToSend = theWordToSend | recordBuffer;
        recordBuffer = 0;
    }

    /*source MIUN:*/
    if(package.commandEntry.sourceBit==TRUE) {
        if (strncmp(package.commandEntry.sourceIdentifier, "NUMBER", 6) == 0) {
            recordBuffer = 1 << 9;
        } else if (strncmp(package.commandEntry.sourceIdentifier, "LABEL", 5) == 0) {
            recordBuffer = 3 << 9;
        } else if (strncmp(package.commandEntry.sourceIdentifier, "REGISTER", 8) == 0) {
            recordBuffer = 5 << 9;
        }
        theWordToSend = theWordToSend | recordBuffer;
        recordBuffer = 0;
    }
    /*opcode:*/
    /*enum{mov,cmp,add,sub,not,clr,lea,inc,dec,jmp,bne,red,prn,jsr,stop};*/
    if (strncmp(package.commandEntry.commandName, "mov", 3) == 0) {
        recordBuffer = mov << 5;
    } else if (strncmp(package.commandEntry.commandName, "cmp", 3) == 0) {
        recordBuffer = cmp << 5;
    } else if (strncmp(package.commandEntry.commandName, "add", 3) == 0) {
        recordBuffer = add << 5;
    } else if (strncmp(package.commandEntry.commandName, "sub", 3) == 0) {
        recordBuffer = sub << 5;
    } else if (strncmp(package.commandEntry.commandName, "not", 3) == 0) {
        recordBuffer = not << 5;
    } else if (strncmp(package.commandEntry.commandName, "clr", 3) == 0) {
        recordBuffer = clr << 5;
    } else if (strncmp(package.commandEntry.commandName, "lea", 3) == 0) {
        recordBuffer = lea << 5;
    } else if (strncmp(package.commandEntry.commandName, "inc", 3) == 0) {
        recordBuffer = inc << 5;
    } else if (strncmp(package.commandEntry.commandName, "dec", 3) == 0) {
        recordBuffer = dec << 5;
    } else if (strncmp(package.commandEntry.commandName, "jmp", 3) == 0) {
        recordBuffer = jmp << 5;
    } else if (strncmp(package.commandEntry.commandName, "bne", 3) == 0) {
        recordBuffer = bne << 5;
    } else if (strncmp(package.commandEntry.commandName, "red", 3) == 0) {
        recordBuffer = red << 5;
    } else if (strncmp(package.commandEntry.commandName, "prn", 3) == 0) {
        recordBuffer = prn << 5;
    } else if (strncmp(package.commandEntry.commandName, "jsr", 3) == 0) {
        recordBuffer = jsr << 5;
    } else if (strncmp(package.commandEntry.commandName, "rts", 3) == 0) {
        recordBuffer = rts << 5;
    }else if (strncmp(package.commandEntry.commandName, "stop", 4) == 0) {
        recordBuffer = stop << 5;
    }
    theWordToSend = theWordToSend | recordBuffer;
    recordBuffer = 0;

    instructionSectionTemp=(int*) realloc(*instructionSection,((*numOfInstructionSection)+1)*sizeof (int));
    if (instructionSectionTemp == NULL) {
        fprintf(stdout, "\nFail to create another entry on data section\n");
        return FALSE;
    }

    instructionSectionTemp[(*numOfInstructionSection)]=theWordToSend;
    theWordToSend=0;
    *instructionSection=instructionSectionTemp;
    instructionSectionTemp=NULL;


    (*numOfInstructionSection)++;
    /*end of table-of-content record:*/

    /*source record:*/
    if(package.commandEntry.sourceBit==TRUE){
        /*AL HADERECH: case: both destination AND source are registers:*/
        if((strncmp(package.commandEntry.sourceIdentifier,"REGISTER",8)==0) &&(strncmp(package.commandEntry.destinationIdentifier,"REGISTER",8)==0)){
            /*register has packaged such as @NUM*/
            recordBuffer = 0;
            theWordToSend=0;
            recordBuffer=recordBuffer | (((int)package.commandEntry.sourceContent[1])<<9);
            recordBuffer=recordBuffer | (((int)package.commandEntry.destinationContent[1])<<2);
            theWordToSend = theWordToSend | recordBuffer;
            recordBuffer = 0;

            instructionSectionTemp=(int*) realloc(*instructionSection,((*numOfInstructionSection)+1)*sizeof (int));
            if (instructionSectionTemp == NULL) {
                fprintf(stdout, "\nFail to create another entry on data section\n");
                return FALSE;
            }
            instructionSectionTemp[(*numOfInstructionSection)]=theWordToSend;
            theWordToSend=0;
            *instructionSection=instructionSectionTemp;
            instructionSectionTemp=NULL;
            (*numOfInstructionSection)++;


        }
        else if(strncmp(package.commandEntry.sourceIdentifier,"REGISTER",8)==0){

            /*register has packaged such as @NUM*/
            recordBuffer=recordBuffer | (((int)package.commandEntry.sourceContent[1])<<9);
            theWordToSend = theWordToSend | recordBuffer;
            recordBuffer = 0;


            instructionSectionTemp=(int*) realloc(*instructionSection,((*numOfInstructionSection)+1)*sizeof (int));
            if (instructionSectionTemp == NULL) {
                return FALSE;
            }
            instructionSectionTemp[(*numOfInstructionSection)]=theWordToSend;
            *instructionSection=instructionSectionTemp;
            instructionSectionTemp=NULL;

            theWordToSend=0;
            (*numOfInstructionSection)++;

        }
        else if(strncmp(package.commandEntry.sourceIdentifier,"LABEL",5)==0){
            theWordToSend = 0;/*place holder*/

            /*into the instruction section:*/
            instructionSectionTemp=(int*) realloc(*instructionSection,((*numOfInstructionSection)+1)*sizeof (int));
            if (instructionSectionTemp == NULL) {
                fprintf(stdout, "\nFail to create another entry on data section\n");
                return FALSE;
            }
            instructionSectionTemp[(*numOfInstructionSection)]=theWordToSend;
            *instructionSection=instructionSectionTemp;

            instructionSectionTemp=NULL;

            /*into the debt:*/
            debtTableTemp = (debt *) realloc(*debtsTable,((*numOfDebts)+1)*sizeof(debt));
            if (debtTableTemp == NULL) {
                fprintf(stdout, "\nFail to create dynamic array for the debts\n");
                exit(1);
            }
            strncpy(debtTableTemp[*numOfDebts].labelName,package.commandEntry.sourceContent, strlen(package.commandEntry.sourceContent));
            debtTableTemp[*numOfDebts].debtLocation=&((*instructionSection)[(*numOfInstructionSection)]);
            debtTableTemp[*numOfDebts].iForInstruction=(*numOfInstructionSection);


            *debtsTable=debtTableTemp;
            debtTableTemp=NULL;

            (*numOfInstructionSection)++;
            (*numOfDebts)++;

        }
        else if(strncmp(package.commandEntry.sourceIdentifier,"NUMBER",6)==0){
            recordBuffer=recordBuffer | atoi(package.commandEntry.sourceContent);
            theWordToSend = theWordToSend | recordBuffer;


            recordBuffer = 0;

            instructionSectionTemp=(int*) realloc(*instructionSection,((*numOfInstructionSection)+1)*sizeof (int));
            if (instructionSectionTemp == NULL) {
                fprintf(stdout, "\nFail to create another entry on data section\n");
                return FALSE;
            }
            instructionSectionTemp[(*numOfInstructionSection)]=theWordToSend;
            *instructionSection=instructionSectionTemp;

            instructionSectionTemp=NULL;
            (*numOfInstructionSection)++;
        }


    }
    /*destination record:*/
    if(package.commandEntry.destinationBit==TRUE){


        if(strncmp(package.commandEntry.destinationIdentifier,"REGISTER",8)==0 && strncmp(package.commandEntry.sourceIdentifier,"REGISTER",8)!=0){

            recordBuffer=recordBuffer | (((int)package.commandEntry.destinationContent[1])<<2);
            theWordToSend = theWordToSend | recordBuffer;
            recordBuffer = 0;

            instructionSectionTemp=(int*) realloc(*instructionSection,((*numOfInstructionSection)+1)*sizeof (int));
            if (instructionSectionTemp == NULL) {
                fprintf(stdout, "\nFail to create another entry on data section\n");
                return FALSE;
            }
            instructionSectionTemp[(*numOfInstructionSection)]=theWordToSend;
            theWordToSend=0;
            *instructionSection=instructionSectionTemp;

            instructionSectionTemp=NULL;
            (*numOfInstructionSection)++;

        }
        else if(strncmp(package.commandEntry.destinationIdentifier,"LABEL",5)==0){
            theWordToSend = 0;/*place holder*/

            /*into the instruction section:*/
            instructionSectionTemp=(int*) realloc(*instructionSection,((*numOfInstructionSection)+1)*sizeof (int));
            if (instructionSectionTemp == NULL) {
                fprintf(stdout, "\nFail to create another entry on data section\n");
                return FALSE;
            }
            instructionSectionTemp[(*numOfInstructionSection)]=theWordToSend;
            *instructionSection=instructionSectionTemp;

            instructionSectionTemp=NULL;

            /*into the debt:*/

            debtTableTemp = (debt *) realloc(*debtsTable,((*numOfDebts)+1)*sizeof(debt));
            if (debtTableTemp == NULL) {
                fprintf(stdout, "\nFail to create dynamic array for the debts\n");
                exit(1);
            }
            strncpy(debtTableTemp[*numOfDebts].labelName,package.commandEntry.destinationContent, strlen(package.commandEntry.destinationContent));
            debtTableTemp[*numOfDebts].debtLocation=&((*instructionSection)[(*numOfInstructionSection)]);
            debtTableTemp[*numOfDebts].iForInstruction=(*numOfInstructionSection);

            *debtsTable=debtTableTemp;

            debtTableTemp=NULL;



            (*numOfInstructionSection)++;
            (*numOfDebts)++;




        }
        else if(strncmp(package.commandEntry.destinationIdentifier,"NUMBER",6)==0){
            recordBuffer=recordBuffer | atoi(package.commandEntry.destinationContent);
            theWordToSend = theWordToSend | recordBuffer;
            recordBuffer = 0;

            instructionSectionTemp=(int*) realloc(*instructionSection,((*numOfInstructionSection)+1)*sizeof (int));
            if (instructionSectionTemp == NULL) {
                return FALSE;
            }
            instructionSectionTemp[(*numOfInstructionSection)]=theWordToSend;
            *instructionSection=instructionSectionTemp;

            instructionSectionTemp=NULL;
            (*numOfInstructionSection)++;
        }


    }
    return TRUE;


}
