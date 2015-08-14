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

int cancel_cli()
{
		
		char brid_str[50];
		int brid;

		printf("\n Enter your brid (Booking reference Id) :  ");
		scanf("%d",&brid);

		tostring(brid_str,brid);
		strcat(resp,"4");
		strcat(resp,",");
		strcat(resp,brid_str);
		printf("\n");


        if (sendto(sockfd, resp, BUFLEN, 0, (struct sockaddr*)&serv_addr, slen)==-1)
            {
                err("sendto()");
            }
            /* receiving data from server */
            
            if (recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&serv_addr, &slen)==-1)
            {
                err("recvfrom()");
            }
            printf("\n%s\n\n", buf);       
return 0;
}