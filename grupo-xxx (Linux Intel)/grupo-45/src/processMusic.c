#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../headers/processMusic.h"


int processMusic(char* inputFileName, char* outputFileName, char** args, int numberArgs){
    char* exec_args[10][10];
    char* string;
    int i, c, a;
    int number_of_commands;
    char* command;
    int pid;
    int status;
    int fdInput;
    int fdOutput;
    int ret;

    for(a=0;a<numberArgs;a++){
        command = strdup(args[a]);
        /*
         * Fazer o parsing da string
         * Colocar num array os vários comandos a executar
         * */
        string = strtok(command," ");
        i = 0;
        while(string != NULL){
            exec_args[a][i] = string;
            string = strtok(NULL," ");
            i++;
        }
        exec_args[a][i] = NULL;
    }

    number_of_commands = numberArgs;

    /*
     * Reproduz o comportamento da system() no caso de ter apenas um executável
     * */
    if(number_of_commands == 1){
        fdInput = open(inputFileName,O_RDONLY,0666);

        if(fdInput == -1){
            perror("Erro na abertura do ficheiro de audio de input:");
            return -1;
        }
        dup2(fdInput,0);
        close(fdInput);

        fdOutput = open(outputFileName,O_WRONLY | O_CREAT,0666);
        if(fdOutput == -1){
            perror("Erro na abertura de um ficheiro de Output:");
            return -1;
        }
        dup2(fdOutput,1);
        close(fdOutput);

        if( (pid = fork()) == 0 ){
            ret = execvp(exec_args[0][0],exec_args[0]);
            perror("processo filho errou: ");
            _exit(ret);
        }
        else{
            wait(&status);
        }
        return 0;
    }

    /*
     * Vamos ter N comandos, por isso temos N-1 pipes e cada pipe tem 2 descritores(leitura e escrita)
     * */
    int pipes[number_of_commands-1][2];

    /*
     * Para cada comando tenho de criar um processo filho
     * iterar sobre os comandos a executar
     * */
    for( c=0; c<number_of_commands; c++ ){
        if( c == 0 ){
            /*
             * Criar os pipe necessário para fazer a comunicação entre o comando c e o comando c+1
             * */
            pipe(pipes[c]);

            switch( ( pid = fork() ) ){
                case -1:
                    perror("fork");
                    return -1;
                case 0:
                    /*
                     * O primeiro executável vai receber como input o ficheiro de audio.
                     * */
                    fdInput = open(inputFileName,O_RDONLY,0666);

                    if(fdInput == -1){
                        perror("Erro na abertura do ficheiro de audio de input:");
                        return -1;
                    }
                    dup2(fdInput,0);
                    close(fdInput);
                    close(pipes[c][0]);
                    dup2(pipes[c][1],1);
                    close(pipes[c][1]);
                    execvp(exec_args[c][0],exec_args[c]);
                    _exit(0);
                default:
                    close(pipes[c][1]);
            }
        }
        else if( c == number_of_commands-1 ){
            switch( ( pid = fork() ) ){
                case -1:
                    perror("fork");
                    return -1;
                case 0:
                    dup2(pipes[c-1][0],0);
                    close(pipes[c-1][0]);

                    fdOutput = open(outputFileName,O_WRONLY | O_CREAT,0666);
                    if(fdOutput == -1){
                        perror("Erro na abertura de um ficheiro de Output:");
                        return -1;
                    }
                    dup2(fdOutput,1);
                    close(fdOutput);

                    execvp(exec_args[c][0],exec_args[c]);
                    _exit(0);
                default:
                    close(pipes[c-1][0]);
            }

        }
        else{
            pipe(pipes[c]);

            switch( ( pid = fork() ) ){
                case -1:
                    perror("fork");
                    return -1;
                case 0:
                    dup2(pipes[c-1][0],0);
                    close(pipes[c-1][0]);
                    close(pipes[c][0]);
                    dup2(pipes[c][1],1);
                    close(pipes[c][1]);
                    execvp(exec_args[c][0],exec_args[c]);
                    _exit(0);
                default:
                    close(pipes[c-1][0]);
                    close(pipes[c][1]);
            }

        }
    }
    for( i=0; i<number_of_commands ;i++){
        wait(&status);
    }

    return 0;
}
