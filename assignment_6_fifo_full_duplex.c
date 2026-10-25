#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

int main()
{
    int fd1, fd2;
    char sentence[500], result[500];

    mkfifo(FIFO1, 0666);  // Create named pipes
    mkfifo(FIFO2, 0666);

    pid_t pid = fork();  // Create child process

    if (pid < 0)
    {
        printf("Fork failed\n");
        exit(1);
    }

    if (pid > 0)
    {
        printf("PROCESS 1\n");
        printf("Enter a sentence: ");
        fgets(sentence, sizeof(sentence), stdin);
        sentence[strcspn(sentence, "\n")] = '\0';

        fd1 = open(FIFO1, O_WRONLY);
        write(fd1, sentence, strlen(sentence) + 1);
        close(fd1);  // Close FIFO after writing

        fd2 = open(FIFO2, O_RDONLY);
        read(fd2, result, sizeof(result));
        close(fd2);  // Close FIFO after reading

        printf("\nResult received from Process 2:\n");
        printf("%s\n", result);

        wait(NULL);  // Wait for child process to finish
        unlink(FIFO1);  // Remove named pipes from the filesystem
        unlink(FIFO2);
    }
    else
    {
        int characters = 0, words = 0, lines = 1;
        int in_word = 0;

        fd1 = open(FIFO1, O_RDONLY);
        read(fd1, sentence, sizeof(sentence));
        close(fd1);

        for (int i = 0; sentence[i] != '\0'; i++)
        {
            characters++;

            if (sentence[i] == '\n')
                lines++;

            if (sentence[i] == ' ' || sentence[i] == '\t')
                in_word = 0;
            else if (in_word == 0)
            {
                words++;
                in_word = 1;
            }
        }

        FILE *fp = fopen("output.txt", "w");

        if (fp == NULL)
        {
            printf("Unable to create file\n");
            exit(1);
        }

        fprintf(fp, "Number of Characters = %d\n", characters);
        fprintf(fp, "Number of Words = %d\n", words);
        fprintf(fp, "Number of Lines = %d\n", lines);
        fclose(fp);  // Close output file

        sprintf(result,
                "Characters = %d\nWords = %d\nLines = %d\nOutput also stored in output.txt",
                characters, words, lines);

        fd2 = open(FIFO2, O_WRONLY);
        write(fd2, result, strlen(result) + 1);
        close(fd2);

        exit(0);  // Terminate child process
    }

    return 0;
}
