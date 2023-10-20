#include <stdio.h>
#include <assert.h>
#include <math.h>

enum {
    NO = 0,
    RED = 1,
    BLUE = 2
};

typedef int board_t[15];
typedef int player_t;

int has_lost(board_t board,player_t player){
    for(int i = 1;i < 6;i++){
        for(int j = i+1;j < 6;j++){
            if(board[(i*(11-i))/2 + j - 7] == player){
                for(int k = j+1;k <= 6;k++){
                    if(board[(i*(11-i))/2 + k - 7] == player && board[(j*(11-j))/2 + k - 7] == player){
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

int has_won(board_t board,player_t player){
    return has_lost(board,player^1);
}

int is_full(board_t board){
    for(int i = 0;i < 15;i++){
        if(board[i] == NO){
            return 0;
        }
    }
    return 1;
}

typedef struct{
    int line;
    int score;
} move_t;

const int sp = 14500000; // search space
move_t bm1[14500000],bm2[14500000]; // best moves 1 and 2

void init(board_t board){
    move_t def = {-1,0};
    for(int i = 0;i < sp;i++){
        if(i < 15){
            board[i] = NO;
        }
        bm1[i] = bm2[i] = def;
    }
}

move_t best_move(board_t board,player_t player){
    move_t move = {-1,16};
    move_t o_move = {-1, -17};
    int ind = 0;
    for(int i = 0;i < 15;i++){
        ind += pow(3,i) * board[i];
    }
    if(player == RED && bm1[ind].line != -1){
        return bm1[ind];
    }
    if(player == BLUE && bm2[ind].line != -1){
        return bm2[ind];
    }
    if(has_won(board,player)){
        move_t move = {-1,16};
        return move;
    }
    for(int i = 0;i < 15;i++){
        if(board[i] == NO){
            board[i] = player;
            move_t rep = best_move(board,player^1);
            board[i] = NO;
            move.line = i;
            if(rep.score < 0){
                move.score = -1 - rep.score;
                if(move.score > o_move.score){
                    o_move = move;
                }
            }else{
                move.score = 1 - rep.score;
                if(move.score > o_move.score){
                    o_move = move;
                }
            }
        }
    }
    if(player == RED){
        bm1[ind] = o_move;
    }else{
        bm2[ind] = o_move;
    }
    return o_move;
}

void exec_move(board_t board,move_t move,player_t player){
    board[move.line] = player;
}

void print_board(board_t board){
    for (int i = 0; i < 15; ++i) {
        switch (board[i]) {
        case RED: printf("R "); break;
        case BLUE: printf("B "); break;
        case NO: printf("_ "); break;
        }
    }
    printf("\n");
}

int main(){
    printf("Enter your choice (R/B): ");
    char choi;
    scanf("%c",&choi);
    board_t board;
    player_t comp,user;
    init(board);
    if(choi == 'R'){
        user = RED;
    }else if(choi == 'B'){
        user = BLUE;
    }else{
        return 0;
    }
    player_t curr = 1;
    while(!is_full(board) && !has_won(board,user) && !has_won(board,comp)){
        if(curr == comp){
            exec_move(board,best_move(board,comp),comp);
        }else{
            print_board(board);
            int choice;
            printf("Enter your choice:\n");
            scanf("%d",&choice);
            move_t move = {choice,0};
            exec_move(board,move,user);
        }
        if(has_won(board,comp)){
            print_board(board);
            printf("You lost\n");
        }else if(has_won(board,user)){
            print_board(board);
            printf("You won\n");
        }
        curr ^= 1;
    }
    return 0;
}