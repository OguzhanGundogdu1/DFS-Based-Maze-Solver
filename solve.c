/*
    Maze Solver using stack-based backtracking (DFS)
    ---> A C project exploring Stack data structures and backtracking to navigate complex 11x11 labyrinths.
    Features: ANSI color support, Error Handling, Memory Safety
    Author: Oguzhan Gundogdu
*/

#include <stdio.h>
#include <stdlib.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct{
    int x;
    int y;
}Position;

typedef struct GameState{
    char maze[11][11];
    Position player;
    Position exit;
    struct GameState *next;
}GameState;

typedef struct stack{
    int elemcount;
    GameState *head;
}Stack;

void initStack(Stack *stack){
    stack->head = NULL;
    stack->elemcount = 0;

    return;
}

void save_the_maze(char *filename, GameState *state){
    FILE *file = fopen(filename, "r");
    if(file==NULL){
        printf("The file could not be opened!\n");
        return;
    }

    char current;
    char arr[121];
    int c = 0;
    while( (current = fgetc(file)) != EOF ){
        if((current != '\n') && (c < 121)){
            arr[c] = current;
            c++;
        }
    }
    fclose(file);

    for(int i = 0; i < 11; i++){
        for(int u = 0; u < 11; u++){
            state->maze[i][u] = arr[11 * i + u];
        }
    }

    state->maze[state->player.y][state->player.x] = 'O';
    state->next = NULL;

    return;
}

void push(Stack *stack, GameState *new_state){
    
    GameState *new = (GameState*) malloc(sizeof(GameState));
    for(int i = 0; i < 11; i++){
        for(int u = 0; u < 11; u++){
            new->maze[i][u] = new_state->maze[i][u];
        }
    }

    new->player.x = new_state->player.x;
    new->player.y = new_state->player.y;
    new->exit.x = new_state->exit.x;
    new->exit.y = new_state->exit.y;
    new->next = stack->head;
    stack->head = new;
    stack->elemcount++;

    return;
}

GameState* pop(Stack *stack, GameState *backup){
    
    if(stack->elemcount == 0){
        printf("\nThe stack is empty.\n");
        return NULL;
    }

    for(int i = 0; i < 11; i++){
        for(int u = 0; u < 11; u++){
            backup->maze[i][u] = stack->head->maze[i][u];
        }
    }
    backup->exit.x = stack->head->exit.x;
    backup->exit.y = stack->head->exit.y;
    backup->next = NULL;
    backup->player.x = stack->head->player.x;
    backup->player.y = stack->head->player.y;

    GameState *old = stack->head;
    stack->head = stack->head->next;
    stack->elemcount--;
    free(old);

    return backup;
}

GameState* top(Stack *stack ,GameState *backup){
    
    if(stack->elemcount == 0){
        printf("\nThe stack is empty.\n");
        return NULL;
    }

    for(int i = 0; i < 11; i++){
        for(int u = 0; u < 11; u++){
            backup->maze[i][u] = stack->head->maze[i][u];
        }
    }
    backup->exit = stack->head->exit;
    backup->next = NULL;
    backup->player.x = stack->head->player.x;
    backup->player.y = stack->head->player.y;

    return backup;
}

void print_the_maze(GameState *state){
    printf("\n");
    for(int i = 0; i < 11; i++){
        for(int u = 0; u < 11; u++){
            char c = state->maze[i][u];
            
            if (c == 'X') {
                printf(ANSI_COLOR_BLUE "X" ANSI_COLOR_RESET);
            } else if (c == 'O') {
                printf(ANSI_COLOR_GREEN "O" ANSI_COLOR_RESET);
            } else if(c == 'F') {
                printf(ANSI_COLOR_RED "F" ANSI_COLOR_RESET);
            } else {
                printf("%c", c);
            }
        }
        printf("\n");
    }
    printf("\n");
}

