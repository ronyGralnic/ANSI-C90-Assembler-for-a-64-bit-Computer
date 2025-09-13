assembler: preprocessor.o middlend.o frontend.o backend.o

	gcc -ansi -Wall -pedantic -g  preprocessor.o middlend.o frontend.o backend.o -o assembler -lm
	
preprocessor.o: preprocessor.h preprocessor.c preprocessorfuncs.c
	gcc -c -ansi -Wall -pedantic preprocessor.c  -o preprocessor.o


middlend.o: middlend.c middlend.h middlendfuncs.c
	gcc -c -ansi -Wall -pedantic middlend.c  -o middlend.o
	
frontend.o:  frontend.c frontendfuncs.c frontend.h
	gcc -c -ansi -Wall -pedantic frontend.c -o frontend.o
	
backend.o: backend.c backend.h 
	gcc -c -ansi -Wall -pedantic  backend.c -o backend.o
	
