#include <stdio.h>

void main()
{
    int n, m, i, j, instance, k = 0, count = 0, temp = 0;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter the number of resources: ");
    scanf("%d", &m);

    int alloc[n][m];
    int max[n][m];
    int avail[m], need[n][m], completed[n];

    for(i = 0; i < n; i++)
    {
        completed[i] = 0;
    }

    printf("Enter the available resources for processes:\n");

    for(i = 0; i < m; i++)
    {
        printf("Resources[%d]:", i);
        scanf("%d", &instance);
        avail[i] = instance;
    }

    printf("\n------------------------------------------\n");

    printf("Enter the max matrix for process:\n");

    for(i = 0; i < n; i++)
    {
        printf("\nFor Process[%d]\n", i);

        for(j = 0; j < m; j++)
        {
            printf("Resource[%d]:", j);
            scanf("%d", &instance);

            max[i][j] = instance;
        }
    }

    printf("\n------------------------------------------\n");

    printf("Enter the allocation matrix:\n");

    for(i = 0; i < n; i++)
    {
        printf("\nFor Process[%d]\n", i);

        for(j = 0; j < m; j++)
        {
            printf("Resource[%d]:", j);
            scanf("%d", &instance);

            alloc[i][j] = instance;
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    printf("\n------------------------------------------\n");

    printf("Request Algorithm\n");

    printf("Enter the process number that is requesting resources:");
    int rp;
    scanf("%d", &rp);

    printf("Enter the resources requested by the process:\n");

    int rm[m];

    for(i = 0; i < m; i++)
    {
        printf("Resource[%d]:", i);
        scanf("%d", &instance);

        rm[i] = instance;
    }

    /* Check if the request can be granted */

    int canGrant = 1;

    for(i = 0; i < m; i++)
    {
        if(rm[i] > need[rp][i])
        {
            canGrant = 0;
            break;
        }
    }

    if(canGrant)
    {
        /* Check if the system has enough resources
           to grant the request */

        for(i = 0; i < m; i++)
        {
            if(rm[i] > avail[i])
            {
                canGrant = 0;
                break;
            }
        }
    }

    if(canGrant)
    {
        /* Grant the request */

        for(i = 0; i < m; i++)
        {
            avail[i] -= rm[i];
            alloc[rp][i] += rm[i];
            need[rp][i] -= rm[i];
        }

        printf("Request granted\n");
    }
    else
    {
        printf("Request cannot be granted\n");
    }

    /* Calculate new safe sequence */

    printf("\nNew Safe Sequence is:");

    count = 0;
    temp = 0;

    for(i = 0; i < n; i++)
    {
        completed[i] = 0;
    }

    while(count != n)
    {
        temp = count;

        for(i = 0; i < n; i++)
        {
            int flag = 1;

            for(j = 0; j < m; j++)
            {
                if(need[i][j] > avail[j])
                {
                    flag = 0;
                    break;
                }
            }

            if(flag && completed[i] == 0)
            {
                printf("P[%d] | ", i);

                completed[i] = 1;

                for(j = 0; j < m; j++)
                {
                    avail[j] = avail[j] + alloc[i][j];
                }

                count++;
            }
        }

        if(count == temp)
        {
            break;
        }
    }

    for(i = 0; i < n; i++)
    {
        if(completed[i] != 1)
        {
            printf("\nP[%d] not able to allocate", i);
        }
    }

    printf("\n");
}