// void print_the_maze(GameState *state){
//     printf("\n");
//     for(int i = 0; i < 11; i++){
//         for(int u = 0; u < 11; u++){
//             printf("%c", state->maze[i][u]);
//         }
//         printf("\n");
//     }
//     printf("\n");
    
//     return;
// }

void free_stack(Stack *stack){
    
    while(stack->head != NULL){
        GameState *current = stack->head;
        stack->head = stack->head->next;
        free(current);
    }

    return;
}

int main(){
    GameState *lab = (GameState*) malloc(sizeof(GameState));
    lab->player.x = 5;
    lab->player.y = 9;
    lab->exit.x = 1;
    lab->exit.y = 0;
    
    save_the_maze("maze_1.txt", lab);
    
    Stack *mystack = (Stack*) malloc(sizeof(Stack));
    initStack(mystack);
    push(mystack, lab);
    free(lab);

    GameState *backup = (GameState*) malloc(sizeof(GameState));
    int control = 0;
    while(mystack->elemcount > 0 && !control){
        GameState *current = pop(mystack, backup);
        print_the_maze(current);
        if( (current->player.x + 1 == current->exit.x) && (current->player.y == current->exit.y) ){
            current->maze[current->player.y][current->player.x] = 'X';
            current->player.x += 1;
            current->maze[current->player.y][current->player.x] = 'O';
            printf("The solution was found.\n");
            print_the_maze(current);
            free_stack(mystack);
            control = 1;
        }else if( (current->player.x - 1 == current->exit.x) && (current->player.y == current->exit.y) ){
            current->maze[current->player.y][current->player.x] = 'X';
            current->player.x -= 1;
            current->maze[current->player.y][current->player.x] = 'O';
            printf("The solution was found.\n");
            print_the_maze(current);
            free_stack(mystack);
            control = 1;
        }else if( (current->player.x == current->exit.x) && (current->player.y + 1 == current->exit.y) ){
            current->maze[current->player.y][current->player.x] = 'X';
            current->player.y += 1;
            current->maze[current->player.y][current->player.x] = 'O';
            printf("The solution was found.\n");
            print_the_maze(current);
            free_stack(mystack);
            control = 1;
        }else if( (current->player.x == current->exit.x) && (current->player.y - 1 == current->exit.y) ){
            current->maze[current->player.y][current->player.x] = 'X';
            current->player.y -= 1;
            current->maze[current->player.y][current->player.x] = 'O';
            printf("The solution was found.\n");
            print_the_maze(current);
            free_stack(mystack);
            control = 1;
        }

        int x = current->player.x;
        int y = current->player.y;
        
        if(current->maze[y][x - 1] == ' '){
            current->maze[y][x] = 'X';
            current->maze[y][x - 1] = 'O';
            current->player.x -= 1;
            push(mystack, current);
            current->maze[y][x - 1] = ' ';
            current->player.x += 1;
            current->maze[y][x] = 'O';
        }
        if(current->maze[y+1][x] == ' '){
            current->maze[y][x] = 'X';
            current->maze[y + 1][x] = 'O';
            current->player.y += 1;
            push(mystack, current);  
            current->maze[y + 1][x] = ' ';
            current->player.y -= 1;
            current->maze[y][x] = 'O';          
        }
        if(current->maze[y - 1][x] == ' '){
            current->maze[y][x] = 'X';
            current->maze[y - 1][x] = 'O';
            current->player.y -= 1;
            push(mystack, current);  
            current->maze[y - 1][x] = ' ';
            current->player.y += 1;
            current->maze[y][x] = 'O';          
        }
        if(current->maze[y][x + 1] == ' '){
            current->maze[y][x] = 'X';
            current->maze[y][x + 1] = 'O';
            current->player.x += 1;
            push(mystack, current);
            current->maze[y][x + 1] = ' ';
            current->player.x -= 1;
            current->maze[y][x] = 'O';
        }
    }

    if (!control) {
        printf("There is no exit in this maze!\n");
    }

    free_stack(mystack);
    free(mystack);
    free(backup);
    
    return 0;
}
