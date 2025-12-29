#include <stdio.h>
#include <conio.h>
//#include <curses.h>

//Fateme Abedi , Maede Ghaniabadi , Bita Shahri

#define Board_Dimention 8

// in board cells, 0 is empty, 1 is white and 2 is black.

struct DiskInfo {
    int row;
    int col;
    int color;   // 1: white   -   2: black
};

void clean_board(int board[Board_Dimention][Board_Dimention]);
void print_board(int board[Board_Dimention][Board_Dimention], int allowed_moves[Board_Dimention][Board_Dimention]);
int play_turn(int board[Board_Dimention][Board_Dimention], int allowed_moves[Board_Dimention][Board_Dimention], int* player_turn);
int spot_is_available(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn, int make_changes_to_board);
void change_turn(int* turn);
void get_allowed_moves(int board[Board_Dimention][Board_Dimention], int allowed_moves[Board_Dimention][Board_Dimention], int player_turn);
int there_are_allowed_moves(int allowed_moves[Board_Dimention][Board_Dimention]);
int there_are_empty_spots(int board[Board_Dimention][Board_Dimention]);
void get_winner(int board[Board_Dimention][Board_Dimention]);
void counter_of_score(int board[Board_Dimention][Board_Dimention]);

//===================================================================
//===================================================================


// these fonctions help to check if a spot is availble or not
int check_row_right(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn);

int check_row_left(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn);

int check_col_up(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn);

int check_col_down(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn);

int check_diag_ascending_top(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn);

int check_diag_ascending_bottom(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn);

int check_diag_descending_top(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn);

int check_diag_descending_bottom(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn);


//===================================================================
//===================================================================

// these fonctions help to make changes that we want in the board of the game
void take_opp_disk_row_right(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn);

void take_opp_disk_row_left(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn);

void take_opp_disk_col_up(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn);

void take_opp_disk_col_down(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn);

void take_opp_disk_diag_ascending_top(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn);

void take_opp_disk_diag_ascending_bottom(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn);

void take_opp_disk_diag_descending_top(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn);

void take_opp_disk_diag_descending_bottom(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn);


//===================================================================
//===================================================================




int main() {
    int Board[Board_Dimention][Board_Dimention];
    int allowed_moves[Board_Dimention][Board_Dimention];
    int player_turn=1, last_player_played=0, game_running=2;

    clean_board(Board);
    //cheking

    while (game_running != 0 && there_are_empty_spots(Board)==1){

        get_allowed_moves(Board, allowed_moves, player_turn);
        print_board(Board, allowed_moves);
        counter_of_score(Board);
        last_player_played = play_turn(Board, allowed_moves, &player_turn);
        // if no moves are allowed for 2 times in a row, that means neither of the players were allowed any moves and so the game must be over
        if (last_player_played==1) {
            if (game_running<2) game_running=2;
        }
        else {
            game_running-=1;
        }
    }

    print_board(Board, allowed_moves);
    get_winner(Board);

    return 0;
}


// cleans all the spots of the bord except the center ones
void clean_board(int board[Board_Dimention][Board_Dimention]) {
    for (int i=0; i<Board_Dimention; i++) {
        for (int j=0; j<Board_Dimention; j++) {
            board[i][j] = 0;
        }
    }

    // white
    board[Board_Dimention/2 - 1][Board_Dimention/2 - 1] = 1;
    board[Board_Dimention/2][Board_Dimention/2] = 1;

    // black
    board[Board_Dimention/2 - 1][Board_Dimention/2] = 2;
    board[Board_Dimention/2][Board_Dimention/2 - 1] = 2;
}


// prints out the board
void print_board(int board[Board_Dimention][Board_Dimention], int allowed_moves[Board_Dimention][Board_Dimention]) {

    // show column numbers
    printf("\n   ");
    for (int k=0; k<Board_Dimention; k++) {
        printf("  %d ", k+1);
    }

    // add horizontal line depending on the Board_Diamention
    printf("\n   ");
    for (int i=0; i<Board_Dimention; i++)
    {
        printf("----");
    }
    printf("\n");

    // print out the spots
    for (int i=0; i<Board_Dimention; i++) {
        printf("%d  |", i+1);
        for (int j=0; j<Board_Dimention; j++) {

            switch (board[i][j])
            {
                case 0:
                    // if the empty spot is allowed for the palyer, it has a * in it.
                    if (allowed_moves[i][j]==1)
                        printf(" * |", board[i][j]);
                    else
                        printf("   |", board[i][j]);
                    break;

                case 1:
                    printf(" W |", board[i][j]); // W for white
                    break;

                case 2:
                    printf(" B |", board[i][j]); // B for black
                    break;
            }
        }

        // add horizontal lines depending on the Board_Diamention
        printf("\n   ");
        for (int i=0; i<Board_Dimention; i++)
        {
            printf("----");
        }
        printf("\n");

    }
    printf("\n");
}


