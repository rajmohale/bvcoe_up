#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Compile with:  gcc fork.c -DZOMBIE   or   gcc fork.c -DORPHAN  */
#if !defined(ZOMBIE) && !defined(ORPHAN)
#define ZOMBIE
#endif

/* Prints the column header line (contains "PID") plus every a.out line */
#define PS "ps -elf | sed -e '1s/F S UID/       /' -e '1s/PPID.*/PPID  CPU SCHEDULING INFO/' | grep -E 'PID|a.out'"
#define LINE "\n----------------------------------\n\n"

void quicksort(int [10], int, int);

int main()
{
    int pid, ppid, a[10], size, i;
    char cmd[64];

    printf("\nPARENT: My process-id is %d.", getpid());
    printf("\n\nEnter the number of elements in the array: ");
    scanf("%d", &size);
    printf("Enter %d elements: ", size);
    for (i = 0; i < size; i++)
        scanf("%d", &a[i]);

    printf("\nForking now!\n");
    pid = fork();

    if (pid == 0)
    {
        /* ---------------- CHILD ---------------- */
        ppid = getppid();                       /* original parent */
        printf("\nCHILD: My process-id is %d.", getpid());
        printf("\nCHILD: My parent's process-id is %d.", ppid);
        quicksort(a, 0, size - 1);
        printf("\nSorted elements: ");
        for (i = 0; i < size; i++)
            printf("%d ", a[i]);

#ifdef ORPHAN
        printf("\nCHILD: I am sleeping now (my parent will die meanwhile).\n");
        fflush(stdout);
        sleep(10);

        printf(LINE);
        printf("CHILD: I woke up. My old parent %d is gone, so I was adopted.", ppid);
        printf("\nCHILD: My NEW parent's process-id is %d.\n", getppid());
        printf("CHILD: My new parent is:\n");
        fflush(stdout);
        sprintf(cmd, "ps -p %d -o pid,comm", getppid());
        system(cmd);                            /* shows new parent's PID and name */
        printf("\nCHILD: Listing (see my PPID column):\n");
        fflush(stdout);
        system(PS);
        printf("\nCHILD: I am dying now.\n");
#else /* ZOMBIE */
        printf("\nCHILD: I am still alive. Running ps now.\n");
        fflush(stdout);
        sleep(1);                               /* let parent print its lines first */
        system(PS);                             /* listing 1: child is alive (S) */
        printf("\nCHILD: I am dying now.\n");
        fflush(stdout);
#endif
        exit(0);
    }
    else if (pid > 0)
    {
        /* ---------------- PARENT ---------------- */
        system("wait");                         /* does NOT really wait for the child */
        printf("\nPARENT: I am back.");
        printf("\nPARENT: My process-id is %d.", getpid());
        printf("\nPARENT: My child's process-id is %d.", pid);
        quicksort(a, 0, size - 1);
        printf("\nSorted elements: ");
        for (i = 0; i < size; i++)
            printf("%d ", a[i]);
        printf(LINE);

#ifdef ORPHAN
        printf("PARENT: Listing (my child is still sleeping):\n");
        fflush(stdout);
        system(PS);                             /* listing 1: child's PPID = me */
        printf("\nPARENT: I am dying now. My child will be left behind.\n");
        exit(0);                                /* parent dies first -> child orphaned */
#else /* ZOMBIE */
        printf("PARENT: I am sleeping now (I will not collect my child).");
        sleep(10);                              /* child dies, nobody reaps it */
        printf(LINE);
        printf("PARENT: Listing (my child is dead but still listed):\n");
        fflush(stdout);
        system(PS);                             /* listing 2: child is Z <defunct> */
        printf("\nPARENT: I am dying now.\n");
#endif
    }
    return 0;
}

void quicksort(int a[10], int first, int last)
{
    int pivot, j, temp, i;

    if (first < last)
    {
        pivot = first;
        i = first;
        j = last;

        while (i < j)
        {
            while (a[i] <= a[pivot] && i < last)
                i++;
            while (a[j] > a[pivot])
                j--;
            if (i < j)
            {
                temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
        }

        temp = a[pivot];
        a[pivot] = a[j];
        a[j] = temp;

        quicksort(a, first, j - 1);
        quicksort(a, j + 1, last);
    }
}
