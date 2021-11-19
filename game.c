#include<ncurses.h>
#include<form.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include"omok.h"
char * array[18][19];
char buffer[sizeof(int)*8+1];
int userinput(char *user1, char *user2)//input user name1, name2
{
	FIELD *field[3];//for input user name 
	FORM *my_form;//making field
	clear();
	int ch;

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	field[0] = new_field(1, 10, 12, 38 , 0, 0);//make a file in a proper location
	field[1] = new_field(1, 10, 14, 38, 0, 0);
	field[2] = NULL;

	set_field_back(field[0], A_UNDERLINE);//show underline
	field_opts_off(field[0], O_AUTOSKIP);
	
	set_field_back(field[1], A_UNDERLINE);
	field_opts_off(field[1], O_AUTOSKIP);

	my_form = new_form(field);
	post_form(my_form);
	refresh();

	mvprintw(12,30,"User 1:");
	mvprintw(14,30,"User 2:");
	mvprintw(1,1,"Press F1 when you're done entering");//show manual
	mvprintw(2,1,"Press ESC if you want to go back");//show manual
	refresh();//show!!
	while(1){//repeat untill pressed f1key
	while((ch = getch()) !=KEY_F(1))
	{
		switch(ch)
		{
			case '\n'://when press enter, down key
			case KEY_DOWN://move under field
				form_driver(my_form, REQ_NEXT_FIELD);
				form_driver(my_form, REQ_END_LINE);
				break;
			case KEY_UP://when press key up 
				form_driver(my_form, REQ_PREV_FIELD);
				form_driver(my_form, REQ_END_LINE);
				break;//move top field
			case KEY_BACKSPACE://when press key backspace
				form_driver(my_form, REQ_DEL_PREV);//remove
				break;
			case 27://esc key
				endwin();//exit --> back to menu window
				return 27;
			default:
				form_driver(my_form, ch);//input string
				break;
		}
	}
	strcpy(user1, field_buffer(field[0], 0));//make user1 name
	strcpy(user2, field_buffer(field[1], 0));//make user2 name
	if(isspace(user1[0])||isspace(user2[0])){
		//if the name begins with a blank space, show error message
		mvprintw(16, 20,"please remove the spaces in the first letter");
		continue;
	}
	unpost_form(my_form);
	free_form(my_form);//end use form 
	free_field(field[0]);
	free_field(field[1]);
	free_field(field[2]);
	endwin();
	return 0;
}
}

void omok_game(user * user1, user * user2)
{
	
	int count = 0;
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);//user keypad

	int col, row;
	int xpoint=3;
	for(int i = 0; i<18; i++)
	{
		for(int j = 0; j<18; j++)
			array[i][j] = "+";//mark '+' if you don't have a stone
	}
	mvprintw(1,2,"play omok!\n");//play!
	while(1){
		mvprintw(1,15," %d time", count);//number of times you played
		for(char i = 0; i<18; i++){
		xpoint = 3;//adjusting the x-axis spacing
		mvprintw((3+i),2,"|");
			for(char j = 0; j<18; j++){
				mvprintw(3+i,xpoint,"-%s-", array[i][j]);
				xpoint+=3;//print omok 
			}
		mvprintw(3+i,57,"|");
		}
		int win = game();//if user1 win --> return1, 
		//if user2 win --> return2
		if(win == 1){//when user1 win 
			endwin();
			printf("%swin!\n",user1->name);
			int userwin = atoi(user1->win);
			int userlose = atoi(user2->lose);
			userwin++; userlose++;//user1's win increase
			//user2's lose increase
			sprintf(user1->win, "%d", userwin);
			sprintf(user2->lose,"%d", userlose);
			return;
		}
		else if(win ==2){//when user2 win
			endwin();
			printf("%swin!\n", user2->name);
			int userwin = atoi(user2->win);
			userwin++;//user2's win increase
			sprintf(user2->win,"%d", userwin);
			int userlose = atoi(user1->lose);
			userlose++;//user1's lose increase
			sprintf(user1->lose,"%d", userlose);
			return;
		}
	if(count % 2 ==0)//setting the order
		mvprintw(22, 3,"%s's turn!", user1->name);
	else
		mvprintw(22, 3, "%s's turn!", user2->name);

	move(11,31);//move around the position of the cursor
	refresh();//show window
	int choice;//lmplement plates as one-dimensional arrays for effect
	int highlight = 0;
	while(1){
		for(int i = 0; i<324; i++)//18*18 --> 324
		{
			if(i == highlight)//effect highlight
				attron(A_REVERSE);
			mvprintw((i/18+3),(((i%18)+1)*3+1),array[i/18][i%18]);
			attroff(A_REVERSE);//specify space for effect
		}
		choice=getch();
		switch(choice){
			case KEY_UP://when press key up --> up floor
				if(highlight<18)//1'st line
					highlight =highlight+(17*18);
				else highlight-=18;
				break;
			case KEY_DOWN://when press key down --> down floor
				if(highlight/18==17)//last line
					highlight = highlight%18;
				else highlight+=18;
				break;
			case KEY_LEFT://when press key left --> left element
				if(highlight%18 == 0)//first column
					highlight = highlight + 17;
				else highlight-=1;
				break;
			case KEY_RIGHT://when press key right--> right element
				if(highlight%18 == 17)//last column
					highlight = highlight -17;
				else highlight+=1;
				break;
			defalut:
				break;
		}
		if(choice == 10){//when press enter key
		//	mvprintw(1,25,"%d", highlight);
			refresh();
			int putcol, putrow;
			if(highlight<18){//if first flooer
				putcol = 0;
				putrow = highlight;//value of highlight variableoutput effects as they are
			}	
			else{ putcol = highlight/18;//~~th floor
				putrow = highlight%18;//~~th element
			}
			if(strcmp(array[putcol][putrow],"+")!=0){
			       	mvprintw(23,3,"you cannot put it here\n"); 
				continue;}//if there's already a rock here
			else mvprintw(23,3,"                           ");
			if(count%2 == 0)//user1->black, user2->white
				array[putcol][putrow] = "@";
			else
				array[putcol][putrow] = "O";
			count++;
			break;
		}
		if(choice == 27){
			endwin();//enter esc ---> back
			return;
		}

	}
}
	getch();
	endwin();
}