// this function handles players' turns.
int play_turn(int board[Board_Dimention][Board_Dimention], int allowed_moves[Board_Dimention][Board_Dimention], int* player_turn) {

    // this makes sure that the player has atleast one allowed move.
    int c = there_are_allowed_moves(allowed_moves);
    if (c == 0) {
        switch (*player_turn)
        {
            case 1:
                printf("It's WHITE's turn. You can't make any moves! Press enter to continue : ");
                break;

            case 2:
                printf("It's BLACKS's turn. You can't make any moves! Press enter to continue : ");
                break;
        }
        change_turn(player_turn);
        getch();
        return 0;
    }

    struct DiskInfo disk;

    // each player plays their turn
    switch (*player_turn)
    {
        case 1:
            printf("It's WHITE's turn. choose a spot (row, column) : ");
            scanf("%d %d", &disk.row, &disk.col);
            disk.color = 1;
            break;

        case 2:
            printf("It's BLACK's turn. choose a spot  (row, column) : ");
            scanf("%d %d", &disk.row, &disk.col);
            disk.color = 2;
            break;
    }

    // this makes sure the spot that the player chose is available for that player.
    int t = spot_is_available(board, disk.row, disk.col, *player_turn, 1);
    if (t == 1) {
        board[disk.row-1][disk.col-1] = disk.color;
        change_turn(player_turn);
        return 1;
    }
    else {
        printf("The spot you chose is not available! Choose again!\n");
        play_turn(board, allowed_moves, player_turn);
    }

    // return 0;
}



// ======================= these functions evaluate the moves =========================

int check_row_right(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn) {
    int opposite_turn = 3 - player_turn;
    if (board[disk_row][disk_col+1] != opposite_turn)
        return 0;
    for (int i=disk_col+1; i<Board_Dimention; i++) {
        if (board[disk_row][i] == player_turn)
            return 1;
    }
    return 0;
}

void take_opp_disk_row_right(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn){
    int opposite_turn = 3 - player_turn;

    for (int i=disk_col+1; i<Board_Dimention; i++) {
        if (board[disk_row][i] == opposite_turn)
            board[disk_row][i] = player_turn;
        else
            break;
    }
}

int check_row_left(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn) {
    int opposite_turn = 3 - player_turn;
    if (board[disk_row][disk_col-1] != opposite_turn)
        return 0;
    for (int i=disk_col-1; i>=0; i--) {
        if (board[disk_row][i] == player_turn)
            return 1;
    }
    return 0;
}

void take_opp_disk_row_left(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn){
    int opposite_turn = 3 - player_turn;

    for (int i=disk_col-1; i>=0; i--) {
        if (board[disk_row][i] == opposite_turn)
            board[disk_row][i] = player_turn;
        else
            break;
    }
}

int check_col_up(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn) {
    int opposite_turn = 3 - player_turn;
    if (board[disk_row-1][disk_col] != opposite_turn)
        return 0;
    for (int i=disk_row-1; i>=0; i--) {
        if (board[i][disk_col] == player_turn)
            return 1;
    }
    return 0;
}

void take_opp_disk_col_up(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn){
    int opposite_turn = 3 - player_turn;

    for (int i=disk_row-1; i>=0; i--) {
        if (board[i][disk_col] == opposite_turn)
            board[i][disk_col] = player_turn;
        else
            break;
    }
}

int check_col_down(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn) {
    int opposite_turn = 3 - player_turn;
    if (board[disk_row+1][disk_col] != opposite_turn)
        return 0;
    for (int i=disk_row+1; i<Board_Dimention; i++) {
        if (board[i][disk_col] == player_turn)
            return 1;
    }
    return 0;
}

void take_opp_disk_col_down(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn){
    int opposite_turn = 3 - player_turn;

    for (int i=disk_row+1; i<Board_Dimention; i++) {
        if (board[i][disk_col] == opposite_turn)
            board[i][disk_col] = player_turn;
        else
            break;
    }
}

int check_diag_ascending_top(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn) {
    int opposite_turn = 3 - player_turn;
    if (board[disk_row-1][disk_col+1] != opposite_turn)
        return 0;

    int i=1;
    while (disk_row-i>=0 && disk_col+i<Board_Dimention) {
        if (board[disk_row-i][disk_col+i] == player_turn)
            return 1;
        i++;
    }

    return 0;
}

