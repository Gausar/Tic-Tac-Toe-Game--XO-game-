/*
Хэрвээ та энэ кодыг харж байгаа бол эхлээд Server талыг ажиллуулаарай.
*/

#include "csapp.h"

char board[20][20];

void draw_board(char board[][20]) {
    printf(" --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---\n");
    printf("| ");
    for (int i = 0; i < 9; i++) {
        printf("%d | ", i);
    }
    for (int i = 9; i < 21; i++) {
        printf("%d |", i);
    }
    printf("\n");
    printf(" --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---\n");
    for (int i = 0; i < 9; i++) {
        printf("| %d |", i + 1);
        for (int j = 0; j < 20; j++) {
            printf(" %c |", board[i][j]);
        }
        printf("\n");
        printf(" --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---\n");
    }
    for (int i = 9; i < 20; i++) {
        printf("|%d |", i + 1);
        for (int j = 0; j < 20; j++) {
            printf(" %c |", board[i][j]);
        }
        printf("\n");
        printf(" --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---\n");
    }
}
int final = 0;
struct position {
    int x;
    int y;
};
typedef struct position position;

int check_winner(int x, int y, char player);
int run_game(position *pos);
void run_game_server(position *pos);

int main(int argc, char **argv) {
    int clientfd;
    char *host, *port;
	socklen_t serverlen;
    position server, client_pos;
    rio_t rio;

    if (argc != 3) {
        fprintf(stderr, "Argument dutuu %s <host> <port>\n", argv[0]);
        exit(0);
    }

    host = argv[1];
    port = argv[2];
    
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            board[i][j] = ' ';
        }
    }

    clientfd = open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    printf("Please start a game!\n");
    while (1) {
        while(1){
            int k;
            printf("enter position (x, y): \n");
            scanf("%d %d", &client_pos.x, &client_pos.y);
            client_pos.x--;
            client_pos.y--;
            if(client_pos.x <= 20 && client_pos.y <= 20){
                k = run_game(&client_pos);
            }
            if(k == 1){
                break;
            }
        }
        Rio_writen(clientfd, &client_pos, sizeof(position));
        if(final == 1){
            Close(clientfd);
        }
		Rio_readn(clientfd, &server, sizeof(position));
		run_game_server(&server);
    }
    return 0;
}

int check_winner(int x, int y, char player) {
    int i, j;
    int count;

    //hondlongoor
    count = 1;
    for (j = y + 1; j < 20 && board[x][j] == player; j++) {
        count++;
    }
    for (j = y - 1; j >= 0 && board[x][j] == player; j--) {
        count++;
    }
    if (count >= 5) return 1;

    //dooshoo
    count = 1;
    for (i = x + 1; i < 20 && board[i][y] == player; i++) {
        count++;
    }
    for (i = x - 1; i >= 0 && board[i][y] == player; i--) {
        count++;
    }
    if (count >= 5){
		return 1;
	}
    //diagonalaar zuun deedees
    count = 1;
    for (i = x + 1, j = y + 1; i < 20 && j < 20 && board[i][j] == player; i++, j++) {
        count++;
    }
    for (i = x - 1, j = y - 1; i >= 0 && j >= 0 && board[i][j] == player; i--, j--) {
        count++;
    }
    if (count >= 5){
		return 1;
	}
    //diagonalaar
    count = 1;
    for (i = x + 1, j = y - 1; i < 20 && j >= 0 && board[i][j] == player; i++, j--) {
        count++;
    }
    for (i = x - 1, j = y + 1; i >= 0 && j < 20 && board[i][j] == player; i--, j++) {
        count++;
    }
    if (count >= 5){
		return 1;
	}
    return 0;
}

int run_game(position *pos) {
    int x, y;
    x = pos->x;
    y = pos->y;
    char player = 'x'; //client tal toglogch x baina

    if (board[x][y] == ' ') {
        board[x][y] = player;
        draw_board(board);
        if (check_winner(x, y, player)) {
            printf("Congrats, you win!\n");
            final = 1;
        }
        return 1;
    } else {
        printf("Invalid move! Cell already occupied.\n");
        return 0;
    }
}
void run_game_server(position *pos) {
    int x = pos->x;
    int y = pos->y;
    char player = 'o'; //hoyrdahi tald toglogch o eer yavna

    if (board[x][y] == ' ') {
        board[x][y] = player;
        draw_board(board);
        if (check_winner(x, y, player)) {
            printf("Your competitor wins!\n");
            exit(0);
        }
    }
}