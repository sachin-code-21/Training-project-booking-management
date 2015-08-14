#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include "mail.c"
int valid_mail(char x[]);
static struct termios old, new;

int signup(char *strlog)
{
	FILE *f1;
	
	char t[50][50],s[50],rpw[50],rc,pw[50],c,mail[50];
	int i=0,f=0,b=0,e=0,result=0;


	f1=fopen("db.csv","a");
	strcpy(strlog,"");
	strcat(strlog,"2");
	strcat(strlog,",");
	
	printf("\nName: ");
	getch();


	/* Accepting the name and validating it */

	while(1)
	{
		scanf("%[^\n]*c",t[0]);
		if(!isalpha(t[0][0]))
		{
			printf("Please enter valid name :");
			clear();
		}
		else
		{
			break;
		}
		
	}

	clear();
	
	/* Accepting and vallidating Gender */
	printf("\nGender: ");
	

	while(1)
	{
		scanf("%[^\n]*c",t[1]);

		if(!isalpha(t[1][0]))
		{
			printf("Please enter a valid gender :");
			clear();
		}
		else
		{
			break;
		}
	}

	clear();
	
	printf("\nEmail: ");

	/* Accepting and validating email-id */
	
	do{
	  scanf("%[^\n]*c",mail);
      result=valid_mail(mail);
      if(result!=0)
      {
        printf("retype email:");
        clear();
    }
      }while(result==-1);

    if(result==0)
    {
    strcpy(t[2],mail);
	//printf("%s",t[2]);
    //printf("\nsuccess\n");
	}

	clear();

/* Accepting and vallidating mobile number */

	printf("\nMob No: ");
	
	while(1)
	{
		scanf("%[^\n]*c",t[3]);
		if(!isalnum(t[3][0]))
		{
			printf("Please enter valid Mobile number :");
			clear();
		}
		else
		{
			break;
		}
	}

	clear();

	/* Accepting and vallidating OP number */

	printf("\nOP Number(if any,otherwise type NILL): ");
	
	while(1)
	{
		scanf("%[^\n]*c",t[6]);
		if(!isalnum(t[6][0]))
		{
			printf("Please enter valid OP number or type NILL :");
			clear();
		}
		else
		{
			break;
		}
	}
	
	clear();
	
	/* Accepting password in 2 fields and checking if both are same to confirm the password */
	while(1)
	{
		printf("\nPassword: ");
	
		while((c = getch())!='\n')
		{
 			printf("*");
			pw[e++]=c;
		}
		pw[e]='\0';
		strcpy(t[4],pw);
	

		printf("\nRetype Password: ");
		while((rc = getch())!='\n')
		{
 			printf("*");
			rpw[b++]=rc;
		}
		rpw[b]='\0';
		strcpy(t[5],rpw);		
		
		if(strcmp(t[5],t[4])==0)
		{

			
			for(f=0;f<7;f++)
			{
				
				if(f==5)
				{
					continue;
				}
				
				strcat(strlog,t[f]);
				if(f!=6)
				{
					
					strcat(strlog,",");
				}
			}
			//strcat(strlog,t[6]);
			//printf("\n%s :signupfile",strlog);		/* Displaying the signup details entered by the user */	
			//printf("\nsignup success full!!! \n");
			break;
		}
		else
		{
			printf("\npasswords not matching");
			printf("\n please retype the password again!! \n");
			e=0;
			b=0;
		}

	}
return 0;
}