void take_opp_disk_diag_ascending_top(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn){
    int opposite_turn = 3 - player_turn;

    int i=1;
    while (disk_row-i>=0 && disk_col+i<Board_Dimention) {
        if (board[disk_row-i][disk_col+i] == opposite_turn)
            board[disk_row-i][disk_col+i] = player_turn;
        else
            break;
        i++;
    }
}

int check_diag_ascending_bottom(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn) {
    int opposite_turn = 3 - player_turn;
    if (board[disk_row+1][disk_col-1] != opposite_turn)
        return 0;

    int i=1;
    while (disk_row+i<Board_Dimention && disk_col-i>=0) {
        if (board[disk_row+i][disk_col-i] == player_turn)
            return 1;
        i++;
    }

    return 0;
}

void take_opp_disk_diag_ascending_bottom(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn){
    int opposite_turn = 3 - player_turn;

    int i=1;
    while (disk_row+i<Board_Dimention && disk_col-i>=0) {
        if (board[disk_row+i][disk_col-i] == opposite_turn)
            board[disk_row+i][disk_col-i] = player_turn;
        else
            break;
        i++;
    }
}

int check_diag_descending_top(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn) {
    int opposite_turn = 3 - player_turn;
    if (board[disk_row-1][disk_col-1] != opposite_turn)
        return 0;

    int i=1;
    while (disk_row-i>=0 && disk_col-i>=0) {
        if (board[disk_row-i][disk_col-i] == player_turn)
            return 1;
        i++;
    }

    return 0;
}

void take_opp_disk_diag_descending_top(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn){
    int opposite_turn = 3 - player_turn;

    int i=1;
    while (disk_row-i>=0 && disk_col-i>=0) {
        if (board[disk_row-i][disk_col-i] == opposite_turn)
            board[disk_row-i][disk_col-i] = player_turn;
        else
            break;
        i++;
    }
}

int check_diag_descending_bottom(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn) {
    int opposite_turn = 3 - player_turn;
    if (board[disk_row+1][disk_col+1] != opposite_turn)
        return 0;

    int i=1;
    while (disk_row+i<Board_Dimention && disk_col+i<Board_Dimention) {
        if (board[disk_row+i][disk_col+i] == player_turn)
            return 1;
        i++;
    }

    return 0;
}

void take_opp_disk_diag_descending_bottom(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn){
    int opposite_turn = 3 - player_turn;

    int i=1;
    while (disk_row+i<Board_Dimention && disk_col+i<Board_Dimention) {
        if (board[disk_row+i][disk_col+i] == opposite_turn)
            board[disk_row+i][disk_col+i] = player_turn;
        else
            break;
        i++;
    }
}
// ======================================================================


// this function takes a player and a spot on the board and makes sure that palyer is allowed to play in that spot
int spot_is_available(int board[Board_Dimention][Board_Dimention], int disk_row, int disk_col, int player_turn, int make_changes_to_board) {

    // if the make_changes_to_board is set to 1, this function not only evaluates the move, but actually considers the move done and turns the opponent's disks.

    int move_is_allowed=0;
    disk_row--; // because when the user enters row 8, we actually need Board[7]
    disk_col--; // because when the user enters column 8, we actually need Board[n][7]

    // if the chosen spot is not empty or does not exist on the board we return 0
    if (board[disk_row][disk_col]!=0)
        return 0;
    if (disk_row>Board_Dimention-1 || disk_row<0 || disk_col>Board_Dimention-1 || disk_col<0) return 0;

    if (disk_col<Board_Dimention-2) {
        int d = check_row_right(board, disk_row, disk_col, player_turn);
        if (d == 1) {
            if (make_changes_to_board==1)
                take_opp_disk_row_right(board, disk_row, disk_col, player_turn);
            move_is_allowed = 1;
        }
    }

    if (disk_col>1) {
        int d =check_row_left(board, disk_row, disk_col, player_turn);
        if (d == 1) {
            if (make_changes_to_board==1)
                take_opp_disk_row_left(board, disk_row, disk_col, player_turn);
            move_is_allowed = 1;
        }
    }

    if (disk_row>1) {
        int d =check_col_up(board, disk_row, disk_col, player_turn);
        if (d == 1) {
            if (make_changes_to_board==1)
                take_opp_disk_col_up(board, disk_row, disk_col, player_turn);
            move_is_allowed = 1;
        }
    }

    if (disk_row<Board_Dimention-2) {
        int d = check_col_down(board, disk_row, disk_col, player_turn);
        if (d == 1) {
            if (make_changes_to_board==1)
                take_opp_disk_col_down(board, disk_row, disk_col, player_turn);
            move_is_allowed = 1;
        }
    }

    if (disk_row>1 && disk_col<Board_Dimention-2) {
        int d = check_diag_ascending_top(board, disk_row, disk_col, player_turn);
        if (d == 1) {
            if (make_changes_to_board==1)
                take_opp_disk_diag_ascending_top(board, disk_row, disk_col, player_turn);
            move_is_allowed = 1;
        }
    }

    if (disk_row<Board_Dimention-2 && disk_col>1) {
        int d = check_diag_ascending_bottom(board, disk_row, disk_col, player_turn);
        if (d == 1) {
            if (make_changes_to_board==1)
                take_opp_disk_diag_ascending_bottom(board, disk_row, disk_col, player_turn);
            move_is_allowed = 1;
        }
    }

    if (disk_row>1 && disk_col>1) {
        int d = check_diag_descending_top(board, disk_row, disk_col, player_turn);
        if (d == 1) {
            if (make_changes_to_board==1)
                take_opp_disk_diag_descending_top(board, disk_row, disk_col, player_turn);
            move_is_allowed = 1;
        }
    }

    if (disk_row<Board_Dimention-2 && disk_col<Board_Dimention-2) {
        int d = check_diag_descending_bottom(board, disk_row, disk_col, player_turn);
        if (d == 1) {
            if (make_changes_to_board==1)
                take_opp_disk_diag_descending_bottom(board, disk_row, disk_col, player_turn);
            move_is_allowed = 1;
        }
    }

    return move_is_allowed;
}


