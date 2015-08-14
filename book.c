#include <stdio.h>
#include <string.h>
#include <math.h>

int book_cli()
{
	char drdetails[BUFLEN],token2[BUFLEN],docname[BUFLEN],email[50],temp_doc[BUFLEN],buf[BUFLEN];
	char *token,s[500],date[50],s1[500],ch[2];

	int l,i=2,f=0,brid;

	do
	{
		clear();
		memset((buf), '\0', (BUFLEN));
		memset((docname), '\0', (BUFLEN));
		strcat(docname,"3");
		strcat(docname,",");

		printf("\n Enter Doctor's name :  ");
		scanf("%s",temp_doc);

		strcat(docname,temp_doc);
		memset((buf), '\0', (BUFLEN));
		memset((buf), '\0', (BUFLEN));

		if (sendto(sockfd, docname,50, 0, (struct sockaddr*)&serv_addr, slen)==-1)
		{
	      	err("sendto()");
		}	

		memset((buf), '\0', (BUFLEN));
		memset((buf), '\0', (BUFLEN));

		if (recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&serv_addr, &slen)==-1)
		{
	        err("recvfrom()");
		}

		printf("Doctor Name\t\tGender\t\tSpecialisation\t\tQalification\tMobile\t\tAvailablity/day\tSlots Available\n");
		printf("-----------\t\t------\t\t--------------\t\t------------\t------\t\t---------------\t---------------\n\n");
	    printf("\n%s",buf);

	}while(strcasecmp(buf,"No match found...!")==0);

	memset((buf), '\0', (BUFLEN));
	memset((docname), '\0', (BUFLEN));

	clear();

	printf("\nEnter full name of Doctor:");
	scanf("%[^\n]*c",docname);	
	printf("\nDoctor:%s\n",docname);

	
	if (sendto(sockfd, docname,100, 0, (struct sockaddr*)&serv_addr, slen)==-1)
	{
      	err("sendto()");
	}

	printf("Date\t\t\tStatus\t\t\tTime\t\t\tRemaining slots\n");
	printf("----\t\t\t------\t\t\t----\t\t\t---------------\n");

	if (recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&serv_addr, &slen)==-1)
	{
        err("recvfrom()");
	}
    printf("\n%s",buf);
	memset((buf), '\0', (BUFLEN));

	printf("\nEnter date to book(dd/mm/yyyy):");
	scanf("%s",date);
	strcpy(buf,date);

	if (sendto(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&serv_addr, slen)==-1)
	{
            err("sendto()");
	}
	
	if (recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&serv_addr, &slen)==-1)
	{
        err("recvfrom()");
	}
    printf("\n%s\n",buf);
	memset((buf), '\0', (BUFLEN));

    return 0;
}
