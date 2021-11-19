#include<ncurses.h>

typedef struct user{
	char name[30];
	char win[30];
	char lose[30];
}user;
int menu(void);
int manual(void);
int userinput(char *, char *);
void init(user *[100], int *);
int find(user *, int ,char *);
void omok_game(user *, user *);
int game(void);
void finduser(user *, int);