// changes the turn from white to black and vice versa
void change_turn(int* turn) {
    *turn = 3 - *turn;
}


// takes a player and gives you all the spots on the board that are allowed for that player at that stage
void get_allowed_moves(int board[Board_Dimention][Board_Dimention], int allowed_moves[Board_Dimention][Board_Dimention], int player_turn) {
    for (int i=0; i<Board_Dimention; i++) {
        for (int j=0; j<Board_Dimention; j++) {
            allowed_moves[i][j] = spot_is_available(board, i+1, j+1, player_turn, 0);
        }
    }
}


// tells you whether or not there are any allowed moves for a player at that stage
int there_are_allowed_moves(int allowed_moves[Board_Dimention][Board_Dimention]) {
    for (int i=0; i<Board_Dimention; i++) {
        for (int j=0; j<Board_Dimention; j++) {
            if (allowed_moves[i][j]==1) return 1;
        }
    }
    return 0;
}


// tells you whether or not there are any empty spots left on the board
int there_are_empty_spots(int board[Board_Dimention][Board_Dimention]) {
    for (int i=0; i<Board_Dimention; i++) {
        for (int j=0; j<Board_Dimention; j++) {
            if (board[i][j]==0) return 1;
        }
    }
    return 0;
}


// determines the winner
void get_winner(int board[Board_Dimention][Board_Dimention]) {
    int white=0, black=0;

    for (int i=0; i<Board_Dimention; i++) {
        for (int j=0; j<Board_Dimention; j++) {
            switch (board[i][j])
            {
                case 1:
                    white++;
                    break;

                case 2:
                    black++;
                    break;

                default:
                    break;
            }
        }
    }

    if (white > black)
        printf("\nThe winner is WHITE!\n");
    else if (black > white)
        printf("\nThe winner is BLACK!\n");
    else
        printf("\nIt's a tie!\n");

    printf("Press enter to exit : ");
    getch();
}
 void counter_of_score(int board[Board_Dimention][Board_Dimention]){
    char name[1][2];
    name[0][0]='W';
    name[0][1]='B';

    int W = 0;
    int B = 0;
     for (int i = 0; i < Board_Dimention ; ++i) {
         for (int j = 0; j < Board_Dimention; ++j) {
             switch (board[i][j]) {

                 case 1:
                      W++;
                      break;

                 case 2:
                     B++;
                     break;

                 default:
                     break;


             }
         }

     }
     int score[1][2];
     score[1][0]= W;
     score[1][1]= B;

     for (int i=0; i<2; i++)
     {
         printf("----");
     }
     printf("\n");

     for (int i = 0; i < 1; ++i) {
         for (int j = 0; j < 2; ++j) {
             printf("| %c " , name[i][j]);

         }
         printf("|");
         printf("\n");
     }
     for (int i=0; i<2; i++)
     {
         printf("----");
     }
     printf("\n");

     for (int i = 0; i < 1; ++i) {
         for (int j = 0; j < 2; ++j) {
             printf("| %d ",score[i][j] );
         }
         printf("|");
         printf("\n");
     }
     for (int i=0; i<2; i++)
     {
         printf("----");
     }
     printf("\n");
}