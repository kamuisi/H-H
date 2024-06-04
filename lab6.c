#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#define MAX_LINE 80 /* The maximum length command */
static pid_t pid;
static int child_exit;
void signal_handler(int sig)
{
    if (child_exit == 1)
    {
        printf("\nEnding process...\n");
        if(kill(pid, SIGKILL) == -1)
        {
            printf("Cannot kill process\n");
        }
    }
    else
    {
        printf("\nHave no child process to end\n");
        printf("it007sh> ");
        fflush(stdout);
    }
}

int main(void)
{
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    char *pipe_args[MAX_LINE / 2 + 1];
    char history[10][MAX_LINE];
    char input[MAX_LINE];
    int history_count = 0;
    int k = 0;
    int default_out = dup(1);
    int default_in = dup(0);
    int should_run = 1; /* flag to determine when to exit program
                         */
    signal(SIGINT, signal_handler);
    while (should_run)
    {
        child_exit = 0;
        printf("it007sh> ");
        fflush(stdout);
        if (fgets(input, MAX_LINE, stdin) == NULL)
        {
            continue;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
        {
            continue;
        }

        if (strcmp(input, "HF") == 0)
        {
            k--;
            if (k == -1)
            {
                k = history_count - 1;
            }
            printf("%s\n", history[k]);
            continue;
        }

        if (strcmp(input, "exit") == 0)
        {
            should_run = 0;
            continue;
        }
        if (history_count == 10)
        {
            for (int j = 0; j < 9; j++)
            {
                strcpy(history[j], history[j + 1]);
            }
            strcpy(history[9], input);
        }
        else
        {
            strcpy(history[history_count], input);
            history_count++;
        }

        int i = 0;
        args[0] = strtok(input, " ");
        while (args[i] != NULL && i < MAX_LINE / 2)
        {
            i++;
            args[i] = strtok(NULL, " ");
        }

        i = 0;
        int pipe_flags = 0;
        while (args[i] != NULL)
        {
            if (strcmp(args[i], "<") == 0)
            {
                int file = open(args[i + 1], O_RDONLY);
                if (file == -1)
                {
                    perror("Open file error");
                    break;
                }
                dup2(file, 0);
                close(file);
                args[i] = '\0';
                args[i + 1] = '\0';
                break;
            }
            else if (strcmp(args[i], ">") == 0)
            {
                int file = open(args[i + 1], O_WRONLY | O_CREAT, 0777);
                if (file == -1)
                {
                    perror("Open file error");
                    break;
                }
                dup2(file, 1);
                close(file);
                args[i] = '\0';
                args[i + 1] = '\0';
                break;
            }
            else if (strcmp(args[i], "|") == 0)
            {
                pipe_flags = 1;
                args[i] = '\0';
                i++;
                int j = 0;
                while (args[i] != NULL)
                {
                    pipe_args[j] = args[i];
                    args[i] = '\0';
                    j++;
                    i++;
                }
                pipe_args[j] = '\0';
                break;
            }
            i++;
        }

        pid = fork();
        if (pid == 0)
        {
            if (pipe_flags)
            {
                int pipe_fd[2];
                pipe(pipe_fd);
                pid_t pipe_pid = fork();
                if (pipe_pid == 0)
                {
                    close(pipe_fd[0]);
                    dup2(pipe_fd[1], 1);
                    close(pipe_fd[1]);
                    if (execvp(args[0], args) < 0)
                    {
                        printf("Wrong command\n");
                        exit(1);
                    }
                }
                else
                {
                    wait(NULL);
                    close(pipe_fd[1]);
                    dup2(pipe_fd[0], 0);
                    close(pipe_fd[0]);
                    if (execvp(pipe_args[0], pipe_args) < 0)
                    {
                        printf("Wrong command\n");
                        exit(1);
                    }
                }
            }
            else
            {
                if (execvp(args[0], args) < 0)
                {
                    printf("Wrong command\n");
                    exit(1);
                }
            }
        }
        else
        {
            child_exit = 1;
            wait(NULL);
            dup2(default_in, 0);
            dup2(default_out, 1);
        }
    }
    close(default_in);
    close(default_out);
    return 0;
}