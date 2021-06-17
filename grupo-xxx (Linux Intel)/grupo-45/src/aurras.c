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
#include "../headers/filtersConfig.h"

#define FIFOSERVERCLIENTS "/tmp/fifo"


void handler_ctrl_c(int signum){
    kill(getpid(),SIGSTOP);
}


int main(int argc,char ** args){


    signal(SIGINT,handler_ctrl_c);

    setbuf(stdout,NULL);
    int pipeAnswer;
    int pipeRequest;
    char pipeClient[30];
    pid_t pidProcess = getpid();

    sprintf(pipeClient,"%s%d",FIFOSERVERCLIENTS,pidProcess);

    if(mkfifo(pipeClient, 0666) == -1){
        perror("pipe de resposta do servidor ao cliente");

    }
     FiltersConfig fConfig = readConfigFile();
    /*
     * Criar o request para enviar ao servidor
     * */
    Request request = initRequest(argc,args,pidProcess,fConfig);

    if (getRequestService(request) == -1){
        printf("Comando não suportado\n");
        return -1;
    }
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

    int end = 0;
    Answer a = malloc(sizeof(answerSize()));
    
    read(pipeAnswer,a,answerSize());
       
    
        
    end = comparaToEnd(a);
    
    

    if(end == 0 && end != -1) printAnswer(a);

    while(!end){

        read(pipeAnswer,a,answerSize());
        
        end = comparaToEnd(a);

    }

    if(end != -1) printAnswer(a);
    else{

        pid_t pid = toInt(a);
        kill(pid,SIGTERM);
    }

    
    close(pipeRequest);
    close(pipeAnswer);
    unlink(pipeClient);


    return 0;
}
