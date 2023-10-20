#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "math.h"

/* Implement connect 4 on a 5 (columns) x 4 (rows) board. */
enum
{
    EMPTY = 0,
    RED = 1,
    BLUE = 2,
};

typedef char board_t[4][5];
typedef char player_t;
typedef struct
{
    int col;
    int score;
} move_t;

bool has_won(board_t board, player_t player)
{
    for (int i = 0; i < 4; i++)
    {
        int row_win = 1; 
        for (int j = 0; j < 4; j++)
        {
            if (board[i][j] != player)
            {
                row_win = 0; 
                break;
            }
        }
        if (row_win)
            return 1;
    }

    for (int j = 0; j < 4; j++)
    {
        int col_win = 1; 
        for (int i = 0; i < 4; i++)
        {
            if (board[i][j] != player)
            {
                col_win = 0; 
                break;
            }
        }
        if (col_win)
            return 1;
    }

    int d2_win = 1; 
    for (int i = 0; i < 4; i++)
    {
        if (board[i][i] != player)
        {
            d2_win = 0; 
            break;
        }
    }
    if (d2_win)
        return 1;

    int d3_win = 1;
    for (int i = 0; i < 4; i++)
    {
        if (board[i][3 - i] != player)
        {
            d3_win = 0;
            break;
        }
    }
    if (d3_win)
        return 1;

    return 0; 
}

bool is_full(board_t board)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 5; j++)
            if (board[i][j] == EMPTY)
                return 0;
    return 1;
}

player_t other_player(player_t player)
{
    if (player == RED)
    {
        return BLUE;
    }
    else if (player == BLUE)
    {
        return RED;
    }
    else
    {
        assert(0);
    }
}

#define SEARCH_SPACE 33554432

move_t best_moves[SEARCH_SPACE]; 

void init_game(board_t board)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 5; j++)
            board[i][j] = EMPTY;
    int j = 0;
    while (j < SEARCH_SPACE) {
        move_t m = {-2, -1};
        best_moves[j] = m;
        j++;
    }
}

move_t best_move(board_t board, player_t player) {
    int fi = 0, si = 0;
    for (int i = 0; i < 5; i++) {
        int f = 0, bm = 0;
        for (int j = 3; j >= 0; j--) {
            if (board[j][i] != EMPTY) {
                if (!f) {
                    if (board[j][i] == player)
                        fi += (1 << i);
                    f = (int)board[j][i];
                }
                if (f == board[j][i])
                    bm = (bm << 1) + 1;
                else
                    bm <<= 1;
            }
        }
        si += bm * (1 << (4 * i));
    }
    si += fi * (1 << 20);
    
    if (best_moves[si].col != -2) {
        return best_moves[si];
    }

    move_t m, bm = {-1, -21}, draw = {-1, 0}, won = {-1, 21}, lost = {-1, -21}, mm;
    
    if (has_won(board, player)) {
        best_moves[si] = won;
        return won;
    }
    
    if (has_won(board, other_player(player))) {
        best_moves[si] = lost;
        return lost;
    }
    
    if (is_full(board)) {
        best_moves[si] = draw;
        return draw;
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 3; j >= 0; j--) {
            if (board[j][i] == EMPTY) {
                board[j][i] = player;
                m = best_move(board, other_player(player));
                board[j][i] = EMPTY;
                mm.col = i;
                
                if (m.score > 0)
                    mm.score = -m.score + 1;
                else if (m.score < 0)
                    mm.score = -m.score - 1;
                else
                    mm.score = 0;

                if (mm.score > bm.score)
                    bm = mm;
            }
        }
    }
    best_moves[si] = bm;
    return bm;
}

void play_move(board_t board, move_t move, player_t player)
{
    for (int i = 3; i >= 0; i--)
    {
        if (board[i][move.col] == EMPTY)
        {
            board[i][move.col] = player;
            return;
        }
    }
}

void print_board(board_t board)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            switch (board[i][j])
            {
            case RED:
                printf("R ");
                break;
            case BLUE:
                printf("B ");
                break;
            case EMPTY:
                printf(". ");
                break;
            default:
                assert(0);
            }
        }
        printf("\n");
    }
}

int main()
{
    /* Your game play logic. */
    /* The user should have the option to select red or blue player. */
    char c;
    printf("Enter your color (R or B): ");
    scanf("%c", &c);
    board_t board;
    player_t comp, user, curr = RED;
    init_game(board);
    if (c == 'R')
    {
        comp = BLUE;
        user = RED;
    }
    else if (c == 'B')
    {
        comp = RED;
        user = BLUE;
    }
    else
    {
        assert(0);
    }
    while (!is_full(board) && !has_won(board, user) && !has_won(board, comp))
    {
        if (curr == comp)
        {
            play_move(board, best_move(board, comp), comp);
        }
        else
        {
            print_board(board);
            int choice;
            printf("Enter your move: ");
            scanf("%d", &choice);
            move_t m = {choice, 0};
            play_move(board, m, user);
        }
        if (has_won(board, user))
        {
            printf("You Won");
            print_board(board);
        }
        else if (has_won(board, comp))
        {
            printf("You Lost");
            print_board(board);
        }
        else if (is_full(board))
        {
            printf("Draw");
            print_board(board);
        }
        curr = other_player(curr);
    }
    return 0;
}
