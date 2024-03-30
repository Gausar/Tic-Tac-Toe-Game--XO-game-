#include "csapp.h"
#include <pthread.h>

char board[20][20];
pthread_mutex_t board_mutex = PTHREAD_MUTEX_INITIALIZER;

int check_winner(int x, int y, char player);
void echo(int connfd);
void *run_game(void *pos);
void *run_game_server(void *pos);

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

struct position {
    int x;
    int y;
};
typedef struct position position;

int main(int argc, char **argv) {
    int clientfd;
    char *host, *port;
	socklen_t serverlen;
    position server, client_pos;
    rio_t rio;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }

    host = argv[1];
    port = argv[2];
    
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            board[i][j] = ' ';
        }
    }

    //draw_board(board);
    clientfd = open_clientfd(host, port);
    
    Rio_readinitb(&rio, clientfd);

    printf("Please start a game!\n");
    printf("Enter position (x y): \n");
    while (1) {
		
		pthread_t thread;
        
        scanf("%d %d", &client_pos.x, &client_pos.y);
        client_pos.x--;
        client_pos.y--;
		Rio_writen(clientfd, &client_pos, sizeof(position));
        pthread_create(&thread, NULL, run_game, (void *)&client_pos);

		Rio_readn(clientfd, &server, sizeof(position));
		pthread_create(&thread, NULL, run_game_server, (void *)&server);
		//pthread_join(thread, NULL);

		//echo(clientfd);
    }

    Close(clientfd);
    exit(0);
}
void echo(int connfd){
	size_t n;
	char buf[MAXLINE];
	rio_t rio;
	Rio_readinitb(&rio, connfd);
	while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0){
		printf("server received %d bytes\n", (int)n);
		Rio_writen(connfd, buf, n);
	}
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

void *run_game(void *pos) {
    int x, y;
    position *params = (position *)pos;
    x = params->x;
    y = params->y;
    char player = 'x'; //ekhnii toglogch o baival

	//pthread_mutex_lock(&board_mutex);
    if (board[x][y] == ' ') {
        board[x][y] = player;
        draw_board(board);
        if (check_winner(x, y, player)) {
            printf("Congrats, you win!\n");
            exit(0);
        }
    } else {
        printf("Invalid move! Cell already occupied.\n");
        /*while(1){
			printf("Enter position again! (x, y):\n");
			scanf("%d %d", &x, &y);
			if (board[x][y] == ' ') {
				board[x][y] = player;
				draw_board(board);
				if (check_winner(x, y, player)) {
					printf("Congrats, you win!\n");
					exit(0);
				}
				break;
			}
		}*/
    }
	//pthread_mutex_unlock(&board_mutex);

    return NULL;
}
void *run_game_server(void *pos) {
    position *params = (position *)pos;
    int x = params->x;
    int y = params->y;
    char player = 'o'; //hoyrdahi toglogch x eer yavna

	//pthread_mutex_lock(&board_mutex);
    if (board[x][y] == ' ') {
        board[x][y] = player;
        draw_board(board);
        if (check_winner(x, y, player)) {
            printf("Your competitor wins!\n");
            exit(0);
        }
        printf("enter position (x, y): \n");
    }
    
	//pthread_mutex_unlock(&board_mutex);
    return NULL;
}