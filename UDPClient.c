#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>

#define BUFLEN 512
#define STRLEN 100
#define LOGLEN 100
#define PORT 9931
char buf[BUFLEN] ,str[STRLEN],resp[BUFLEN]; 
struct sockaddr_in serv_addr;
int sockfd,slen=sizeof(serv_addr);

void err(char *s)
{
    perror(s);
    exit(1);
}
void clear (void)
{    
	while ( getchar() != '\n' );
}
#include "help.c"
#include "loginfin.c"
#include "signup.c"
#include "book.c"
#include "cancel.c"
 
int main(int argc, char** argv)
{
    int  i,a;
	char log[LOGLEN]; 
	
    if(argc != 2)/* Checking the number of arguments */
    {
      printf("Usage : %s <Server-IP>\n",argv[0]);
      exit(0);
    }

    /* Client socket creation 
	 * setting the ip family
	 * specifying the type of datagram
	 * specifying that UDP protocol is used
    */
     
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    {
        err("socket");
 	}
 	
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if (inet_aton(argv[1], &serv_addr.sin_addr)==0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
 	hdr();
    while(1)
    {
    	getchar();
		printf("\n$>");
		//getch();  
        memset((str), '\0', (STRLEN));
		scanf("%[^\n]",str);
		if((strcasecmp(str,"login"))==0)
		{
			strcpy(log,"");	
			login(log); /* calling login() from loginfin.c */
			strcpy(str,"");
			strcpy(buf,"");	

			/* Sending login data to the server */

 			if (sendto(sockfd, log, LOGLEN, 0, (struct sockaddr*)&serv_addr, slen)==-1)
			{
      			err("sendto()");
			}

			/* Receiving data from the server */

			if (recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&serv_addr, &slen)==-1)
			{
            	err("recvfrom()");
            }
        	//printf("\nReceived packet from ip: %s port: %d\nData: %s\n\n",inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port), buf);        	
			printf("%s",buf);
			continue;	

		}	
		else if((strcasecmp(str,"signup"))==0)
		{
			//printf("signup");
			strcpy(buf,"");	
			signup(log); /* calling signup() from signup.c */

			/* sending signup details to server */
			
			if (sendto(sockfd, log, LOGLEN, 0, (struct sockaddr*)&serv_addr, slen)==-1)
			{
      			err("sendto()");
			}
            /* receiving data from server */
            
			if (recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&serv_addr, &slen)==-1)
			{
            	err("recvfrom()");
            }
        	//printf("\nReceived packet from ip: %s port: %d\nData: %s\n\n",inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port), buf); 
        	printf("%s",buf);       	
			continue;	
		}
		else if((strcasecmp(str,"help"))==0)
		{
			hlp();/* calling hlp() from help.c */
			continue;	
		}
		else if((strcasecmp(str,"logout"))==0)
		{
			/* logout code */
			strcpy(buf,"");
			strcat(buf,"0");
			if (sendto(sockfd,buf, BUFLEN, 0, (struct sockaddr*)&serv_addr, slen)==-1)
			{
      			err("sendto()");
			}
			if (recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&serv_addr, &slen)==-1)
			{
            	err("recvfrom()");
            }
        	//printf("\nReceived packet from ip: %s port: %d\nData: %s\n\n",inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port), buf);
        	printf("%s",buf);        	
			continue;	
		}
		else if((strcasecmp(str,"book"))==0)
		{
			book_cli();
			memset((str), '\0', (STRLEN));
			continue;	
		}
		else if((strcasecmp(str,"cancel"))==0)
		{
			
			memset((buf), '\0', (BUFLEN));
			/* sending signup details to server */
			cancel_cli();	
			memset((str), '\0', (STRLEN));
			continue;	
		}
		else
		{
			printf("\ncommand not found\n");
			continue;	
		}

    }
 
    close(sockfd);
    return 0;

}
 
