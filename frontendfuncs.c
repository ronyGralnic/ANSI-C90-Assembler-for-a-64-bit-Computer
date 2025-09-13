/*funcs the frontend is based of
 * validation checks, buffering the right types, squeezing infos form each line
 *
 * pay attention to the labeling/numbering/funcs:
 * no fprintf documentation, NO TRACES
 * whule buffering the source/ dest parameters: this phase is going through TRIAL AND ERROR  between the three, only one winner
 * */



/*processing a potentially entry/extern line.
 * structure:
 *          .entry SOMETHING
 *       or .extern SOMETHING */
static int processingEntryOrExternLine(char* lineFromAM, char* labelBuffer){
    int iForLine=0;
    int iForLabelBuffer=0;

    int dotBit=TRUE;
    int labelBit=FALSE;
    int cleanBit=FALSE;

    while(iForLine<MAXLINESIZE){
        /* .entry/.extern phase:*/
        while(dotBit==TRUE){
            /*these white chars:*/
            if(lineFromAM[iForLine]==' '||lineFromAM[iForLine]=='\t'){
                iForLine++;
            }
            /*good start:*/
            else if (lineFromAM[iForLine]=='.'){
                dotBit=FALSE;
                labelBit=TRUE;
                /*AL HADERECH: "entry"/"extern" sticks right next:*/
                iForLine++;
                if(strncmp(&lineFromAM[iForLine],"entry "/*mind the space*/,6)==0) {
                    iForLine+=6;
                }
                else if(strncmp(&lineFromAM[iForLine],"extern "/*mind the space*/,7)==0) {
                    iForLine+=7;
                }
                /*got the dot, miss the entry/extern*/
                else {
                    fprintf(stdout,"\nNo \"entry\" or \"extern\" on time for %s\n",lineFromAM);
                    return FALSE;
                }

            }
            /*miss the dot at the start*/
            else{
                fprintf(stdout,"\nits dot time, if entry/extern is all about for %s\n",lineFromAM);
                return FALSE;

            }

            /*----------------------------------------------------------------------------*/
        }
        /*label phase:*/
        while(labelBit==TRUE){
                        /*those white chars before the label*/
            if(iForLabelBuffer==0 &&    (   lineFromAM[iForLine]==' '  ||  lineFromAM[iForLine]=='\t'  )   ){
                iForLine++;
            }
            /*first letter always alpha;*/
            else if(iForLabelBuffer==0 && isalpha(lineFromAM[iForLine])) {
                labelBuffer[iForLabelBuffer]=lineFromAM[iForLine];
                iForLabelBuffer++;
                iForLine++;

            }
            /*for the other letters:*/
            else if (iForLabelBuffer>0 &&   (isalpha(lineFromAM[iForLine])  ||  isdigit(lineFromAM[iForLine])   )   ){
                labelBuffer[iForLabelBuffer]=lineFromAM[iForLine];
                iForLabelBuffer++;
                iForLine++;
            }
            /*that first white char after the label:*/
            else if(iForLabelBuffer>0 &&     (   lineFromAM[iForLine]==' '  ||  lineFromAM[iForLine]=='\t'||  lineFromAM[iForLine]=='\n'||  lineFromAM[iForLine]=='\0'  )   ){
                labelBit=FALSE;
                cleanBit=TRUE;
                iForLine++;
            }
            /*invalid label name:*/
            else{
                fprintf(stdout,"\ninvalid label phase for %s\n",lineFromAM);
                return FALSE;
            }
        }

        /*redundant input: */
        while(cleanBit==TRUE && iForLine<MAXLINESIZE ){
            if(lineFromAM[iForLine]!='\t' && lineFromAM[iForLine]!=' '){
                fprintf(stdout,"\nweve noticed a reduandant char is %s, it will be neglected with other after him, if existed\n",lineFromAM);
                /* no big deal */
                labelBuffer[iForLabelBuffer]='\0';
                return TRUE;
            }
            iForLine++;

        }

    }

    labelBuffer[iForLabelBuffer]='\0';
    return TRUE;

}
/*potentially data/string definition
 * structure:
 *          LABEL: .string "Sdsfr"
 *       or LABEL: .data ,1,24,-5 , 55*/
