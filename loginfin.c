#include<stdio.h>
#include<string.h>
#include <termios.h>

static struct termios old, new;

/* For masking the password using '*' */

void initTermios(int echo) 
{
	tcgetattr(0, &old);                        /* grab old terminal i/o settings */
	new = old;                                 /* make new settings same as old settings */
	new.c_lflag &= ~ICANON;                    /* disable buffered i/o */
	new.c_lflag &= echo ? ECHO : ~ECHO;        /* set echo mode */
	tcsetattr(0, TCSANOW, &new);              /* use these new terminal i/o settings now */
}

void resetTermios(void) 
{
	tcsetattr(0, TCSANOW, &old);
}

char getch_(int echo) 
{
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

char getch(void) 
{
	return getch_(0);
}

/*void clear (void)
{    
	while ( getchar() != '\n' );
}*/

int login(char *strlog)
{
	char mail[100],pw[100],c='a';
	int i=0;
	printf("\nemail: ");/* accepting mail-id and password and masking password */
	scanf("%s",mail);
	clear();
	printf("\npassword: ");
	while(1)
	{
		c = getch();
		if(c!='\n')
		{
 			printf("*");
			pw[i++]=c;
		}
		else
		{
	
			break;
		}
	}
	pw[i]='\0';
	printf("\n");
	i=0;
	
	strcat(strlog,"1");
	strcat(strlog,",");
	strcat(strlog,mail);
	strcat(strlog,",");
	strcat(strlog,pw);
	//printf("%s : loginfin\n",strlog);

return 0;
}





