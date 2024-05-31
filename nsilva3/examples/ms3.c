// minish - v3 - change date for sleep

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 1024

void
prompt(char *ps) {
    // ps is the prompt string
    fprintf(stderr, "(%s) ^D to exit > ", ps);
}

int
main(int argc, char *argv[]) {
    char line[MAXLINE];
    char *progname = argv[0];

    for (;;) {
        prompt(progname);
        if (fgets(line, MAXLINE, stdin) == NULL) {  // EOF
            break;
        }
        fprintf(stderr, "Will execute command %s", line);

        // ============== NEW CODE HERE ==============
        if (strncmp(line, "sleep", 5) == 0) { 
            // very bad technique, only for demo purposes
            // will fork a child to run date command

            pid_t pid;                          // process ID: an unsigned integer type
            int wait_status;                    // wait status will be filled by waitpid syscall

            fprintf(stderr, "Will fork sleep command (5 sec sleep) \n");

            if ((pid = fork()) < 0) {           // fork error, i.e. too many processes
                error(0, errno, "fork error\n"); // will fprintf the error and go on
            }

            else if (pid == 0) {                 // child process
                // ============== NEW CODE HERE ==============
                execlp("sleep", "sleep", "5", NULL);    // if successful, child will go on with new executable
                error(EXIT_FAILURE, errno, "execvp error\n"); // if exec not successful, just exit child
            }

            else {                               // pid > 0: parent (shell) process
                waitpid(pid, &wait_status, 0);
                fprintf(stderr, "Ended sleep child process\n");

                // do something with wait_status
            }
        }
    }

    fputc('\n', stderr);
    fprintf(stderr, "Exiting %s ...\n", progname);
    exit(EXIT_SUCCESS);
}