static int processingDataOrStringLine(char* lineFromAM,char* labelBuffer, char* parametersBuffer,int* numOfParameters){
    int iForLine=0;
    int iForLabelBuffer=0;
    int iForParametersBuffer=0;


    int labelBit=TRUE;
    int dotBit=FALSE;
    int parametersBit=FALSE;
    int cleanBit=FALSE;

    int commaBit=FALSE;
    int minusBit=FALSE;

    (*numOfParameters)=0;

    while(iForLine<MAXLINESIZE){
        /*label phase:*/
        while (labelBit==TRUE){

            if(iForLabelBuffer==0 &&    (   lineFromAM[iForLine]==' '  ||  lineFromAM[iForLine]=='\t'  )   ){
                iForLine++;
            }
            /*first letter always alpha;*/
            else if(iForLabelBuffer==0 && isalpha(lineFromAM[iForLine])) {
                labelBuffer[iForLabelBuffer]=lineFromAM[iForLine];
                iForLabelBuffer++;
                iForLine++;

            }
            /*for the other letters:*/
            else if (iForLabelBuffer>0 &&   (isalpha(lineFromAM[iForLine])  ||  isdigit(lineFromAM[iForLine])   )   ){
                labelBuffer[iForLabelBuffer]=lineFromAM[iForLine];
                iForLabelBuffer++;
                iForLine++;
            }
            else if (lineFromAM[iForLine]==':'){
                labelBit=FALSE;
                dotBit=TRUE;
                iForLine++;
            }
            /*miss on the label check:*/
            else{
                fprintf(stdout,"\ninvalid label phase for %s\n",lineFromAM);
                return FALSE;

            }

        }

        while(dotBit==TRUE){
            if(lineFromAM[iForLine]==' '||lineFromAM[iForLine]=='\t'){
                iForLine++;
            }
            else if (lineFromAM[iForLine]=='.') {
                dotBit = FALSE;
                parametersBit=TRUE;
                /*AL HADERECH: "data"/"string" sticks right next:*/
                iForLine++;
                if (strncmp(&lineFromAM[iForLine], "string "/*mind the space*/, 7) == 0) {
                    iForLine += 7;
                }
                else if (strncmp(&lineFromAM[iForLine], "data "/*mind the space*/, 5) == 0) {
                    iForLine += 5;
                }
                else {/*got the dot, miss the 'string' or 'data'*/
                    fprintf(stdout, "\nNo \"data\" or \"string\" on time for %s\n", lineFromAM);
                    return FALSE;
                }
            }
            else{/*miss the dot*/
                fprintf(stdout,"\ninvalid data/string phase for %s\n",lineFromAM);
                return FALSE;
            }



        }
        /*parameters phase, for string: "adasfwqe"*/
        while (parametersBit==TRUE && strstr(lineFromAM,".string")){
            if(iForParametersBuffer==0 &&    (   lineFromAM[iForLine]==' '  ||  lineFromAM[iForLine]=='\t'  )   ){
                iForLine++;
            }
            /*nice start:*/
            else if( lineFromAM[iForLine]=='"'){
                /*into the string:*/
                iForLine++;
                /*buffering:*/
                while(lineFromAM[iForLine]!='"' &&iForLine<MAXLINESIZE){

                    parametersBuffer[iForParametersBuffer]=lineFromAM[iForLine];
                    iForParametersBuffer++;
                    iForLine++;
                    (*numOfParameters)++;

                }
                if (lineFromAM[iForLine]=='"'){
                    parametersBit=FALSE;
                    cleanBit=TRUE;
                    iForLine++;

                }

                else{/*miss on the second "  :*/
                    fprintf(stdout,"\nMissing closing \" for %s\n",lineFromAM);
                    return FALSE;

                }

            }
            else{/*miss on the first " :*/
                fprintf(stdout,"\ncant find the \" opener %s\n",lineFromAM);
                return FALSE;
            }
        }

        /*ATTENTION: NO CLEANBIT WILL BE AFFECTED, THE INPUT TOO FLUID. WE'LL JUST RUN TILL THE END*/
        /*data paremters phase: 3,45 , -2*/
        while (parametersBit==TRUE && strstr(lineFromAM,".data")){
            while(iForLine<MAXLINESIZE){
                /*these white chars before the storm*/
                if (iForParametersBuffer==0&&(lineFromAM[iForLine]==' ' || lineFromAM[iForLine]=='\t')){
                    iForLine++;

                }

                else if (isdigit(lineFromAM[iForLine])||lineFromAM[iForLine]=='+'||lineFromAM[iForLine]=='-'){
                    if (lineFromAM[iForLine]=='+'){
                        if (minusBit==TRUE){
                            iForLine++;
                            if (!(isdigit(lineFromAM[iForLine]))){
                                return FALSE;/*like "+-". terrible*/
                            }
                            minusBit=FALSE;
                        }
                        else{/*not the time for + */
                            fprintf(stdout,"\nNot the time for + in %s\n",lineFromAM);
                            return FALSE;
                        }
                    }
                    else if (lineFromAM[iForLine]=='-'){
                        if (minusBit==TRUE){
                            parametersBuffer[iForParametersBuffer] = lineFromAM[iForLine];
                            iForParametersBuffer++;
                            iForLine++;
                            if (!(isdigit(lineFromAM[iForLine]))){
                                return FALSE;/*like "-,". terrible*/
                            }
                            minusBit=FALSE;
                            commaBit=FALSE;

                        }
                        else{/*not the time for - */
                            fprintf(stdout,"\nNot the time for - in %s\n",lineFromAM);
                            return FALSE;

                        }
                    }
                    else if (isdigit(lineFromAM[iForLine])) {
                        parametersBuffer[iForParametersBuffer] = lineFromAM[iForLine];
                        iForParametersBuffer++;
                        iForLine++;
                        if (lineFromAM[iForLine]=='\n' ||iForLine==MAXLINESIZE){
                            parametersBuffer[iForParametersBuffer]='\0';
                            labelBuffer[iForLabelBuffer]='\0';
                            (*numOfParameters)++;
                            return TRUE;
                        }
                        if (lineFromAM[iForLine]==','){
                            parametersBuffer[iForParametersBuffer] = lineFromAM[iForLine];
                            iForParametersBuffer++;
                            iForLine++;
                            (*numOfParameters)++;
                            commaBit=FALSE;
                            minusBit=TRUE;
                        }

                        else{
                            minusBit=FALSE;
                            commaBit=TRUE;
                        }

                    }
                }
                else if (lineFromAM[iForLine]==','){
                    if (commaBit==TRUE){
                        parametersBuffer[iForParametersBuffer] = lineFromAM[iForLine];
                        iForParametersBuffer++;
                        iForLine++;
                        (*numOfParameters)++;
                        commaBit=FALSE;
                        minusBit=TRUE;
                        if (lineFromAM[iForLine]=='\n' ||iForLine==MAXLINESIZE){
                            fprintf(stdout,"\ncomma declaration, no number after in %s\n",lineFromAM);
                            return FALSE;
                        }
                    }
                    else{/*comma not on time*/
                        fprintf(stdout,"\n*comma not on time in %s\n",lineFromAM);
                        return FALSE;
                    }

                }
                else if (lineFromAM[iForLine]==' ' || lineFromAM[iForLine]=='\t'){
                    iForLine++;
                    if (isdigit(lineFromAM[iForLine]) && commaBit==TRUE){/*basically "6 6" . terrible*/
                        fprintf(stdout,"\ninvalid num phase in %s\n",lineFromAM);
                        return FALSE;
                    }
                }
                else{/*all the other junk*/
                    fprintf(stdout,"\ninvalid num phase in %s\n",lineFromAM);
                    return FALSE;
                }




            }
        }
        /*redundant chars phase:*/
        while(cleanBit==TRUE){
            while (iForLine<MAXLINESIZE){
                if (lineFromAM[iForLine]!=' ' && lineFromAM[iForLine]!='\t'){
                    fprintf(stdout,"\nweve noticed a reduandant char is %s, it will be neglected with other after him, if existed\n",lineFromAM);
                    labelBuffer[iForLabelBuffer]='\0';
                    parametersBuffer[iForParametersBuffer]='\0';
                    return TRUE;
                }
                iForLine++;
            }
            labelBuffer[iForLabelBuffer]='\0';
            parametersBuffer[iForParametersBuffer]='\0';
            return TRUE;


        }

    }
    return FALSE;/*end of the func AND miss the chances to finish*/

}
/*potentially command line:
 * structrue:
 *          LABEL: valid-command-name parameters
 *       or valid-command-name parameters*/

