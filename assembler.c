/**
 * Project 1
 * Assembler code fragment for LC-2K
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
static inline int isNumber(char *);

typedef struct {
    char label[6];
    int lineNum;
} Labels;

typedef struct {
    char type[1];
    int opcode;
    int regA;
    int regB;
    int regDest;
    int offset;
} Instruction;

int
main(int argc, char **argv)
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    /* here is an example for how to use readAndParse to read a line from
        inFilePtr */
    Labels labels[1000];
    int counter = 0;
    while ( readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
        for (int i; i < counter; i++) {
            if (!strcmp(labels[i].label, label) && strlen(label) != 0) {
                exit(1);
            }
        }
        if (isNumber(label) || isNumber(opcode)) {
            exit(1);
        }
        if (strcmp(label, "")) {
            strcpy(labels[counter].label, label);
            labels[counter].lineNum = counter;
  
        }
        counter++;
    }

    /*for (int i = 0; i < 10; i++) {
        printf("%s\n", labels[i].label);
        printf("%d\n", labels[i].lineNum);
    }*/

    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    
    rewind(inFilePtr);

    Instruction instructions[1000];
    counter = 0;
    while ( readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
        if (!strcmp(opcode, "lw")) {
            if (!(isNumber(arg0) && isNumber(arg1))) {
                exit(1);
            }
            strcpy(instructions[counter].type, "I");
            instructions[counter].opcode = 0b010;
            instructions[counter].regA = atoi(arg0);
            instructions[counter].regB = atoi(arg1);
            if ((instructions[counter].regA < 0 || instructions[counter].regA > 7) 
            || (instructions[counter].regB < 0 || instructions[counter].regB > 7)) {
                exit(1);
            }
            if ( isNumber(arg2) ) {
                instructions[counter].offset = atoi(arg2);
                if (instructions[counter].offset < -32768 || instructions[counter].offset > 32767) {
                    exit(1);
                }
            } else {
                bool labelExists = false;
                for (int i = 0; i < 1000; i++) {
                    if (!strcmp(labels[i].label, arg2)) {
                        instructions[counter].offset = labels[i].lineNum;
                        if (instructions[counter].offset < -32768 || instructions[counter].offset > 32767) {
                            exit(1);
                        }
                        labelExists = true;
                        break; // Exit the loop once the label is found
                    }
                }
                if (labelExists == false) {
                    exit(1);
                }
            }
             
        } 
        else if (!strcmp(opcode, "add")) {
            if (!(isNumber(arg0) && isNumber(arg1) && isNumber(arg2))) {
                exit(1);
            }
            strcpy(instructions[counter].type, "R");
            instructions[counter].opcode = 0b000;
            instructions[counter].regA = atoi(arg0);
            instructions[counter].regB = atoi(arg1);
            instructions[counter].regDest = atoi(arg2);
            if ((instructions[counter].regA < 0 || instructions[counter].regA > 7) 
            || (instructions[counter].regB < 0 || instructions[counter].regB > 7)
            || (instructions[counter].regDest < 0 || instructions[counter].regDest > 7)) {
                exit(1);
            }
        } 
        else if (!strcmp(opcode, "nor")) {
            if (!(isNumber(arg0) && isNumber(arg1) && isNumber(arg2))) {
                exit(1);
            }
            strcpy(instructions[counter].type, "R");
            instructions[counter].opcode = 0b001;
            instructions[counter].regA = atoi(arg0);
            instructions[counter].regB = atoi(arg1);
            instructions[counter].regDest = atoi(arg2);
            if ((instructions[counter].regA < 0 || instructions[counter].regA > 7) 
            || (instructions[counter].regB < 0 || instructions[counter].regB > 7)
            || (instructions[counter].regDest < 0 || instructions[counter].regDest > 7)) {
                exit(1);
            }
        }
        else if (!strcmp(opcode, "sw")) {
            if (!(isNumber(arg0) && isNumber(arg1))) {
                exit(1);
            }
            strcpy(instructions[counter].type, "I");
            instructions[counter].opcode = 0b011;
            instructions[counter].regA = atoi(arg0);
            instructions[counter].regB = atoi(arg1);
            if ((instructions[counter].regA < 0 || instructions[counter].regA > 7) 
            || (instructions[counter].regB < 0 || instructions[counter].regB > 7)) {
                exit(1);
            }
            if ( isNumber(arg2) ) {
                instructions[counter].offset = atoi(arg2);
                if (instructions[counter].offset < -32768 || instructions[counter].offset > 32767) {
                    exit(1);
                }
            } else {
                bool labelExists = false;
                for (int i = 0; i < 1000; i++) {
                    if (!strcmp(labels[i].label, arg2)) {
                        instructions[counter].offset = labels[i].lineNum;
                        if (instructions[counter].offset < -32768 || instructions[counter].offset > 32767) {
                            exit(1);
                        }
                        labelExists = true;
                        break; // Exit the loop once the label is found
                    }
                }
                if (labelExists == false) {
                    exit(1);
                }
            }
        }
        else if (!strcmp(opcode, "beq")) {
            if (!(isNumber(arg0) && isNumber(arg1))) {
                exit(1);
            }
            strcpy(instructions[counter].type, "I");
            instructions[counter].opcode = 0b100;
            instructions[counter].regA = atoi(arg0);
            instructions[counter].regB = atoi(arg1);
            if ((instructions[counter].regA < 0 || instructions[counter].regA > 7) 
            || (instructions[counter].regB < 0 || instructions[counter].regB > 7)) {
                exit(1);
            }
            if ( isNumber(arg2) ) {
                instructions[counter].offset = atoi(arg2);
                if (instructions[counter].offset < -32768 || instructions[counter].offset > 32767) {
                    exit(1);
                }
            } else {
                bool labelExists = false;
                for (int i = 0; i < 1000; i++) {
                    if (!strcmp(labels[i].label, arg2)) {
                        instructions[counter].offset = labels[i].lineNum - counter - 1;
                        if (instructions[counter].offset < -32768 || instructions[counter].offset > 32767) {
                            exit(1);
                        }
                        labelExists = true;
                        break; // Exit the loop once the label is found
                    }
                }
                if (labelExists == false) {
                    exit(1);
                }
            }
        }
        else if (!strcmp(opcode, "jalr")) {
            if (!(isNumber(arg0) && isNumber(arg1))) {
                exit(1);
            }
            strcpy(instructions[counter].type, "J");
            instructions[counter].opcode = 0b101;
            instructions[counter].regA = atoi(arg0);
            instructions[counter].regB = atoi(arg1);
            if ((instructions[counter].regA < 0 || instructions[counter].regA > 7) 
            || (instructions[counter].regB < 0 || instructions[counter].regB > 7)) {
                exit(1);
            }
        }
        else if (!strcmp(opcode, "halt")) {
            strcpy(instructions[counter].type, "O");
            instructions[counter].opcode = 0b110;
        }
        else if (!strcmp(opcode, "noop")) {
            strcpy(instructions[counter].type, "O");
            instructions[counter].opcode = 0b111;
        }
        else if (!strcmp(opcode, ".fill")) {
            strcpy(instructions[counter].type, "D");
            if ( isNumber(arg0) ) {
                instructions[counter].regA = atoi(arg0);
            } else {
                bool labelExists = false;
                for (int i = 0; i < 1000; i++) {
                    if (!strcmp(labels[i].label, arg0)) {
                        instructions[counter].regA = labels[i].lineNum;
                        labelExists = true;
                        break; // Exit the loop once the label is found
                    }
                }
                if (labelExists == false) {
                    exit(1);
                }
            }
        } else {
            exit(1);
        }
        counter++;
    }

    for (int i = 0; i < counter; i++) {
        int result = 0;

        if (!strcmp(instructions[i].type, "R")) {
            result = (instructions[i].opcode << 22) + (instructions[i].regA << 19)
                + (instructions[i].regB << 16) + (instructions[i].regDest);
        } else if (!strcmp(instructions[i].type, "J")) {
            result = (instructions[i].opcode << 22) + (instructions[i].regA << 19)
                + (instructions[i].regB << 16);
        } else if (!strcmp(instructions[i].type, "I")) {
            if (instructions[i].offset < 0) {
                instructions[i].offset &= 0xFFFF; // Ensure 16-bit offset
            }
            result = (instructions[i].opcode << 22) + (instructions[i].regA << 19)
                + (instructions[i].regB << 16) + (instructions[i].offset);
        } else if (!strcmp(instructions[i].type, "O")) {
            result = (instructions[i].opcode << 22);
        } else {
            result = (instructions[i].regA);
        }

        fprintf(outFilePtr, "%d\n", result);
    }
    exit(0);
}

/*
* NOTE: The code defined below is not to be modifed as it is implimented correctly.
*/

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int
readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
    char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
	/* reached end of file */
        return(0);
    }

    /* check for line too long */
    if (strlen(line) == MAXLINELENGTH-1) {
	printf("error: line too long\n");
	exit(1);
    }

    // Treat a blank line as end of file.
    // Arguably, we could just ignore and continue, but that could
    // get messy in terms of label line numbers etc.
    char whitespace[4] = {'\t', '\n', '\r', ' '};
    int nonempty_line = 0;
    for(size_t line_idx = 0; line_idx < strlen(line); ++line_idx) {
        int line_char_is_whitespace = 0;
        for(int whitespace_idx = 0; whitespace_idx < 4; ++ whitespace_idx) {
            if(line[line_idx] == whitespace[whitespace_idx]) {
                ++line_char_is_whitespace;
                break;
            }
        }
        if(!line_char_is_whitespace) {
            ++nonempty_line;
            break;
        }
    }
    if(nonempty_line == 0) {
        return 0;
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
	/* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);

    return(1);
}

static inline int
isNumber(char *string)
{
    int num;
    char c;
    return((sscanf(string, "%d%c",&num, &c)) == 1);
}



