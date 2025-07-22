#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAPE_L 30000
#define TAPE_MIN -1


//Definition of the tape and the stack

typedef struct{
    unsigned char content[TAPE_L];
    int pos;

}tape;

typedef struct{
    int content[TAPE_L];
    int top;
}stack;

//Operations related to the stack

int push(stack *s, int n){ //Adds n to the top of the stack
    if(s->top < TAPE_L){
        s->top++;
        s->content[s->top] = n;
        return 1;
    }
    return 0;
}

int pop(stack *s){ //Removes the element from the top of the stack
    if(s->top > TAPE_MIN){
        s->top--;
        return 1;
    }
    return 0;
}

int top(stack *s){ //Returns the element on top of the stack
    return s->content[s->top];
}

int is_empty(stack *s){ //Checks if the stack is empty
    if(s->top == TAPE_MIN) return 1;
    return 0;
}

//Operations related specifically to brainfuck

int pointer_increment(tape *t){ //Increments the pointer
    if(t->pos == TAPE_L) return 0;

    t->pos++;
    return 1;
}

int pointer_decrement(tape *t){ //Decrements the pointer
    if(t->pos == 0) return 0;
    
    t->pos--;
    return 1;
}

void data_increment(tape *t){ //Increments the value at the current cell
    if(t->content[t->pos] < 255) t->content[t->pos]++;
    else t->content[t->pos] = 0;
}

void data_decrement(tape *t){ //Decrements the value at the current cell
    if(t->content[t->pos] > 0) t->content[t->pos]--;
    else t->content[t->pos] = 255;
}

void output(tape *t){ //Prints the output of the current cell as a character
    printf("%c", t->content[t->pos]);
}

void input(tape *t){ //Takes a character as an input from the user
    char c = getchar();
    getchar(); //This getchar is encessary to avoid taking \n character in input
    t->content[t->pos] = c;
}

void debug(tape *t){ //Prints 5 elements of the tape, in normal circumstances the current element is in the middle
    int beginning, end;

    switch (t->pos)
    {
    case 0:
        beginning = t->pos;
        end = t->pos + 4;
        break;
    case 1:
        beginning = 0;
        end = t->pos + 3;
        break;
    case TAPE_L - 1:
        beginning = t->pos - 4;
        end = t->pos;
        break;
    case TAPE_L - 2:
        beginning = t->pos - 3;
        end = t->pos + 1;
        break;
    default:
        beginning = t->pos - 2;
        end = t->pos + 2;
        break;
    }

    printf("\n|"); //Prints the index of the tape
    for(int i = beginning; i < end; i++)
        printf("%3d|", i);
    printf("\n ");

    for(int i = beginning; i < end; i++) //Prints the separators
        printf("--- ");
    printf("\n|");

    for(int i = beginning; i < end; i++) //Prints the values of the single tape's element
        printf("%3d|", t->content[i]);
    printf("\n");


    printf(" ");
    for(int i = beginning; i < end; i++){ //Prints the arrow that indicates where the tape's cursor is positioned
        if(i == t->pos){
            printf("  ^ ", t->content[i]);
            break;
        }
        printf("   ");
    }

    printf("\n");
}

int jump_forward(char string[], int start){ //Returns the position of the closing ], starting by the given position
    int counter = 0;
    int l = strlen(string);

    for(int i = start + 1; i < l; i++){
        if(string[i] == '[') counter++;
        else if(string[i] == ']') counter--;
        if(counter == 0) return i;
    }
}


void execute(tape *t, char string[]){ //Calls the functions related to their operator
    stack s;
    s.top = TAPE_MIN;

    int l = strlen(string);

    for(int i = 0; i < l; i++){
        
        switch(string[i]){
            case '>':
                pointer_increment(t);
                break;
            case '<':
                pointer_decrement(t);
                break;
            case '+':
                data_increment(t);
                break;
            case '-':
                data_decrement(t);
                break;
            case '.':
                output(t);
                break;
            case ',':
                input(t);
                break;
            case ':':
                debug(t);
                break;
            case '[':
                if(t->content[t->pos] != 0) //If the element at position pos is != 0, it pushes the current index to the top of the stack
                    push(&s, i);
                else
                    i = jump_forward(string, i); //Else it jumps to ]
                break;
            case ']':
                if(t->content[t->pos] != 0) //If the element at position pos is not 0, it goes back to the beginning of the last cycle
                    i = top(&s);
                else //If not it deletes the starting index of the last cycle
                    pop(&s);
                    
                break;
            default:
                continue;
        }
    }
}



void initialize(tape *t){ //Initializes the content of the tape by setting every element to 0 and setting the start position
    for(int i = 0; i < TAPE_L; i++)
        t->content[i] = 0;
    t->pos = 0;
}

int valid(FILE *fp){ //Checks if the square brackets are used correctly
    char temp;
    int row = 1;
    
    stack s;
    s.top = TAPE_MIN;

    while((temp = getc(fp)) != EOF){ //The parentheses around temp = getc(fp) are necessary to assign the correct value to temp
        if(temp == '\n') row++;
        if(temp == '['){
            push(&s, row);
        }
        if(temp == ']'){
            if(!pop(&s)){
                printf("The ] at line %d is missing its counter part\n", row);
                return 0;
            }
        }
    }

    if(is_empty(&s)) return 1;
    else{
        for(int i = 0; i <= s.top; i++){
            printf("The [ at line %d is missing its counter part\n", s.content[i]);
        }
    }
    return 0;

}

char* load_file(FILE *fp){
    fseek(fp, 0, SEEK_END); //Moves the current file stream's position to the end of the file
    int length = ftell(fp); //Returns the current position of the file stream. In this case it's used to get the length of the file
    rewind(fp);

    char *code = malloc(sizeof(char) * (length + 1)); /*Creates the string that will contain the code. The formula
                                                        in the malloc allocates the space for length + 1 characters*/

    char c;
    int i = 0;
    while( (c = getc(fp)) != EOF){
        if(c == '>' || c == '<' || c == '+' || c == '-' || c == '.' || c == ',' || c == ':' || c == '[' || c == ']')
            code[i++] = c;
    }
    code[i] = '\0';

    return code;
}

int main(int argc, char *argv[]){
    FILE *fp;
    tape t;
    t.pos = TAPE_MIN;

    if(argc == 1){
        printf("ERROR 1: No file given\n");
        return 1;
    }

    if(argc != 2){
        printf("ERROR 2: Too many arguments\n");
        return 2;
    }

    int l = strlen(argv[1]);

    if(l < 4 || !(argv[1][l - 1] == 'f' && argv[1][l - 2] == 'b' && argv[1][l - 3] == '.')){ //Checks for the correct filename length and its format
        printf("ERROR 3: Incorrect file extension\n");
        return 4;
    }

    fp = fopen(argv[1], "r");

    if(fp == NULL){
        printf("ERROR 4: File given doesn't exist\n");
        return 2;
    }

    if(!valid(fp)){
        printf("ERROR 5: The code presents some syntax errors\n");
        return 3;
    }
    rewind(fp);
    char *s = load_file(fp); //Stores the code
    fclose(fp);

    if(s[0] == '\0'){
        printf("ERROR 6: The given file doesn't contain code\n");
        return 7;
    }

    initialize(&t);
    execute(&t, s);//Tell to execute the code from the beginning

    free(s);

    return 0;
}