static int processingCommand(char* lineFromAM, char* labelBuffer, char* commandBuffer, char* sourceBuffer, char* destinationBuffer ){
    int iForLine=0;
    int iForLabelBuffer=0;
    int iForCommandBuffer=0;
    /*i for source/destination buffer is in labeling/numbering/registering*/



    int labelBit=TRUE;
    int commandBit=TRUE;
    int cleanBit=FALSE;
    int sourceBit=FALSE;
    int destinationBit=FALSE;
    int commaBit=TRUE;

    int toProceed=0;


    while(iForLine<MAXLINESIZE) {
        /*if a label definition is around:*/
        if (strstr(lineFromAM, ":")) {
            while (labelBit == TRUE) {
                /*those white chars before the label*/
                if (iForLabelBuffer == 0 && (lineFromAM[iForLine] == ' ' || lineFromAM[iForLine] == '\t')) {
                    iForLine++;
                }
                    /*first letter always alpha;*/
                else if (iForLabelBuffer == 0 && isalpha(lineFromAM[iForLine])) {
                    labelBuffer[iForLabelBuffer] = lineFromAM[iForLine];
                    iForLabelBuffer++;
                    iForLine++;

                }
                /*for the other letters:*/
                else if (iForLabelBuffer > 0 && (isalpha(lineFromAM[iForLine]) || isdigit(lineFromAM[iForLine]))) {
                    labelBuffer[iForLabelBuffer] = lineFromAM[iForLine];
                    iForLabelBuffer++;
                    iForLine++;
                }
                /*the closure:*/
                else if (iForLabelBuffer > 0 && lineFromAM[iForLine] == ':') {
                    labelBit = FALSE;
                    commandBit = TRUE;
                    iForLine++;

                }
                else {/*miss on the label name*/
                    fprintf(stdout, "\ninvalid label phase for %s\n", lineFromAM);
                    return FALSE;
                }
            }
        }/*%%%%%%%%%%%%%%%%%%    END OF THE CHANCE TO BUFFER A LABEL   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

        /*command phase:*/
        while (commandBit == TRUE) {
            /*those white chars before the storm:*/
            if (iForCommandBuffer == 0 && (lineFromAM[iForLine] == ' ' || lineFromAM[iForLine] == '\t')) {
                iForLine++;
            }
            /*gotcha*/
            else if (islower(lineFromAM[iForLine])) {
                /*99.9999% of our commands are a 3-letter word*/
                while (iForCommandBuffer != 3) {
                    /*small letters only:*/
                    if (islower(lineFromAM[iForLine])) {
                        commandBuffer[iForCommandBuffer] = lineFromAM[iForLine];
                        iForCommandBuffer++;
                        iForLine++;
                    }
                    /*told ya*/
                    else {
                        fprintf(stdout, "\ninvalid command phase for %s\n", lineFromAM);
                        return FALSE;
                    }
                }
                /*got 3-letter word in our pocket. Is it valid command?*/
                if (iForCommandBuffer == 3) {
                    if (threeLettersCommandValidation(commandBuffer, &sourceBit, &destinationBit, &commaBit) == TRUE) {
                        /*sourceBit/destinationBit determined in the func above, depending on the command name*/
                        commandBit = FALSE;
                        iForLine++;

                    }
                    /*invalid name? well, we have one more try: stop:*/
                    else {
                        if (islower(lineFromAM[iForLine])) {
                            commandBuffer[iForCommandBuffer] = lineFromAM[iForLine];
                            iForCommandBuffer++;
                            iForLine++;
                            /*stop is that you?:*/
                            if (strncmp(commandBuffer, "stop", 4) == 0) {
                                commandBit = FALSE;
                                sourceBit = FALSE;
                                destinationBit = FALSE;
                                iForLine++;
                            }
                            /*game over*/
                            else {
                                fprintf(stdout, "\ninvalid command name for %s\n", lineFromAM);
                                return FALSE;
                            }


                        }
                        /*couldn't reach to our last try: no small case letter*/
                        else {
                            fprintf(stdout, "\ninvalid command name for %s\n", lineFromAM);
                            return FALSE;

                        }
                    }
                }
            }
            /*you way off:*/
            else {
                fprintf(stdout, "\ninvalid command name for %s\n", lineFromAM);
                return FALSE;
            }

        }

        /*source phase, determined by the valid command names function called above, AL HADEREECH */
        while (sourceBit != FALSE) {

            if (sourceBit == EVERYTHING) {
                if (labeling(lineFromAM,iForLine,sourceBuffer,&toProceed) == FALSE) {
                    if (registering(lineFromAM,iForLine,sourceBuffer,&toProceed) == FALSE) {
                        if (numbering(lineFromAM,iForLine,sourceBuffer,&toProceed) == FALSE) {
                            fprintf(stdout, "\ninvalid source phase for %s\n", lineFromAM);
                            return FALSE;
                        } else {
                            iForLine += toProceed;
                            sourceBit = FALSE;
                        }
                    } else {
                        iForLine += toProceed;
                        sourceBit = FALSE;
                    }
                } else {
                    iForLine += toProceed;
                    sourceBit = FALSE;
                }
            } else if (sourceBit == ONLYTHREE) {
                if (labeling(lineFromAM,iForLine,sourceBuffer,&toProceed) == FALSE) {
                    fprintf(stdout, "\ninvalid source phase for %s\n", lineFromAM);
                    return FALSE;

                } else {
                    iForLine += toProceed;
                    sourceBit = FALSE;
                }
            } else {
                fprintf(stdout, "\ninvalid source phase for %s\n", lineFromAM);
                return FALSE;

            }

        }

        /*destination phase, detemined by the valid command names function called above, AL HADEREECH */
        while (destinationBit != FALSE) {

            while (commaBit == TRUE) {

                if (lineFromAM[iForLine] == ' ' || lineFromAM[iForLine] == '\t'|| lineFromAM[iForLine] == '\0') {
                    iForLine++;
                } else if (lineFromAM[iForLine] == ',') {
                    commaBit = FALSE;
                    iForLine++;
                } else {
                    fprintf(stdout, "\nmissing comma between source to dest phase in %s\n", lineFromAM);
                    return FALSE;
                }
            }

            if (destinationBit == EVERYTHING) {
                if (labeling(lineFromAM, iForLine, destinationBuffer, &toProceed) == FALSE) {
                    if (registering(lineFromAM, iForLine, destinationBuffer, &toProceed) == FALSE) {
                        if (numbering(lineFromAM, iForLine, destinationBuffer, &toProceed) == FALSE) {
                            fprintf(stdout, "\ninvalid destination phase for %s\n", lineFromAM);
                            return FALSE;
                        } else {
                            iForLine += toProceed;
                            destinationBit = FALSE;
                        }
                    } else {
                        iForLine += toProceed;
                        destinationBit = FALSE;
                    }
                }
                else {
                    iForLine += toProceed;
                    destinationBit = FALSE;
                }
            }
            else if (destinationBit == THREEANDFIVE) {
                if (labeling(lineFromAM,iForLine,destinationBuffer,&toProceed) == FALSE) {

                    if (registering(lineFromAM, iForLine, destinationBuffer, &toProceed) == FALSE) {
                        fprintf(stdout, "\ninvalid destination phase for %s\n", lineFromAM);
                        return FALSE;
                    }
                    else {
                        destinationBit = FALSE;
                        iForLine += toProceed;
                    }
                }
                else {
                    destinationBit = FALSE;
                    iForLine += toProceed;
                }
            }
            else {
                fprintf(stdout, "\ninvalid destination phase for %s\n", lineFromAM);
                return FALSE;
            }
        }
        if (iForCommandBuffer > 0 && sourceBit == FALSE && destinationBit == FALSE) {
            cleanBit = TRUE;
        }
        while (cleanBit == TRUE) {
            while (lineFromAM[iForLine] != '\n') {
                if (lineFromAM[iForLine] != ' ' && lineFromAM[iForLine] != '\t') {
                    fprintf(stdout,"\nweve noticed a reduandant char is %s, it will be neglected with other after him, if existed\n",lineFromAM);
                    labelBuffer[iForLabelBuffer]='\0';
                    commandBuffer[iForCommandBuffer]='\0';
                    return TRUE;
                }
                iForLine++;
            }
            labelBuffer[iForLabelBuffer]='\0';
            commandBuffer[iForCommandBuffer]='\0';
            return TRUE;


        }
    }
    labelBuffer[iForLabelBuffer]='\0';
    commandBuffer[iForCommandBuffer]='\0';
    return TRUE;
}
/*for the processing command function, when determing the command validation, we've gathered all our 3-word-command possible
 * this will help determine validation of course, but some of our next coming steps too*/
