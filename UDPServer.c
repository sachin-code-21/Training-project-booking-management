#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include <assert.h>

#define BUFLEN 512
#define RESPLEN 1024
#define LOGLEN 100
#define PORT 9931

struct sockaddr_in my_addr, cli_addr;
socklen_t slen=sizeof(cli_addr);

char mail[LOGLEN],resp[RESPLEN],buf[BUFLEN],email[LOGLEN];
int f=0,flag=0,sockfd,i=0,pch=0;

FILE *fp,*fp1,*fp2,*fp3;

void clear (void)
{    
	while ( getchar() != '\n' );
}

void err(char *str)
{
    perror(str);
    exit(1);
}

void tostring(char str[], int num)
{

    int i, rem, len = 0, n;

    n = num;

    while (n != 0)
    {
    	len++;
		n /= 10;
    }

    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }

    	str[len] = '\0';
}

int book_serv()
{	
	int dex=0,l=0,i=2,brid;;
	char *token,temp2[BUFLEN],temp[BUFLEN],drdetails[BUFLEN],token2[BUFLEN],docname[50],s[BUFLEN],date[50],s1[BUFLEN];
	fp2=fopen("doctors.csv","r");
	memset((temp), '\0', (BUFLEN));
	memset((temp2), '\0', (BUFLEN));
	token=strtok(buf,",");
	token= strtok (NULL,",");

	/* temp2 has the recived docter name */

	strcat(temp2,token);

	/* finding all the possible matches from the docters list */

	
	while(fgets(drdetails,1000,fp2)!=NULL)
	{      
		strcpy(token2,drdetails);
		token=strtok(drdetails,",");
		token= strtok (NULL, ",");
	 	token=strtok(token," ");

		if((strcasecmp(token,temp2))==0)
		{	
			dex=1;
			token=strtok(token2,",");
			token= strtok (NULL, ",");	
        	while (token != NULL)
  			{				
				strcat(resp,token);
				token= strtok (NULL, ",");
				if(token!=NULL)
				{
					strcat(resp,"\t\t");
				}
				
			}
			strcat(resp,"\n");			
		}

	}
	if(dex==0)
	{
	
		memset((resp), '\0', (RESPLEN));
		strcpy(resp,"No match found...!");
		dex=0;
	}

	fclose(fp2);
	memset((temp), '\0', (BUFLEN));
	memset((temp2), '\0', (BUFLEN));
	
	/*  sending all the possile matches to the client */

	if (sendto(sockfd, resp, RESPLEN, 0, (struct sockaddr*)&cli_addr, slen)==-1)
	{
    	err("sendto()");
    }
    if(dex==0)
    {
    		strcpy(resp,"");
    	return 0;	
    }
    
	memset((buf), '\0', (BUFLEN));
	memset((resp), '\0', (RESPLEN));
	memset((drdetails), '\0', (BUFLEN));
	
	dex=0;
	dex=0;
    
    /* reciving the full name of the docter from the client */

    if (recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&cli_addr, &slen)==-1)
    {
            err("recvfrom()");
    }	
    /* finding the exact match from the file based on the recived full name of the docter */
	fp1=fopen("availability.csv","r");
    if(fp1!=NULL)
    {
    	while(fgets(drdetails,1000,fp1)!=NULL)
		{    
			token=strtok(drdetails,",");
			strcpy(temp,token);
			int kp = strcasecmp(temp,buf); 
	 		if(kp == 0)
			{	
				strcpy(docname,temp);  /* storing the original name of the docter to the variable for writing to file*/
				dex=1;
				token= strtok (NULL, ",");	
				while (token != NULL)
				{
					strcat(resp,token);/* gathering the availability details of the corresponding matched case*/
					strcat(resp,"\t\t");
					token= strtok (NULL, ",");

				}
			}
		}
	}
	else
	{
		printf("cannot be opened");
	}
	if(dex==0)
	{
		strcpy(resp,"No match found... from date!");
		dex=0;
	}

	/* sending the collected availability details to the  client */

	if (sendto(sockfd, resp, RESPLEN, 0, (struct sockaddr*)&cli_addr, slen)==-1)
	{
    	err("sendto()");
    }
    fclose(fp1);
    
    memset((buf), '\0', (BUFLEN));
   
    /* reciving date from the client */
    if (recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&cli_addr, &slen)==-1)
    {
            err("recvfrom()");
    }
    strcpy(date,buf);
  //  printf("\n Reciving date from cilent :%s",date);

    /* updating the number of slotes based on the recived date in the availability file */
    dex=0;
    fp1=fopen("availability.csv","r+");
    if(fp1!=NULL)
    {
    	while(fgets(s,1000,fp1)!=NULL)
		{
			strcpy(s1,s);
			token=strtok(s1,",");
			strcpy(token2,token); /* token2 had the docter name  from the file */
			token= strtok (NULL, ",");/* token has the date of the correspoonting date from the file */

			if(strcasecmp(token,date)==0&&strcasecmp(token2,docname)==0)
			{
				l=strlen(s);
				l=l-2;
				if(s[l]=='0'&&s[l-1]!='0')
				{
					s[l]='9';
					s[l-1]--;
				}
				else if(s[l]=='0'&&s[l-1]=='0')
				{
					/* checking wether a slot is available or not */	
					dex=1;
					break;
				}
				else
				{	
					s[l]--;
				}
				fseek(fp1,-(strlen(s)),SEEK_CUR);
				fprintf(fp1,"%s",s);
				break;
			}
		}
	}
	else
	{
		printf("\n cannot open the file...!");
	}
	
	fclose(fp1);

	/* writing collected details into the appointment file for confirming the appointment */

	memset((buf), '\0', (BUFLEN));
	memset((resp), '\0', (RESPLEN));

	fp3=fopen("appointment.csv","a");

    srand(time(NULL));
	brid = rand() % 1000;
	tostring(s1,brid);

	strcpy(s,"");
	strcat(s,"0");
	strcat(s,",");
	strcat(s,s1);
	strcat(s,",");
	strcat(s,docname);
	strcat(s,",");
	strcat(s,email);
	strcat(s,",");
	strcat(s,date);			
	fputs(s,fp3); 
	fprintf(fp3,"\n");
	fclose(fp3);

	/* response to the client "booking reference id" */
	if(dex==1)
	{
		strcpy(resp,"No slots available...!");	
	}
	else
	{
		strcpy(resp,"\nPlease note down the booking reference id for further reference.\nYour booking reference id is:");	
		strcat(resp,s1);
	}

	/* sending the appropriate response to client about the booking */

	if (sendto(sockfd, resp, RESPLEN, 0, (struct sockaddr*)&cli_addr, slen)==-1)
	{
    	err("sendto()");
    }
    memset((resp), '\0', (RESPLEN));
}


