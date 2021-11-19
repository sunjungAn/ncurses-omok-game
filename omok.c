#include<stdio.h>
#include"omok.h"
#include<string.h>

int man;
int find1, find2;
user user_info[100];
int main(void)
{
//==============init=============================================
	int len = 0;//length of final structure, number of users
	int i = 0;
	FILE * ft = NULL;
	ft = fopen("data.txt", "r");//read mode 
	if(ft == NULL){
		ft = fopen("data.txt", "w");
		fclose(ft);
		ft = fopen("data.txt", "r");
	}
	char ch;
	char word[30];
	int k = 0;
	while(fscanf(ft, "%c", &ch)!=EOF){
		if(ch == ',' || ch =='\n')
		{
			word[i] = '\0';
			if(k == 0){//first word
				strcpy(user_info[len].name,word);
			}
			else if(k ==1)//second word
				strcpy(user_info[len].win,word);
			else if(k ==2){//third word
				strcpy(user_info[len++].lose,word);
				k = -1;
			}
			k++;
			i=0;
		}
		else word[i++] = ch;
	}
//================menu====================================================
	user user1;//a game player1
	user user2;//a game player2
	while(1){//compose for repeating
		int choice = menu();
		if(choice ==0){//if first choose--> game start!
			man = userinput(user1.name, user2.name);
			if(man == 27)//if input esc --> back
				continue;
		if((find1=find(user_info,len, user1.name))==-1)
		{//if user1 is not existing user
			strcpy(user1.win,"0");//initialize '0'
			strcpy(user1.lose,"0");//initialize '0'
		}
		else{//if user is existing user
			strcpy(user1.win, user_info[find1].win);
			strcpy(user1.lose, user_info[find1].lose);
		}//recall the winning and losing records of existing users

		if((find2=find(user_info, len, user2.name))==-1){
			strcpy(user2.win,"0");
			strcpy(user2.lose,"0");
		}//if user2 is not existing user-->initialize 0
		else{//if user is existing user
		       	strcpy(user2.win, user_info[find2].win);
			strcpy(user2.lose, user_info[find2].lose);
		}//recall the winning and losing records of existing users

		omok_game(&user1, &user2);
		user_info[len++] = user1;//add user1
		user_info[len++] = user2;//add user2

			break;
		}
		else if(choice ==1){//if second chose --> show manual
			 man = manual();
			 if(man == 27)//if input esc --> back 
				 continue;
		}
		else if(choice ==2)
		{//third's choose --> find user exe
			finduser(user_info, len);
				continue;
		}
		else break;
	}
	FILE * ftw = NULL;
	ftw = fopen("data.txt", "w");//finally re-enter all users into the file
	for(int k = 0; k<len; k++)
	{
		if(k == find1|| k==find2){
				continue;
		}
		fprintf(ftw, "%s,%s,%s\n",user_info[k].name, 
				user_info[k].win, user_info[k].lose);
	}
	fclose(ftw);
	return 0;
}
	