static int threeLettersCommandValidation(char* commandBuffer,int* sourceBit, int* destinationBit, int* commaBit){

    if(strncmp(commandBuffer,"mov",3)==0){
        (*sourceBit)=EVERYTHING;
        (*destinationBit)=THREEANDFIVE;
        (*commaBit)=TRUE;
        return TRUE;
    }
    else if(strncmp(commandBuffer,"cmp",3)==0){
        (*sourceBit)=EVERYTHING;
        (*destinationBit)=EVERYTHING;
        (*commaBit)=TRUE;
        return TRUE;
    }
    else if(strncmp(commandBuffer,"add",3)==0){
        (*sourceBit)=EVERYTHING;
        (*destinationBit)=THREEANDFIVE;
        (*commaBit)=TRUE;
        return TRUE;
    }
    else if(strncmp(commandBuffer,"sub",3)==0){
        (*sourceBit)=EVERYTHING;
        (*destinationBit)=THREEANDFIVE;
        (*commaBit)=TRUE;
        return TRUE;
    }
    else if(strncmp(commandBuffer,"not",3)==0){
        (*sourceBit)=FALSE;
        (*destinationBit)=THREEANDFIVE;
        (*commaBit)=FALSE;
        return TRUE;
    }
    else if(strncmp(commandBuffer,"clr",3)==0){
        (*sourceBit)=FALSE;
        (*destinationBit)=THREEANDFIVE;
        (*commaBit)=FALSE;
        return TRUE;
    }

    else if(strncmp(commandBuffer,"lea",3)==0){
        (*sourceBit)=ONLYTHREE;
        (*destinationBit)=THREEANDFIVE;
        (*commaBit)=TRUE;
        return TRUE;
    }else if(strncmp(commandBuffer,"inc",3)==0){
        (*sourceBit)=FALSE;
        (*destinationBit)=THREEANDFIVE;
        (*commaBit)=FALSE;
        return TRUE;
    }
    else if(strncmp(commandBuffer,"dec",3)==0){
        (*sourceBit)=FALSE;
        (*destinationBit)=THREEANDFIVE;
        (*commaBit)=FALSE;
        return TRUE;
    }
    else if(strncmp(commandBuffer,"jmp",3)==0){
        (*sourceBit)=FALSE;
        (*destinationBit)=THREEANDFIVE;
        (*commaBit)=FALSE;
        return TRUE;
    }
    else if(strncmp(commandBuffer,"bne",3)==0){
        (*sourceBit)=FALSE;
        (*destinationBit)=THREEANDFIVE;
        (*commaBit)=FALSE;
        return TRUE;
    }
    else if(strncmp(commandBuffer,"red",3)==0){
        (*sourceBit)=FALSE;
        (*destinationBit)=THREEANDFIVE;
        (*commaBit)=FALSE;
        return TRUE;
    }
    else if(strncmp(commandBuffer,"prn",3)==0){
        (*sourceBit)=FALSE;
        (*destinationBit)=EVERYTHING;
        (*commaBit)=FALSE;
        return TRUE;
    }
    else if(strncmp(commandBuffer,"jsr",3)==0){
        (*sourceBit)=FALSE;
        (*destinationBit)=THREEANDFIVE;
        (*commaBit)=FALSE;
        return TRUE;
    }
    else if(strncmp(commandBuffer,"rts",3)==0){
        (*sourceBit)=FALSE;
        (*destinationBit)=FALSE;
        (*commaBit)=FALSE;
        return TRUE;
    }

    else{
        return FALSE;
    }
}
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

