//  Lexical Analyzer
//
//  Created by:
//  Alex Chatham
//  Jesse Spencer, je017812

// BANCH: Efficiency
// This branch is to improve performance and memory usage

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// Constants
#define TRUE 1
#define FALSE 0

static const int CODE_BUFFER = 10000;
static const int TOKEN_ARRAY_BUFFER = 1000;

static const char INPUT_FILE_NAME[] = "input.txt";

static const int MAX_LEXEME_LENGTH = 11;


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

// Token struct
struct token_t{
    int class;
    char lexeme[MAX_LEXEME_LENGTH];
};


// Global variables
//

// Used to track the final size of the token array
int numberOfTokens = 0;
int rawCodeLength = 0;

// Code array to read to, and token array
char rawCode[CODE_BUFFER] = "";
char cleanCode[CODE_BUFFER] = "";
struct token_t tokenArray[TOKEN_ARRAY_BUFFER];


// Prototypes

// Loads input file and stores the information as characters in the rawCode array
void loadProgramFromFile();
void outputClean();
void analyzeCode();
void printCode();
void outputTable();
void outputList();



int main() {
    
    
    loadProgramFromFile();
    
    outputClean();
    
    analyzeCode();
    
    
    return 0;
}


void loadProgramFromFile() {
    
    FILE* programFile = fopen(INPUT_FILE_NAME, "r");
    
    // Check to see the file has loaded
    if (programFile) {
        
        // Reads in information from file
        char currentCharacter = ' ';     // <--- holds current character
        int i = 0;      //  <--- Counter
        
        //   This while loop reads in raw code from a text file and adds it into the rawCode array, it ends when
        //    it reaches the end of the file
        while ( fscanf(programFile, "%c", &currentCharacter) != EOF) {
            rawCode[i] = currentCharacter;
            i++;
        }
        rawCodeLength = i;
        
    } else {
        printf("Input file not found");
        exit(1);
    }
    
}


