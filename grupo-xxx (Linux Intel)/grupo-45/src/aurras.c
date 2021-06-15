#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../headers/request.h"
#include "../headers/answer.h"

#define FIFOSERVERCLIENTS "/tmp/fifo"


int main(int argc,char ** args){

    setbuf(stdout,NULL);
    int pipeAnswer;
    int pipeRequest;
    char pipeClient[30];
    pid_t pidProcess = getpid();

    sprintf(pipeClient,"%s%d",FIFOSERVERCLIENTS,pidProcess);


    if(mkfifo(pipeClient, 0666) == -1){
        perror("pipe de resposta do servidor ao cliente");

    }

    /*
     * Criar o request para enviar ao servidor
     * */
    Request request = initRequest(argc,args,pidProcess);

    /*
     * O serviço pedido não é válido
     * */
    if(!request){
        printf("Os argumentos não são válidos\n");
        return -1;
    }

    /*
     * Mostrar ao cliente o serviço que acabou de pedir
     * */
    pipeRequest = open(FIFOSERVERCLIENTS, O_WRONLY);

    /*
     * Enviar request ao servidor
     * */
    write(pipeRequest,request,requestSize());

    /*
     * Obter resposta do servidor
     * */



    if( (pipeAnswer = open(pipeClient, O_RDONLY)) == -1){
            perror("fifo between server and clients Read");
    }

    Answer a = malloc(sizeof(answerSize()));
    read(pipeAnswer,a,answerSize());
    printAnswer(a);


    close(pipeRequest);
    close(pipeAnswer);
    unlink(pipeClient);


    return 0;
}