/*for any parameters' phase, in the command processing label is an option:*/
static int labeling(char* lineFromAM, int iForLine ,char* buffer, int *toProceed) {
    int iForBuffer=0;
    (*toProceed)=0;
    while (iForLine < MAXLINESIZE) {

        /*those white chars before the label*/
        if ((iForBuffer) == 0 && (lineFromAM[iForLine] == ' ' || lineFromAM[iForLine] == '\t')) {
            (iForLine)++;
            (*toProceed)++;
        }
        else if (iForBuffer == 0 &&(!isalpha(lineFromAM[iForLine]))){
            return FALSE;
        }
        /*first letter always alpha;*/
        else if (iForBuffer == 0 && isalpha(lineFromAM[iForLine])) {
            buffer[iForBuffer] = lineFromAM[iForLine];
            iForBuffer++;
            iForLine++;
            (*toProceed)++;

        }
            /*for the other letters:*/
        else if (iForBuffer > 0 && (isalpha(lineFromAM[iForLine]) || isdigit(lineFromAM[iForLine]))) {
            buffer[iForBuffer] = lineFromAM[iForLine];
            iForBuffer++;
            iForLine++;
            (*toProceed)++;
        }
            /*that first white char after the label:*/
        else if ((iForBuffer) > 0 &&
                 (lineFromAM[iForLine] == ' ' || lineFromAM[iForLine] == '\t' || lineFromAM[iForLine] == '\n' ||
                  lineFromAM[iForLine] == '\0')) {
            buffer[iForBuffer]='\0';
            return TRUE;
        }
        else {
            return FALSE;
        }

    }
    return FALSE;
}

