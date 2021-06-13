#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../headers/request.h"

#define FIFOSERVERCLIENTS "/tmp/fifo"



int main(int argc,char ** args){

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
    printRequest(request);
    int fd = open(FIFOSERVERCLIENTS, O_WRONLY);

    /*
     * Enviar request ao servidor
     * */
    write(fd,request,requestSize());

    close(fd);
    return 0;
}
