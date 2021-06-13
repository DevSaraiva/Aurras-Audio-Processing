#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../headers/request.h"
#include "../headers/listTasks.h"
#include "../headers/task.h"
#include "../headers/status.h"

#define FIFOSERVERCLIENTS "/tmp/fifo"
#define CONFIGFILENAME "../etc/aurrasd.conf"


int contaLinhas(int fd){
    char buff;
    int linhas = 0;
    int bytesRead = 0;


    while((bytesRead = read(fd, &buff, 1)) > 0){
        if(buff == '\n') linhas++;
    }
    return linhas;
}

ssize_t readln(int fd, char *line, size_t size){
    size_t size_Line = 0;
    int barraN = 0;


    do{
        read(fd,line + size_Line,1);
        if(line[size_Line] != '\n' && line[size_Line] != '\0') {
            size_Line += 1;
        }
        else{
            barraN = 1;
        }
    }
    while(size_Line < size-1 && !barraN);
    line[size_Line] = '\0';


    return size_Line;
}

void criaConfigs(filtroConfig configs[], int numFiltros){

    char line[1024];

    int fdConfig  = open(CONFIGFILENAME, O_RDONLY, 0666);


    for(int i = 0; i < numFiltros; i++){

        readln(fdConfig, line, 1024);
        configs[i].identificador = strdup(strtok(line," "));
        configs[i].executavel = strdup(strtok(NULL," "));
        configs[i].numeroMaxExecucao = atoi(strtok(NULL," "));

    }


}

int contaPal (char s[]){
	int pal = 0, i = 0;
	while (s[i]){
		if (s[i] != ' ' && s[i] != '\n' && (s[i+1] == ' '|| s[i+1] == '\0'))
			pal++;
		i++;
	}
	return pal;
}


// Função que recebe uma string e separa por espaços os argumentos
void parse (char * string2, char ** destination){
    char * string = strtok(string2,";");
    //printf ("string :%s\n",string);
    char * aux = strtok(string," ");
    int i = 0;
    while (aux != NULL){
        //printf("%s\n",aux);
        destination[i] = strdup(aux);
        i++;
        aux = strtok(NULL," ");
    }

}

void initServer(){

}





int main(int argc, char ** args){
    
    int fd,hold_fifo;

    unlink(FIFOSERVERCLIENTS);

    if(mkfifo(FIFOSERVERCLIENTS, 0666) == -1){
            perror("fifo between server and clients");

    }

    //Leitura da configuração
    
    int fdConfig = open(CONFIGFILENAME, O_RDONLY, 0666);

    int numFiltros = contaLinhas(fdConfig);

    filtroConfig configs[numFiltros];

    criaConfigs(configs, numFiltros);

    
    //Criação das estruturas de controlo

    listTasks emEspera = createListTasks();

    listTasks * aExecutar = createListTasks();
    
    
    
    //Abertura do Fifo

    if( (fd = open(FIFOSERVERCLIENTS, O_RDONLY)) == -1){
            perror("fifo between server and clients Read");
    }

    if( ( hold_fifo = open(FIFOSERVERCLIENTS, O_WRONLY)) == -1){
            perror("fifo between server and clients Write");
    }

    /*
     * Abrimos a extremidade de escrita do pipe que comunica com os clientes
     * para que caso nenhum cliente esteja associado ao servidor este pipe não desapareça
     * */


    while(1){
        
        Request request = createRequest();
        read(fd,request,requestSize());

        printRequest(request);

        switch(getRequestService(request)){
            case 1:
                printf("Processar transform");
                // O servidor tem de ir buscar o estado atual do servidor e enviar para o client
                // A leitura do estado tem de ser uma operação atómica
                // ao criar um processo flho o estado do processo filho não é mais alterado pelo processo pai
                // por isso o estado do servidor no processo filho é estático
                //

                //verificar se posso fazer o processamento
                //se sim criar processo filho para fazer o processamento e atualizar os filtro
                //se não, colocar o processamento numa fila de espera, e passar para o próximo request

                pid_t pid,terminated_pid;
                int status;

                //filtersNeeded(request);

                if((pid = fork()) == 0){
                    //fazer o processamento pedido
                }
                else{
                    /*
                     * O terminated_pid é o identificador do processo filho que terminou
                     * o WEXITSTATUS(status) permite ver o codigo de saída do filho, como temos _exit(0) se tudo correr bem será 0
                     * */
                    terminated_pid = wait(&status);

                    /*
                     * waitpid(pid,&status,0) <- isto permitia que este processo esperasse pelo processo com o idenitificador igual a pid
                     * */
                    printf("terminated_pid: %d  |  exit_status: %d \n", terminated_pid,WEXITSTATUS(status));
                }
                break;

            case 2:
                printf("Processar status");
                

                if((pid = fork()) == 0){
                    _exit(0);
                }
                else{
                    /*
                     * O terminated_pid é o identificador do processo filho que terminou
                     * o WEXITSTATUS(status) permite ver o codigo de saída do filho, como temos _exit(0) se tudo correr bem será 0
                     * */
                    terminated_pid = wait(&status);

                    /*
                     * waitpid(pid,&status,0) <- isto permitia que este processo esperasse pelo processo com o idenitificador igual a pid
                     * */
                    printf("terminated_pid: %d  |  exit_status: %d \n", terminated_pid,WEXITSTATUS(status));
                }
                break;

            default:
                printf("Nothing");

        }
    }

    close(fd);
    close(hold_fifo);

    return 0;

}

