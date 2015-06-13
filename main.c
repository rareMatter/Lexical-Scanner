//  Lexical
//
//  Created by Alex Chatham on 6/10/15.



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


//Identify all lexical conventions
typedef enum {
    nulsym = 1,
    identsym = 2,
    numbersym = 3,
    plussym = 4,
    minussym = 5,
    multsym = 6,
    slashsym = 7,
    oddsym = 8,
    eqlsym = 9,
    neqsym = 10,
    lessym = 11,
    leqsym = 12,
    gtrsym = 13,
    geqsym = 14,
    lparentsym = 15,
    rparentsym = 16,
    commasym = 17,
    semicolonsym = 18,
    periodsym = 19,
    becomessym = 20,
    beginsym = 21,
    endsym = 22,
    ifsym = 23,
    thensym = 24,
    whilesym = 25,
    dosym = 26,
    callsym = 27,
    constsym = 28,
    varsym = 29,
    procsym = 30,
    writesym = 31,
    readsym = 32,
    elsesym = 33
} token_type;

// token struct
struct token_t{
    int class;
    char lexeme[11];
};


#define TRUE 1
#define FALSE 0


static const int CODE_BUFFER = 10000;


//code array to read to, and token array
char rawCode[CODE_BUFFER] = "";
char cleanCode[CODE_BUFFER] = "";
struct token_t tokenArray[100];


//functions
void loadProgramFromFile();
void outputClean();
void analyzeCode();
void printCode();


// this should be about how the code is executed
int main() {
    
    
    loadProgramFromFile();
    
    outputClean();
        
    //analyzeCode();
    
    //printCode();
    
    return 0;
    
}

//should work and read in file
void loadProgramFromFile() {
    
    FILE* programFile = fopen("input.txt", "r");
    
    //check to see the file has loaded
    if (programFile == NULL) {
        printf("uhoh ");
        exit(1);
    }
    
    else {        // If file exists
        
        // Reads in information from file and stores in CODE structure array
        char currentCharacter;     // <--- holds code
        int i = 0;      //  <--- Counter
        //   This while loop reads in raw code from a text file and adds it onto the code array, it ends when
        //    it reaches the end of the file
        while (fscanf(programFile, "%c", &currentCharacter) != EOF) {
            rawCode[i] = currentCharacter;
            i++;
        }
    }
}


// Outputs text file with comments removed and transfers cleaned array to cleanCode
void outputClean() {
    
    
    char currentChar = ' ';
    int i = 0;
    
    // The minus one is needed here because the string terminating character was being inserted into the cleanCode array and causing issues
    while (i < strlen(rawCode) - 1) {
        
        currentChar = rawCode[i];
        
        switch (currentChar) {
            
            // Case for when the forward slash is encountered
            case '/':
                
                // Check to see if the next charcter is the *, with no whitespace in between, which will mean a comment has been found
                if (rawCode[i + 1] == '*') {
                    
                    // A comment was found, now we need to find where it stops and change i so that the comment isn't transferred to the cleaned array
                    // Set i to the first character of the comment
                    i += 2;
                    currentChar = rawCode[i];
                    // Use a nested switch statement, just like the enclosing one except for finding the end comment notation
                    
                    int commentEndFound = FALSE;
                    
                    while (commentEndFound == FALSE) {
                        
                        currentChar = rawCode[i];
                        
                        switch (currentChar) {
                            
                            case '*':
                                
                                if (rawCode[i + 1] == '/') {
                                    commentEndFound = TRUE;
                                    i += 2;
                                    break;
                                }
                                
                                i++;
                                break;
                                
                            default:
                                i++;
                                break;
                        }
                    }
                }
                
                cleanCode[i] = rawCode[i];
                i++;
                break;
            
            // The default case will simply transfer the characters to the cleaned array
            default:
                cleanCode[i] = rawCode[i];
                i++;
                break;
        }
    }
    
    
}


//need to add more cases
void analyzeCode(){
    
    //initialize
    int last_index = 0;
    struct token_t token;
    int index = 0;
    int state = 1;
    int tokenIndex = 0;
    
    //loop to read in code
    while (index < strlen(rawCode)) {
        
        //getting code from index
        char ch = rawCode[index];
        
        //switch function to create tokens from code array
        switch(ch) {
                
            // if ch is == i, then it could be an if statement or a variable
            case 'i':
                
                state = 2;
                token.class = identsym;
                token.lexeme[strlen(token.lexeme)] = ch;
                last_index = index + 1;
                
                break;
                
            //if ch is == f then it is an if statement "if" is the only operation with an f
            case 'f':
                state=3;
                token.class = ifsym;
                token.lexeme[strlen(token.lexeme)]=ch;
                
                break;
                
            //if there is a space new line or tab, we know that the last token has ended and the new token begins after this character
            // Whitespace case
            case ' ' | '\n' | '\t':
                
                tokenArray[tokenIndex]=token;
                
                break;
                
            // J: Moved to default case for now, may be a better way so that default can be used for errors
            //if ch == an alpha or digit that is not stated above then it just reads in the characters
            default:
                //if(isalpha(ch)||isdigit(ch)){
                    state=4;
                    //token.lexeme[strlen(token.lexeme)] = ch;
                    last_index = index+1;
                    break;
                //}
                
                
        }
    }
}

void processcode(){
    
}