/* signup() in server side */

void signup_serv()
{
	int p=0;
	char *token,temp2[BUFLEN],slist[BUFLEN],temp[BUFLEN];
	
	strcpy(temp,"");
	strcpy(temp2,"");
	token=strtok(buf,",");
	token= strtok (NULL,",");
	//clear();
	/* making the recived string in the database format */

	while (token != NULL)
  	{

		strcat(temp2,token);			
		token= strtok (NULL, ",");
		
		if(token!=NULL)
		{
			strcat(temp2,",");
		}
		
	}
	
	/* taking the mail field alone form the recived data */

	strcpy(temp,temp2);
	token=strtok(temp,",");
	token= strtok (NULL,",");
	token= strtok (NULL,",");
	strcpy(mail,token);
	
	/* checking whether the mail already exist in the data base */

	fp=fopen("db.csv","r");

	while(fgets(slist,BUFLEN,fp)!=NULL)/* reading from the db.csv file */
	{

		f=0;
		token=strtok(slist,",");
		token=strtok (NULL, ",");
		token=strtok (NULL, ",");
 	
		strcpy(temp,token);
	
		if(strcasecmp(temp,mail)==0)
		{
			//printf("\n%s\n",temp);
			f=1;
			break;
		}
	//	printf("\ninside - %s\n",temp2);
		strcpy(slist,"");
		strcpy(token,"");
	}
	//printf("\n%s\n",temp2);
	//printf("\n%d\n",f);
	fclose(fp);
	//clear();

	if(f!=1)
	{


		fp=fopen("db.csv","a");
		fputs(temp2,fp); /* writing all the signup details into the db.csv file */
		fprintf(fp,"\n");
		fclose(fp);
	}
}

/* canceling appointment */

int cancel_serv()
{
		int check=0;
		char temp[BUFLEN],temp1[BUFLEN],*token1,*token2,brid_str[BUFLEN],swe[BUFLEN];
		/*memset((temp), '\0', (BUFLEN));
		memset((temp1), '\0', (BUFLEN));
		memset((token1), '\0', (BUFLEN));
		memset((token2), '\0', (BUFLEN));*/
		strcpy(swe,buf);
		token1=strtok(swe,",");
		token2= strtok (NULL,",");
		
		strcpy(brid_str,token2);

		//printf("\nasda\n");
		
		//printf("%s--------->>>\n",token1);
		//printf("%s-------<<<<<<<<<<<-->>>\n",token2);
		
		fp3=fopen("appointment.csv","r+");
		if(fp3!=NULL)
		{
			strcpy(temp,"");
			puts("test");
			while(fgets(temp,1000,fp3)!=NULL)
			{
				//puts(temp);
				strcpy(temp1,temp);
				token1=strtok(temp1,",");
				strcpy(token2,token1);
				token1= strtok (NULL, ",");

				if(strcasecmp(token1,brid_str)==0)
				{
					puts("test2");
					if(temp[0]=='0')
					{
						temp[0]='1';
						check=1;
					}

					fseek(fp3,-(strlen(temp)),SEEK_CUR);
					fprintf(fp3,"%s",temp);
					break;
				}
			}
		}
		fclose(fp3);
		//printf("%d\n",check);
		if (check==1)
		{
			strcpy(resp,"Booking canceled succesfully \n");
		}
		else
		{
			strcpy(resp,"Brid doesnot exist or the appointment is already canceled..\n");
		}

		check=0;
		if (sendto(sockfd, resp, RESPLEN, 0, (struct sockaddr*)&cli_addr, slen)==-1)
		{
    		err("sendto()");
    	}
		return 0; 
}

