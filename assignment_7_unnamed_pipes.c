#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int pipe1[2], pipe2[2];
    pid_t pid;
    char pathname[200];
    char buffer[500];

    pipe(pipe1);  // Create parent-to-child pipe
    pipe(pipe2);  // Create child-to-parent pipe

    pid = fork();  // Create child process

    if (pid < 0)
    {
        printf("Fork failed\n");
        exit(1);
    }

    if (pid > 0)
    {
        close(pipe1[0]);  // Close unused read end
        close(pipe2[1]);  // Close unused write end

        printf("Enter file pathname: ");
        fgets(pathname, sizeof(pathname), stdin);
        pathname[strcspn(pathname, "\n")] = '\0';

        write(pipe1[1], pathname, strlen(pathname) + 1);
        close(pipe1[1]);  // Close pipe after sending pathname

        printf("\nContents of the file:\n");

        int n;
        while ((n = read(pipe2[0], buffer, sizeof(buffer))) > 0)
            write(STDOUT_FILENO, buffer, n);  // Display received contents

        close(pipe2[0]);  // Close pipe after reading
        wait(NULL);  // Wait for child process to finish
    }
    else
    {
        close(pipe1[1]);  // Close unused write end
        close(pipe2[0]);  // Close unused read end

        read(pipe1[0], pathname, sizeof(pathname));
        close(pipe1[0]);  // Close pipe after receiving pathname

        FILE *fp = fopen(pathname, "r");

        if (fp == NULL)
        {
            char *msg = "Unable to open file\n";
            write(pipe2[1], msg, strlen(msg));
            close(pipe2[1]);
            exit(1);
        }

        int n;
        while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0)
            write(pipe2[1], buffer, n);  // Send file contents to parent

        fclose(fp);  // Close file after reading
        close(pipe2[1]);  // Close pipe after sending contents
        exit(0);  // Terminate child process
    }

    return 0;
}
