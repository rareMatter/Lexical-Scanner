//  Lexical Analyzer
//
//  Created by:
//  Alex Chatham
//  Jesse Spencer, je017812



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
    
    // Create output file
    FILE* output = fopen("cleaninput.txt", "w");
    
    char currentChar;
    int rawIndex = 0;
    int cleanIndex = 0;
    
    
    while (rawIndex < strlen(rawCode)) {
        
        currentChar = rawCode[rawIndex];
        
        switch (currentChar) {
            
            // Case for when the forward slash is encountered
            case '/':
                
                // Check to see if the next charcter is the *, with no whitespace in between, which will mean a comment has been found
                if (rawCode[rawIndex + 1] == '*') {
                    
                    // A comment was found, now we need to find where it stops and change i so that the comment isn't transferred to the cleaned array
                    // Set i to the first character of the comment
                    rawIndex += 2;
                    currentChar = rawCode[rawIndex];
                    // Use a nested switch statement, just like the enclosing one except for finding the end comment notation
                    
                    int commentEndFound = FALSE;
                    
                    while (commentEndFound == FALSE) {
                        
                        currentChar = rawCode[rawIndex];
                        
                        switch (currentChar) {
                            
                            case '*':
                                
                                if (rawCode[rawIndex + 1] == '/') {
                                    commentEndFound = TRUE;
                                    rawIndex += 2;
                                    break;
                                }
                                
                                rawIndex++;
                                break;
                                
                            default:
                                rawIndex++;
                                break;
                        }
                    }
                }
                
                cleanCode[cleanIndex] = rawCode[rawIndex];
                fprintf(output, "%c", cleanCode[cleanIndex]);
                cleanIndex++;
                rawIndex++;
                break;
            
            // The default case will simply transfer the characters to the cleaned array
            default:
                cleanCode[cleanIndex] = rawCode[rawIndex];
                fprintf(output, "%c", cleanCode[cleanIndex]);
                cleanIndex++;
                rawIndex++;
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
    while (index<strlen(cleanCode)) {
        index=last_index;
        //getting code from index
        char ch = cleanCode[index];
        
        //switch function to create tokens from code array
        switch(ch) {
                
                // if ch is == i, then it could be an if statement or a variable
            case 'i':
                
                state=2;
                token.class=identsym;
                token.lexeme[strlen(token.lexeme)]=ch;
                last_index = index+1;
                
                break;
                
                //if ch is == f then it is an if statement "if" is the only operation with an f
            case 'f':
                state=3;
                token.class = ifsym;
                token.lexeme[strlen(token.lexeme)]=ch;
                last_index = index+1;
                break;
                
                // if ch is == e then it could could be the end statement
            case 'e':
                token.lexeme[strlen(token.lexeme)]=ch;
                if (cleanCode[index+1]=='n') {
                    token.class = endsym;
                }
                else if (cleanCode[index+1]=='l'){
                    token.class = elsesym;
                }
                break;
                
            case 'v':
                token.lexeme[strlen(token.lexeme)]=ch;
                if (cleanCode[index+1]=='a') {
                    token.class = varsym;
                }
                last_index = index+1;
                break;
            // begin
            case 'b':
                if (cleanCode[index + 1] == 'e') {
                    if (cleanCode[index + 2] == 'g') {
                        if (cleanCode[index + 3] == 'i') {
                            if (cleanCode[index + 4] == 'n') {
                                token.class = beginsym;
                                last_index = index + 5;
                            }
                        }
                    }
                }
                break;
            case 'p':
                token.lexeme[strlen(token.lexeme)]=ch;
                if (cleanCode[index+1]=='r') {
                    token.class = procsym;
                }
                last_index = index+1;
                break;
                
            case 'w':
                token.lexeme[strlen(token.lexeme)]=ch;
                if (cleanCode[index+1]== 'r') {
                    token.class= writesym;
                }
                else if (cleanCode[index+1]=='h'){
                    token.class= whilesym;
                }
                last_index = index+1;
                break;
                
            case 'c':
                if (cleanCode[index+1]== 'a') {
                    token.class=callsym;
                }
                last_index = index+1;
                break;
                //if there is a space new line or tab, we know that the last token has ended and the new token begins after this character
                // Whitespace case
            case ' ' :
            case '\n':
            case '\t':
                tokenArray[tokenIndex]=token;
                tokenIndex++;
                token=tokenArray[tokenIndex];
                last_index = index+1;
                break;
                //case where the character is either a comment or division
            case '/':
                token.class = slashsym;
                token.lexeme[strlen(token.lexeme)]=ch;
                last_index = index+1;
                break;
            case '+':
                token.class = plussym;
                token.lexeme[strlen(token.lexeme)]=ch;
                last_index = index+1;
                break;
            case '-':
                token.class = minussym;
                token.lexeme[strlen(token.lexeme)]=ch;
                last_index = index+1;
                break;
            case '*':
                token.class = multsym;
                token.lexeme[strlen(token.lexeme)]=ch;
                last_index = index+1;
                break;
            case '(':
                token.class = lparentsym;
                token.lexeme[strlen(token.lexeme)]=ch;
                last_index = index+1;
                break;
            case ')':
                token.class = rparentsym;
                token.lexeme[strlen(token.lexeme)]=ch;
                last_index = index+1;
                break;
            case ':':
                token.class = becomessym;
                token.lexeme[strlen(token.lexeme)]=ch;
                //the next input should be an = sign
                token.lexeme[strlen(token.lexeme)]=cleanCode[index+1];
                //increment the index by two to incorperate the : and the =
                last_index = index + 2;
                break;
            case ',':
                //since there is not a space before commas we need to end the last token
                tokenArray[tokenIndex]=token;
                tokenIndex++;
                token=tokenArray[tokenIndex];
                token.class = commasym;
                token.lexeme[strlen(token.lexeme)]=ch;
                last_index = index+1;
                break;
            case '.':
                //there is not a space before periods
                tokenArray[tokenIndex]=token;
                tokenIndex++;
                token=tokenArray[tokenIndex];
                token.class = periodsym;
                token.lexeme[strlen(token.lexeme)]=ch;
                break;
            case ';':
                tokenArray[tokenIndex]=token;
                tokenIndex++;
                token=tokenArray[tokenIndex];
                token.class = semicolonsym;
                token.lexeme[strlen(token.lexeme)]=ch;
                last_index = index+1;
                break;
            case '#':
                token.class = neqsym;
                token.lexeme[strlen(token.lexeme)] = ch;
                last_index = index + 1;
                break;
            case '%':
                token.class = geqsym;
                token.lexeme[strlen(token.lexeme)] = ch;
                last_index = index + 1;
                break;
            // oddsym
            case 'o':
                if (cleanCode[index + 1] == 'd') {
                    if (cleanCode[index + 1] == 'd') {
                        token.class = oddsym;
                        last_index = index + 3;
                    }
                }
                break;
            case '=':
                break;
            // read
            case 'r':
                break;
            
                //A: cases left readsym, nulsym, numbersym. I think that is all
                
                // J: Moved to default case for now, may be a better way so that default can be used for errors
                //if ch == an alpha or digit that is not stated above then it just reads in the characters
            default:
                //if(isalpha(ch)||isdigit(ch)){
                state=4;
                token.lexeme[strlen(token.lexeme)]=ch;
                last_index = index+1;
                break;
                //}
                
                
        }
    }

}

void processcode(){
    
}