int game(void)//function to tell the winner of loser of a game
{
	for(int i = 0; i<18; i++)//examine all elements
		for(int j = 0; j<18; j++)
			//when there are five Go stones in row
			if((j<14)&&strcmp(array[i][j],"@")==0&&
					strcmp(array[i][j+1],"@") == 0 &&
					strcmp(array[i][j+2],"@") == 0 &&
					strcmp(array[i][j+3],"@") == 0 &&
					strcmp(array[i][j+4],"@") == 0)
				return 1;
			//when there are five Go stones in column
			else if((i<14)&&strcmp(array[i][j],"@") == 0 &&
					strcmp(array[i+1][j],"@") == 0 &&
					strcmp(array[i+2][j],"@") == 0 &&
					strcmp(array[i+3][j],"@") == 0 &&
					strcmp(array[i+4][j],"@") == 0)
				return 1;
			//when there are five Go stoens a diagonal line
			else if((j<14)&&(i<14)&&strcmp(array[i][j],"@")== 0 &&
					strcmp(array[i+1][j+1],"@") == 0 &&
					strcmp(array[i+2][j+2],"@") == 0 &&
					strcmp(array[i+3][j+3],"@") == 0 &&
					strcmp(array[i+4][j+4],"@")== 0)
				return 1;
			//when there are five Go stones a diagonal line
			else if((i>4)&&strcmp(array[i][j],"@") == 0 &&
					strcmp(array[i-1][j+1],"@") == 0 &&
					strcmp(array[i-2][j+2],"@") == 0 &&
					strcmp(array[i-3][j+3],"@") == 0 &&
					strcmp(array[i-4][j+4],"@") == 0)
				return 1;
			//when there are five Go stones in row
			else if((j<14)&&strcmp(array[i][j],"O")==0&&
					strcmp(array[i][j+1],"O") == 0 &&
					strcmp(array[i][j+2],"O") == 0 &&
					strcmp(array[i][j+3],"O") == 0 &&
					strcmp(array[i][j+4],"O") == 0)
				return 2;
			//when there are five Go stones in col
			else if((i<14)&&strcmp(array[i][j],"O") == 0 &&
					strcmp(array[i+1][j],"O") == 0 &&
					strcmp(array[i+2][j],"O") == 0 &&
					strcmp(array[i+3][j],"O") == 0 &&
					strcmp(array[i+4][j],"O") == 0)
				return 2;
			//when there are five Go stones in a diagonal line
			else if((i<14)&&(j<14)&&strcmp(array[i][j],"O") == 0 &&
					strcmp(array[i+1][j+1],"O") == 0 &&
					strcmp(array[i+2][j+2],"O") == 0 &&
					strcmp(array[i+3][j+3],"O") == 0 &&
					strcmp(array[i+4][j+4],"O")== 0)
				return 2;
			//when there are five Go stones in a diagonal line
			else if((i>4)&&(strcmp(array[i][j],"O")) == 0 &&
					strcmp(array[i-1][j+1],"O") == 0 &&
					strcmp(array[i-2][j+2],"O") == 0 &&
					strcmp(array[i-3][j+3],"O") == 0 &&
					strcmp(array[i-4][j+4],"O") == 0)
				return 2;
	return 0;
}


	
















