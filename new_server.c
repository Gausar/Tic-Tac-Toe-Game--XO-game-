/*
Хэрвээ та энэ кодыг харж байгаа бол эхлээд Server талыг ажиллуулаарай.
*/

#include"csapp.h"

char board[20][20];

void draw_board(char board[][20]){
	printf(" --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---\n");
	printf("| ");
	for(int i = 0; i < 9; i++){
		printf("%d | ", i);
	}
	for(int i = 9; i < 21; i++){
		printf("%d |", i);
	}
	printf("\n");
	printf(" --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---\n");
	for(int i = 0; i < 9; i++){
		printf("| %d |", i + 1);
		for(int j = 0; j < 20; j++){
			printf(" %c |", board[i][j] );
		}
		printf("\n");
		printf(" --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---\n");
	}
	for(int i = 9; i < 20; i++){
		printf("|%d |", i+1);
		for(int j = 0; j < 20; j++){
			printf(" %c |", board[i][j]);
		}
		printf("\n");
		printf(" --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---\n");
	}
}
int final = 0;
struct position{
	int x;
	int y;
};
typedef struct position position;
//void echo(int connfd);
int run_game(position * pos);
int check_winner(int x, int y, char player);
void run_game_client(position *pos);

int main(int argc, char **argv){
	int listenfd, connfd;
	socklen_t clientlen;
	position server, client_pos;
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 20; j++){
			board[i][j] = ' ';
		}
	}
	struct sockaddr_storage clientaddr;
	char client_hostname[MAXLINE], client_port[MAXLINE];
	listenfd = open_listenfd(argv[1]);
	
	clientlen = sizeof(struct sockaddr_storage);
	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
	Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
	printf("Connected to (%s %s)\n", client_hostname, client_port);

	while(1){
		
		Rio_readn(connfd, &client_pos, sizeof(position));
		run_game_client(&client_pos);
		while(1){
            printf("enter position (x, y): \n");
            int k;
            scanf("%d %d", &server.x, &server.y);
            server.x--;
            server.y--;
            if(server.x < 21 && server.y < 21){
                k = run_game(&server);
            }
            if(k == 1){
                break;
            }
        }
        Rio_writen(connfd, &server, sizeof(position));
        if(final == 1){
            Close(connfd);
        }
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

int run_game(position *params) {
	int x;
	int y;
    x = params->x;
    y = params->y;
    char player = 'o'; //ekhnii toglogch o baival

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

void run_game_client(position *params) {
    int x = params->x;
    int y = params->y;
    char player = 'x'; //hoyrdahi toglogch x eer yavna

    if (board[x][y] == ' ') {
        board[x][y] = player;
		draw_board(board);
        if (check_winner(x, y, player)) {
            printf("Your competitor wins!\n");
			exit(0);
        }
    }
}