/*for any parameters' phase, in the command processing number is an option:*/
static int numbering(char lineFromAM[MAXLINESIZE], int iForLine,char *buffer, int *toProceed) {
    int iForBuffer = 0;
    int minusBit = TRUE;
    (*toProceed)=0;
    while ((iForLine) < MAXLINESIZE) {
        if (iForBuffer == 0 && (lineFromAM[iForLine] == ' ' || lineFromAM[iForLine] == '\t')) {
            iForLine++;
            (*toProceed)++;
        }
        else if (iForBuffer == 0 &&lineFromAM[iForLine] != '-'&&lineFromAM[iForLine] != '+'&&(!isdigit(lineFromAM[iForLine])) ){
            return FALSE;
        }

        else if (isdigit(lineFromAM[iForLine])) {
            buffer[iForBuffer] = lineFromAM[iForLine];
            iForBuffer++;
            iForLine++;
            (*toProceed)++;
            minusBit = FALSE;

        }
        else if (lineFromAM[iForLine] == '-') {


            if (minusBit == TRUE) {
                buffer[iForBuffer] = lineFromAM[iForLine];
                iForBuffer++;
                iForLine++;
                (*toProceed)++;
                minusBit = FALSE;

            } else {
                return FALSE;
            }
        } else if (lineFromAM[iForLine] == '+') {
            if (minusBit == TRUE) {
                iForLine++;
                (*toProceed)++;
                minusBit = FALSE;
            } else {
                return FALSE;
            }
        }
        else if (iForBuffer > 0 && (lineFromAM[iForLine] == ' ' || lineFromAM[iForLine] == '\t'|| lineFromAM[iForLine] == '\0'|| lineFromAM[iForLine] == '\n')){
            buffer[iForBuffer]='\0';
            return TRUE;
        }
        else{
            return FALSE;
        }

    }
    return FALSE;
}
/*for any parameters' phase, in the command processing regsiter is an option:*/
static int registering(char* lineFromAM, int iForLine, char* buffer, int *toProceed){
    int iForBuffer=0;
    (*toProceed)=0;
    while ((iForLine) < MAXLINESIZE) {

        if (iForBuffer == 0 && (lineFromAM[iForLine] == ' ' || lineFromAM[iForLine] == '\t')) {
            iForLine++;
            (*toProceed)++;
        }
        else if (iForBuffer == 0 &&lineFromAM[iForLine] != '@' ){
            return FALSE;
        }

        else if (lineFromAM[iForLine] == '@') {/*i want hin to be an identifier*/
            /*a stretch of @r0 for example:*/
            if (iForBuffer==0) {

                buffer[iForBuffer] = lineFromAM[iForLine];
                iForBuffer++;
                iForLine++;
                (*toProceed)++;

                if (lineFromAM[iForLine] == 'r') {
                    iForLine++;
                    (*toProceed)++;
                    if (lineFromAM[iForLine] >= '0' && lineFromAM[iForLine] <= '7') {
                        buffer[iForBuffer] = lineFromAM[iForLine];
                        iForBuffer++;
                        iForLine++;
                        (*toProceed)++;
                        buffer[iForBuffer]='\0';
                        return TRUE;
                    }
                    else {/*out of register numbers range*/
                        return FALSE;

                    }

                }
                else {/*we need r right after the @*/
                    return FALSE;
                }
            }
            else{/*@ came not in the right time*/
                return FALSE;
            }

        }
        else {
            return FALSE;
        }

    }
    return FALSE;

}

