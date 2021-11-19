#include<ncurses.h>
#include<stdio.h>
#include"omok.h"
#include<stdlib.h>
#include<string.h>
#include<form.h>
int menu(void)//first page -menu window
{
	clear();
	initscr();
	cbreak();
	noecho();

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);//get screen size
	
	WINDOW * menuwin = newwin((yMax/2),(xMax/3), (yMax/4),(xMax/3));
	box(menuwin, 0, 0);
	wattron(menuwin, A_BOLD);//implement a window so you can choose
	mvwprintw(menuwin, 0, 7, "<omok game>");
	wattroff(menuwin, A_BOLD);
	refresh();	//show window
	wrefresh(menuwin);

	keypad(menuwin, true);//for use keypad

	char * choices[3] = {"Game start", "Menual", "Find user"};
	int choice;
	int highlight = 0;

	while(1){//compose with repeating statements to enable window movement
		for(int i = 0; i<3; i++)
		{
			if(i == highlight)//highlight select menu
				wattron(menuwin, A_REVERSE);
			mvwprintw(menuwin, i+6, 7, choices[i]);
			wattroff(menuwin, A_REVERSE);
		}
		choice = wgetch(menuwin);//user choice!

		switch(choice){
			case KEY_UP://when key up
				if (choice == 1) highlight = 3;
				highlight--;
				break;
			case KEY_DOWN://when key down
				if(choice == 3) highlight = 1;
				highlight++;
				break;
			default://other 
				break;
		}
		if(choice == 10)//when enter key-->choose!!
			break;
	}
	endwin();
	return highlight;//returns the corresponding number of a selected menu
}

int manual(void)
{//mmanual window 
	clear();
	initscr();
	cbreak();
	noecho();
	int esc;
	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);

	WINDOW * manwin = newwin(yMax -12, xMax -12, 5,5);
	//configure the right size manual box
	box(manwin, 0, 0);
	mvwprintw(manwin, 0, 0,"<MANUAL>--Press key if you want to go back");
	mvwprintw(manwin, 3,1,"This program is a 'omok'program.\n This program has two functions.\n  1)game : The winner's name will be displayed when the game starts,\n    and the winner's name will be displayed.\n    This game should be used by two users.\n    User1 is a black stone, and user2 is a white stone, and if there\n    are 5 Go stones in a row, horizontally, vertically, and diagonally\n    the game is won\n\n  2)find-user : press the third menu and the corresponding window \n    will appear. Enter the name you want to find in the field, and you\n    can look up the names of users who are similar to that name, the\n    number of wins, and the number of defeats\n");
	
	refresh();
	wrefresh(manwin);
	esc = wgetch(manwin);
	//if getch -- > 27 , exit program.
	endwin();
	return esc;
}

int find(user *man,int len, char *findstr )//funtion for using finduser 
{
	int k = 0;
	int a = 0;

	while(a<=len)
	{
		if(strcmp(man[a].name, findstr)==0){
			return a;//compare success!!
		}
		a++;
	}
	return -1;//fail find
	

}

void finduser(user *man, int len)//3'th menu--> find user
{
	clear();
	initscr();
	cbreak();
	char findname[30];//the name you are looking for to enter
	noecho();
	keypad(stdscr, TRUE);//user keypad
	int xMax, yMax;
	getmaxyx(stdscr, yMax, xMax);

	WINDOW *findwin = newwin((yMax/2),(xMax/2),(yMax/4),(xMax/4));
	//compose window size
	box(findwin, 0,0);
	mvwprintw(findwin,2, 5, "name:");
	refresh();//show window
	wrefresh(findwin);
	getstr(findname);//input you are looking for 
	mvwprintw(findwin, 2,11,"%s", findname);
	refresh();//show name
	wrefresh(findwin);
	int k = 0;
	int a = 0;	
	while(k<=len){//strstr--> not equal, show name included
		if(strstr(man[k].name, findname)!=NULL){
			mvwprintw(findwin,a+4,2, "name: %s win : %s lose : %s"
			,man[k].name,man[k].win, man[k].lose);
			a++;
		}
		k++;
	}
	refresh();//show user name's, number of wins, number of loses
	wrefresh(findwin);
	getchar();
	endwin();
}

///find user
