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
    /*
     * Este array tem os nomes dos comandos a executar
     * Estamos a assumir numero maximo de argumentos (poderia ser melhorado com realloc)
     * */
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

    /*
     * Separar os argumentos e os comandos por um array com 2 dimensões: dimensão 1 -> comando; dimensão 2 -> argumentos do comando
     * */


    for(a=0;a<numberArgs;a++){
        /*
         * Copiar a string com os comandos recebida por parâmetro nesta função
         * */
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

        /*
         * No final preciso de colocar o NULL pois é assim que esta formatado o parâmetro do execvp com o s argumentos do executável
         * */
        exec_args[a][i] = NULL;
    }

    number_of_commands = numberArgs;

    /*
     * Reproduz o comportamento da system() no caso de ter apenas um executável
     * */

    if(number_of_commands == 1){
        /*
         * Para ser mais eficiente: se não sei o número de arrgumentos do executável que pretendo correr então deveria alocar memória das strings
         * Usar realloc
         * Neste caso defino como sendo 20 o número máximo de argumentos
         * Este array tem todos os argumentos do executavel que pretendo correr este é precisamente um dos argumentos a passar no execv(execvp), por isso aqui estou já a dizer que vou usar o execv (ou execvp) para executar o programa
         * */
        fdInput = open(inputFileName,O_RDONLY,0666);

        if(fdInput == -1){
            perror("Erro na abertura do ficheiro de audio de input:");
            return -1;
        }
        dup2(fdInput,0);
        close(fdInput);

        /*
         * Redirecionar o stdout para o ficheiro de output
         * */
        fdOutput = open(outputFileName,O_WRONLY | O_CREAT,0666);
        if(fdOutput == -1){
            perror("Erro na abertura de um ficheiro de Output:");
            return -1;
        }
        dup2(fdOutput,1);
        close(fdOutput);

        /*
         * Crio um filho para executar o executável que pretendo correr e assim o processo pai poder retornar para a função que chamou a mySystem e continuar a executar o codigo
         * */
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
        //primeiro comand(não redir stdin)
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
                     * O executável vai receber como input o ficheiro de audio.
                     * Necessário fazer o redirecionamento
                     * */
                    fdInput = open(inputFileName,O_RDONLY,0666);

                    if(fdInput == -1){
                        perror("Erro na abertura do ficheiro de audio de input:");
                        return -1;
                    }
                    dup2(fdInput,0);
                    close(fdInput);

                    close(pipes[c][0]);
                    /*
                     * Se fizesse já o execlp do comando o resultado da execução do comando seria impressa no stdout
                     * mas pretendemos que este resultado seja escrito no pipe, por isso temos de fazer o redirecionamento
                     * do stdout para a extremidade de escrita do pipe
                     * (ou seja, tudo o que era escrito no ecrã vai passar a ser escrito na extremidade de escrita do pipe)
                     * */
                    dup2(pipes[c][1],1);
                    /*
                     * Já fizemos o redirectionamento por isso podemos apagar o descriptor p[1]
                     * */
                    close(pipes[c][1]);
                    execvp(exec_args[c][0],exec_args[c]);
                    _exit(0);
                default:
                    /*
                     * O pai não vai usar a extremidade de leitura do pipe c (que é zero porque estamos dentro do if(c==0)) por isso temos de fechar essa parte
                     * Não deveremos fechar a extremidade de leitura do pipe no pai neste momento pois caso o fizessemos e o comando executado escrevesse na extremidade de escrita
                     * como as extremidades de leitura estariam todas fechadas receberíamos um sinal SIGPIPE
                     * */
                    close(pipes[c][1]);
                    /*
                     * O processo pai continua para criar as ligações entre os comandos restantes
                     * */
            }
        }
        else if( c == number_of_commands-1 ){
            /*
             * Não é necessário criar pois este comando apenas interage com 1 pipe que já foi criado (pipe(number_of_commands-2))
             * ou seja, o ultimo pipe da sequencia de pipes
             * */
            switch( ( pid = fork() ) ){
                case -1:
                    perror("fork");
                    return -1;
                case 0:
                    /*
                     * O filho só lê no último pipe por isso podemos redicionar o stdin para a extremidade de leitura do pipe
                     * */
                    dup2(pipes[c-1][0],0);
                    close(pipes[c-1][0]);
                    /*
                     * Redirecionar o stdout para o ficheiro de output
                     * */
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
                    /*
                     * Fechar a extremidade de leitura que estava aberta no pai desde a última iteração
                     * */
                    close(pipes[c-1][0]);
                    /*
                     * O processo pai já não vai criar mais processos filhos nem pipes
                     * */
            }

        }
        else{
            pipe(pipes[c]);

            switch( ( pid = fork() ) ){
                case -1:
                    perror("fork");
                    return -1;
                case 0:
                    /*
                     * Não preciso de fechar a extremidade de escrita do pipe c-1 pois esta operação já foi feita na iteração anterior no código do pai
                     * De lembrar que ao fazer fork é copiado o estado do pai para o filho
                     * */

                    /*
                     * Necessário redirecionar o stdin para a extremidade de leitura do pipe c-1 (pipe atrás desse processo que liga com o stdin do processo filho)
                     * */
                    dup2(pipes[c-1][0],0);
                    close(pipes[c-1][0]);
                    /*
                     * Também não preciso da extremidade de leitura do pipe c aberta
                     * */
                    close(pipes[c][0]);
                    /*
                     * Necessário redirecionar o stdout para a extremidade de escrita do pipe c
                     * */
                    dup2(pipes[c][1],1);
                    close(pipes[c][1]);
                    execvp(exec_args[c][0],exec_args[c]);
                    _exit(0);
                default:
                    /*
                     * Fechar a extremidade de leitura do pipe c-1, que não tinhamos fechado na iteração anterior.
                     * Só fechamos aqui pois o filho já redirecionou a extremidade de leitura do pipe e então já não há possibilidade de causar SIGPIPE
                     * */
                    close(pipes[c-1][0]);
                    /*
                     * Fechar a extremidade de escrita do pipe pois já temos esta extremidade aberta no processo filho que acabamos de criar
                     * */
                    close(pipes[c][1]);
                    /*
                     * O processo pai continua para criar as ligações entre os comandos restantes
                     * */
            }

        }
    }
    /*
     * Esperar que os filhos terminem antes de terminar o programa
     * */
    for( i=0; i<number_of_commands ;i++){
        wait(&status);
    }

    return 0;
}
