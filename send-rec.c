#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>
#include<string.h>

int main(int argc, char* argv[]){
    int fd[2];
    if(pipe(fd) == -1){
        exit(1);
    }

    int pid = fork();
    if(pid < 0){
        exit(2);
    }

    if(pid == 0){
        // Child process
        close(fd[0]);
        char str[200];
        printf("Input string: ");
        fgets(str, 200, stdin);
        str[strlen(str) - 1] = '\0';

        int n = strlen(str);
        if(write(fd[1], &n, sizeof(int)) == -1){
            exit(3);
        }
        if(write(fd[1], str, sizeof(char) * n) == -1){
            exit(4);
        }
        close(fd[1]);
    } else {
        // Parent process
        close(fd[1]);
        int n;
        if(read(fd[0], &n, sizeof(int)) < 0){
            exit(5);
        }
        char str[n + 1];
        if(read(fd[0], str, sizeof(char) * n) < 0){
            exit(6);
        }
        str[n] = '\0';
        printf("Received string : %s\n", str);
    }

    return 0;
}
