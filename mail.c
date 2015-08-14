int valid_mail(char x[])
{
    int count=0;
    int countp=0;
    int k=0;

    int i,j;
    int t,t2;
    
    for(i=0 ; i<strlen(x) ; i++)
    {
        if(x[i]=='@')
        {
            count++;
            t=i;
        }
    }
    if(count==1)
    {
        for(j=0; j<t ; j++)
        {
            if(j==0)
            {
                if((x[j]=='_' || x[j]=='.')||isdigit(x[j]))
                {
                    k=-1;
                    printf("\nInvalid e-mail\n");
                    break;
                }
            }
            else
            {
                if(!(x[j]=='_' || x[j]=='.' || isalpha(x[j]) || isdigit(x[j])))
                {
                    k=-1;
                    printf("\nInvalid e-mail\n");
                    break;
                }
            }
        }
        if(k==0)
        {
            for(i=t; i<strlen(x) ; i++)
            {
                if(x[i]=='.')
                {
                    t2=i;
                    if(t+1!=t2)
                    {                       
                        countp++;
                    }

                }
            }

            if(countp==1)
            {
                for(i=t+1; i<t2; i++)
                {
                    if(!(isalpha(x[i])))
                    {
                        k=-1;
                        printf("\nInvalid e-mail\n");
                        break;
                    }
                    else
                    {
                        k=1;
                    }
                }
                if(k==1)
                {
                    int j=t2+4;
                    if(x[j]!='\0')
                    {
                        k=-1;
                        printf("\nInvalid e-mail\n");
                    }
                    else
                        k=0;
                }
            }
            else
            {
                k=-1;
                printf("\nInvalid e-mail\n");
            }
        }
    }
    else
    {
        k=-1;
        printf("\nInvalid e-mail\n");
    }
    return k;
}