static int commandsWithSource(char commandName[4]){
    if(strncmp(commandName,"mov",3)==0){
        return TRUE;
    }
    else if(strncmp(commandName,"cmp",3)==0){
        return TRUE;
    }
    else if(strncmp(commandName,"add",3)==0){
        return TRUE;
    }
    else if(strncmp(commandName,"sub",3)==0){
        return TRUE;
    }
    else if(strncmp(commandName,"lea",3)==0) {
        return TRUE;
    }
    else{
        return FALSE;
    }


    return FALSE;
}

static int commandsWithDestination(char commandName[4]){
    if(strncmp(commandName,"mov",3)==0){
        return TRUE;
    }
    else if(strncmp(commandName,"cmp",3)==0){
        return TRUE;
    }
    else if(strncmp(commandName,"add",3)==0){
        return TRUE;
    }
    else if(strncmp(commandName,"sub",3)==0){
        return TRUE;
    }
    else if(strncmp(commandName,"not",3)==0){
        return TRUE;
    }
    else if(strncmp(commandName,"clr",3)==0){
        return TRUE;
    }

    else if(strncmp(commandName,"lea",3)==0){
        return TRUE;
    }
    else if(strncmp(commandName,"inc",3)==0){
        return TRUE;
    }
    else if(strncmp(commandName,"dec",3)==0){
        return TRUE;
    }
    else if(strncmp(commandName,"jmp",3)==0){
        return TRUE;
    }
    else if(strncmp(commandName,"bne",3)==0){
        return TRUE;
    }
    else if(strncmp(commandName,"red",3)==0){
        return TRUE;
    }
    else if(strncmp(commandName,"prn",3)==0){
        return TRUE;
    }
    else if(strncmp(commandName,"jsr",3)==0){
        return TRUE;
    }
    else{
        return FALSE;
    }

    return FALSE;
}