// Outputs text file with comments removed and transfers cleaned array to cleanCode
void outputClean() {
    
    // Create output file
    FILE* output = fopen("cleaninput.txt", "w");
    
    char currentChar = ' ';
    int rawIndex = 0;
    int cleanIndex = 0;
    
    
    while (rawIndex < rawCodeLength) {
        
        currentChar = rawCode[rawIndex];
        
        switch (currentChar) {
                
                // Case for when the forward slash is encountered
            case '/':
                
                // Check to see if the next charcter is the *, with no whitespace in between, which will mean a comment has been found
                if (rawCode[rawIndex + 1] == '*') {
                    
                    // A comment was found, now we need to find where it stops and change i so that the comment isn't transferred to the cleaned array
                    // Set i to the first character of the comment
                    rawIndex += 2;
                    
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


// The bulk of the input scanning happens here, using a switch statement
void analyzeCode(){
    
    // Initialize
    int last_index = 0;
    int index = 0;
    int tokenIndex = 0;
    int i;
    
    struct token_t token;
    
    token.class = -1;
    for (i = 0; i < 12; i++) {
        token.lexeme[i] = '\0';
    }
    
    
    // Loop to read in code
    while (index < strlen(cleanCode)) {
        
        index = last_index;
        
        // Getting code from index
        char ch = cleanCode[index];
        
        // This allows letters to follow commas
        if (cleanCode[index - 1] == ',' && ch != ' ' && ch != '\n' && ch != '\t') {
            tokenArray[tokenIndex]=token;
            tokenIndex++;
            for(i=0;i<12;i++){
                token.lexeme[i] = '\0';
            }
        }
        
        //this allows for ':=' right after identifiers
        if (ch==':'&&cleanCode[index-1]!=' ') {
            tokenArray[tokenIndex]=token;
            tokenIndex++;
            for(i=0;i<12;i++){
                token.lexeme[i] = '\0';
            }
        }
        
        if (cleanCode[index-1]=='/'||cleanCode[index-1]=='+'||cleanCode[index-1]=='-'||cleanCode[index-1]=='='||cleanCode[index-1]=='*') {
            if (ch!=' ') {
                tokenArray[tokenIndex]=token;
                tokenIndex++;
                for(i=0;i<12;i++){
                    token.lexeme[i] = '\0';
                }
            }
        }
       
        //switch function to create tokens from code array
        switch(ch) {
                
                // if ch is == i, then it could be an if statement or a variable
            case 'i':
                
                
                if (token.lexeme[0]=='i') {
                    token.class=identsym;
                }
                token.lexeme[strlen(token.lexeme)]=ch;
                if (cleanCode[index + 1] == 'f') {
                    token.lexeme[strlen(token.lexeme)]=cleanCode[index + 1];
                    token.class = ifsym;
                    last_index = index + 2;
                    break;
                    
                }
                last_index = index + 1;
                
                break;
                // if ch is == e then it could could be the end or else statement
            case 'e':
                token.lexeme[strlen(token.lexeme)]=ch;
                if(token.lexeme[3]=='\0'){
                    token.class = identsym;
                }
                if (cleanCode[index+1]=='n'&&token.lexeme[0]!='t') {
                    token.class = endsym;
                }
                if (cleanCode[index+1]=='l'){
                    token.class = elsesym;
                }
                last_index = index +1;
                break;
            case 'v':
                token.class = identsym;
                token.lexeme[strlen(token.lexeme)]=ch;
                if (cleanCode[index+1]=='a') {
                    token.class = varsym;
                }
                last_index = index+1;
                break;
                // begin
            case 'b':
                token.class = identsym;
                token.lexeme[strlen(token.lexeme)]=ch;
                last_index = index +1;
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
                if (cleanCode[index+1]=='h'){
                    token.class= whilesym;
                }
                last_index = index+1;
                break;
                
            case 'c':
                token.lexeme[strlen(token.lexeme)]=ch;
                if (cleanCode[index+1]== 'a') {
                    token.class=callsym;
                    last_index = index+1;
                    break;
                }
                
                if (cleanCode[index+1]=='o') {
                    token.class=constsym;
                    last_index = index+1;
                    break;
                }
                last_index = index+1;
                break;
                //if there is a space new line or tab, we know that the last token has ended and the new token begins after this character
                // Whitespace case
            case ' ' :
            case '\n':
            case '\t':
                if (cleanCode[index-1]==' '||cleanCode[index-1]=='\n') {
                    last_index = index +1;
                    break;
                    
                }
                tokenArray[tokenIndex]=token;
                tokenIndex++;
                for(i=0;i<12;i++){
                token.lexeme[i] = '\0';
                }
                
                last_index = index+1;
                break;
                //case where the character is either a comment or division
            case '/':
                if (cleanCode[index-1]!=' ') {
                    tokenArray[tokenIndex]=token;
                    tokenIndex++;
                    for(i=0;i<12;i++){
                        token.lexeme[i] = '\0';
                    }
                }
                token.class = slashsym;
                token.lexeme[strlen(token.lexeme)]=ch;
                last_index = index+1;
                break;
            case '+':
                if (cleanCode[index-1]!=' ') {
                    tokenArray[tokenIndex]=token;
                    tokenIndex++;
                    for(i=0;i<12;i++){
                        token.lexeme[i] = '\0';
                    }
                }
                token.class = plussym;
                token.lexeme[strlen(token.lexeme)]=ch;
                last_index = index+1;
                break;
            case 'm':
                token.class = identsym;
                if(cleanCode[index+1]=='u'){
                    token.class = multsym;
                }
                token.lexeme[strlen(token.lexeme)]=ch;
                last_index = index + 1;
                break;
            case '-':
                if (cleanCode[index-1]!=' ') {
                    tokenArray[tokenIndex]=token;
                    tokenIndex++;
                    for(i=0;i<12;i++){
                        token.lexeme[i] = '\0';
                    }
                }
                token.class = minussym;
                token.lexeme[strlen(token.lexeme)]=ch;
                last_index = index+1;
                break;
            case '*':
                if (cleanCode[index-1]!=' ') {
                    tokenArray[tokenIndex]=token;
                    tokenIndex++;
                    for(i=0;i<12;i++){
                        token.lexeme[i] = '\0';
                    }
                }
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
                for(i=0;i<12;i++){
                    token.lexeme[i] = '\0';
                }
                tokenIndex++;
                token=tokenArray[tokenIndex];
                token.class = commasym;
                token.lexeme[strlen(token.lexeme)]=ch;
                last_index = index+1;
                break;
            case '.':
                //there is not a space before periods, periods also end the file
                tokenArray[tokenIndex]=token;
                for(i=0;i<12;i++){
                    token.lexeme[i] = '\0';
                }
                tokenIndex++;
                token.class = periodsym;
                token.lexeme[strlen(token.lexeme)]=ch;
                tokenArray[tokenIndex]=token;
                for(i=0;i<12;i++){
                    token.lexeme[i] = '\0';
                }
                tokenIndex++;
                last_index = index+1;
                break;
            case ';':
                tokenArray[tokenIndex]=token;
                for(i=0;i<12;i++){
                    token.lexeme[i] = '\0';
                }
                tokenIndex++;
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
                token.lexeme[strlen(token.lexeme)] = ch;
                if (cleanCode[index + 1] == 'd') {
                    token.lexeme[strlen(token.lexeme)] = cleanCode[index+1];
                    if (cleanCode[index + 1] == 'd') {
                        token.lexeme[strlen(token.lexeme)] = cleanCode[index+2];
                        token.class = oddsym;
                        last_index = index + 3;
                        break;
                    }
                }
                last_index = index + 1;
                break;
            case '=':
                token.lexeme[strlen(token.lexeme)] = ch;
                if (cleanCode[index-1]==' ') {
                    token.class = eqlsym;
                }
                last_index = index + 1;
                break;
                // read
            case 'r':
                token.lexeme[strlen(token.lexeme)] = ch;
                if (cleanCode[index-1]==' '||cleanCode[index-1]==',') {
                    token.class = identsym;
                }
                if (cleanCode[index + 1] == 'e'&& cleanCode[index+2]=='a') {
                    token.lexeme[strlen(token.lexeme)] = cleanCode[index+1];
                    if (cleanCode[index + 2] == 'a') {
                        token.lexeme[strlen(token.lexeme)] = cleanCode[index+2];
                        if (cleanCode[index + 3] == 'd') {
                            token.lexeme[strlen(token.lexeme)] = cleanCode[index+3];
                            token.class = readsym;
                            last_index = index + 4;
                            break;
                        }
                    }
                }
                last_index = index + 1;
                break;
            case '>':
                token.lexeme[strlen(token.lexeme)] = ch;
                token.class = gtrsym;
                if (cleanCode[index+1]=='=') {
                    token.class = geqsym;
                }
                last_index = index +1;
                break;
            case '<':
                token.lexeme[strlen(token.lexeme)] = ch;
                token.class = lessym;
                if (cleanCode[index+1]== '=') {
                    token.class = leqsym;
                }
                last_index = index +1;
                break;
            case 'n':
                token.lexeme[strlen(token.lexeme)] = ch;
                if (token.lexeme[0]=='n') {
                token.class = identsym;
                }
                if (cleanCode[index + 1]== 'u'){
                    token.class = nulsym;
                }
                if (token.lexeme[0]=='t') {
                    token.class = thensym;
                }
                last_index = index+1;
                break;
                //A: cases left  nulsym, numbersym. I think that is all
                
                // J: Moved to default case for now, may be a better way so that default can be used for errors
                //if ch == an alpha or digit that is not stated above then it just reads in the character
            default:
                //if(isalpha(ch)||isdigit(ch)){
                if(token.class == 0 ){
                    token.class = identsym;
                }
                if(isdigit(ch)==TRUE){
                    token.class = numbersym;
                }
                if (isalpha(ch)==TRUE &&isalpha(cleanCode[index-1])==FALSE) {
                    token.class = identsym;
                }
                if (token.lexeme[0]=='c' && token.lexeme[1]=='o') {
                    token.class = constsym;
                }
                if (token.lexeme[0]=='b'&& token.lexeme[1]=='e') {
                    token.class = beginsym;
                }
                
                token.lexeme[strlen(token.lexeme)]=ch;
                last_index = index+1;
                break;
                //}
                
                
        }
        
         numberOfTokens = tokenIndex;
    }
    
    printCode();
    
}

void printCode() {
    
    outputTable();
    outputList();
    
}

void outputTable() {
    
    FILE* output = fopen("lexemetable.txt", "w");
    
    // Header
    fprintf(output, "Lexeme\t\tToken Type\n");
    
    for (int i = 0; i < numberOfTokens; i++) {
       fprintf(output, "%s", tokenArray[i].lexeme);
        fprintf(output, "\t");
        if (strlen(tokenArray[i].lexeme) < 6){
            fprintf(output, "\t");
        }
        fprintf(output, "%d\n", tokenArray[i].class);
    }
    
}

void outputList() {
    
    FILE* output = fopen("lexemelist.txt", "w");
    
    
    for (int i = 0; i < numberOfTokens; i++) {
        
        if (tokenArray[i].class == identsym) {
            fprintf(output, "%d ", tokenArray[i].class);
            fprintf(output, "%s ", tokenArray[i].lexeme);
        }
        
        else
            fprintf(output, "%d ", tokenArray[i].class);
    }
    
    
}