/* login() in server side */

int login_serv()
{
	char pw[50],slist[BUFLEN];
	char *token;
	int i=0,count=0,p=0,m=0;;
		
	fp=fopen("db.csv","r");
		
	token=strtok(buf,",");
	token= strtok (NULL, ",");
	strcpy(mail,token);

	token= strtok (NULL, ",");
	strcpy(pw,token);

/* validating the user */

	while(fgets(slist,1000,fp)!=NULL)
	{
		token=strtok(slist,",");

 		while (token != NULL)
  		{
			p++;

			if(p<3)
			{
				token= strtok (NULL, ",");
				continue;
			}								
			if(strcmp(token,mail)!=0)
			{
				break;
			}
				
			token= strtok (NULL, ",");		
			token= strtok (NULL, ",");

			if(strcmp(token,pw)==0)
			{
				strcpy(email,mail);
				f=1;
				break;
			}
		}
		if(f==1)
		{
			break;
		}
	}
	fclose(fp);
	return 0;	
} 


int main(void)
{
	char * ch,chk[BUFLEN];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)/* creation of server socket*/
    {
    	err("socket cannot be created");
    }
    else
    {
    	printf("Server : Socket() successful\n");
    }
 
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
     
    if (bind(sockfd, (struct sockaddr* ) &my_addr, sizeof(my_addr))==-1) /* binding of socket*/
    {
    	err("bind");
    }
    else
    {
    	printf("Server : bind() successful\n");
    }

    while(1)
    {
		memset((buf), '\0', (BUFLEN));
		if (recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&cli_addr, &slen)==-1)
		{
            err("recvfrom()");
		}
		printf("\nRequest from ip: %s port: %d \nData: %s\n\n",inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buf);
		
		memset((chk), '\0', (BUFLEN));
		strcpy(chk,buf);
		ch = strtok(chk,",");
		pch=atoi(ch); /* converting alpba to integer */

		if(pch==0)
		{
			if(flag)
			{
    	    	flag=0;
				strcpy(resp,"Successfuly Logged out!!!!");
				if (sendto(sockfd, resp, RESPLEN, 0, (struct sockaddr*)&cli_addr, slen)==-1)
				{
    		    	err("sendto()");
    	    	}

    	    }
    	    else
    	    {
    	    	strcpy(resp,"you are not logged in!!!!");
				if (sendto(sockfd, resp, RESPLEN, 0, (struct sockaddr*)&cli_addr, slen)==-1)
				{
    		    	err("sendto()");
    	    	}	
    	    }

		}
		else if(pch ==1)
		{
			if(!flag)
			{
				login_serv();
				memset((resp), '\0', (RESPLEN));
				if(f==1)
				{	
  					flag=1;
					strcpy(resp,"Login Successfull!!!!");
					if (sendto(sockfd, resp, RESPLEN, 0, (struct sockaddr*)&cli_addr, slen)==-1)
					{
    	    			err("sendto()");
        			}	 
				}
				else
				{
					flag=0;
					strcpy(resp,"Invalid login");
					if (sendto(sockfd, resp, RESPLEN, 0, (struct sockaddr*)&cli_addr, slen)==-1)
					{
         				err("sendto()");
         			}
				}
			}
			else
			{
				strcpy(resp,"Already logged in");
				if (sendto(sockfd, resp, RESPLEN, 0, (struct sockaddr*)&cli_addr, slen)==-1)
				{
         			err("sendto()");
         		}	
			}

		}		
		else if(pch==2)
		{	
			if(flag==0)
			{	
				signup_serv();
				if(f==1)
				{
					strcpy(resp,"Mail already exist..!");
					if (sendto(sockfd, resp, RESPLEN, 0, (struct sockaddr*)&cli_addr, slen)==-1)
					{
	    				err("sendto()");
	    			}
				}
				else 
				{
					
					strcpy(resp,"Successfuly Registered..!");
					if (sendto(sockfd, resp, RESPLEN, 0, (struct sockaddr*)&cli_addr, slen)==-1)
					{
	    				err("sendto()");
	    			}	
				}
			}
			else
			{
				strcpy(resp,"You are logged in");
				if (sendto(sockfd, resp, RESPLEN, 0, (struct sockaddr*)&cli_addr, slen)==-1)
				{
         			err("sendto()");
         		}	
			}		
		}
		else if(pch==3)
		{
			
			/* booking code */
			book_serv();
			continue;	

		}	
		else if(pch==4)
		{
			if(flag==1)
			{
				cancel_serv();
				continue;
			}
			else
			{
				strcpy(resp,"You are not logged in");
				if (sendto(sockfd, resp, RESPLEN, 0, (struct sockaddr*)&cli_addr, slen)==-1)
				{
         			err("sendto()");
         		}	
			}	
		}	
		else
		{	
			printf("\nno operation till now..!\n");
		}
    }
    close(sockfd);
    return 0;
}
