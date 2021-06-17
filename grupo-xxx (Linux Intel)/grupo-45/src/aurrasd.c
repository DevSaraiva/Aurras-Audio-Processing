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
#include "../headers/listTasks.h"
#include "../headers/task.h"
#include "../headers/filtersConfig.h"
#include "../headers/processMusic.h"

#define FIFOSERVERCLIENTS "/tmp/fifo"
#define SAMPLESFILENAME "../samples/"
#define OUTPUTSFILENAME "../outputs/"
#define MAXSIZEINPUTNAME 50
#define MAXSIZEOUTPUTNAME 50


static int pipeFiltro[2];
static int pipeServidorEntrada[2];
static int pipeServidorSaida[2];

/* Lista de tasks em execucao */
ListTasks runningTasks;

/* Lista de tasks em espera */
ListTasks waitingTasks;


FiltersConfig filtersConfig;

static int numberOfTasks = 1;




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
    char * aux = strtok(string," ");
    int i = 0;
    while (aux != NULL){
        destination[i] = strdup(aux);
        i++;
        aux = strtok(NULL," ");
    }

}


void executaTask(Task task){

    pid_t pid;
    int numberExecs = getNumberFiltersTask(task);
    
      //Redirecionamento do ficheiro de output para a pasta correta
      char redInputName[MAXSIZEINPUTNAME] = "";
      char* fileSamplesName = SAMPLESFILENAME;
      strcat(redInputName,fileSamplesName);
      strcat(redInputName,getInputFileTask(task));

      char redOutputName[MAXSIZEOUTPUTNAME] = "";
      char* fileOutputsName = OUTPUTSFILENAME;
      strcat(redOutputName,fileOutputsName);
      strcat(redOutputName,getOutputFileTask(task));
    
    
    if((pid = fork()) == 0){

        //fecha o pipe que liga o processo intermediario e o servidor
        close(pipeServidorEntrada[0]);
        close(pipeServidorEntrada[1]);
        close(pipeServidorSaida[0]);
        close(pipeServidorSaida[1]);
        //fazer o processamento pedido
        char** execsFilters = getExecsFilters(task, filtersConfig);
        processMusic(redInputName,redOutputName, execsFilters,numberExecs);
        int numberTask = getNumberTask(task);
        write(pipeFiltro[1],&numberTask,4);
        close(pipeFiltro[1]);
        _exit(0);
    }
}




void processWaitingTasks(){
    int i;
    int* filtersRequired;

    for(i=0; i < getNumberListTasks(waitingTasks); i++){
        
        Task task = getTaskIndex(waitingTasks,i);
        
        int validTask = validateTaskProcessing(filtersConfig,task);
        
        if(validTask == 1){
            /*
             * Fazer update dos filtros disponíveis
             * */
            filtersRequired = getFiltersRequired(task);
            
            updateFiltersConfig(filtersConfig,filtersRequired,1);
            
            addTask(runningTasks,task);

            removeTaskIndex(waitingTasks,i);

            executaTask(task);

            }
        }
    }


void handler_terminatedProcessing(int signum){
    /*
     * Receber um pid de processo
     * */

    int on = 1;

    int numberTask;

    //Lê o numero da task a retirar
    
    read(pipeServidorEntrada[0],&numberTask,4);

    //Imprime no servidor sobre o termino de uma Task

    printf("A task numero %d terminou\n",numberTask);

    Task task = getTask(runningTasks,numberTask);
    
    int* filtersRequired = getFiltersRequired(task);
    /*
     * Remover a task que foi executada no processo com o pid recebido
     * */
    removeTaskByNumber(runningTasks,numberTask);
    
    /*
     * Atualizar os filtros
     * */
    updateFiltersConfig(filtersConfig,filtersRequired,-1);

    
    //Coloca as tasks da lista de espera em processamento
    processWaitingTasks();
    
    //Alertar filho responsavel pelo processamento que está livre para tratar de sinais
     write(pipeServidorSaida[1], &on, 4);

}



int main(int argc, char ** args){
    
    int fd,hold_fifo;
    pid_t pid;

    unlink(FIFOSERVERCLIENTS);

    setbuf(stdout,NULL);

    if(mkfifo(FIFOSERVERCLIENTS, 0666) == -1){
            perror("fifo between server and clients");
    }

    if(signal(SIGUSR1,handler_terminatedProcessing) == SIG_ERR){
        perror("SIGUSRS failed");
    }

    if(signal(SIGUSR1,handler_terminatedProcessing) == SIG_ERR){
        perror("SIGUSRS failed");
    }


    /* Lista de tasks em execucao */
    runningTasks = createListTasks();

    /* Lista de tasks em espera */
    waitingTasks = createListTasks();


    /*
     * Guardar a informação sobre os filtros do ficheiros config
     * */
    filtersConfig = readConfigFile();


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


    //Cria filho gestor de processos
        
    pipe(pipeFiltro);
    pipe(pipeServidorEntrada);
    pipe(pipeServidorSaida);

    pid_t pai = getpid();
    
    

    if((pid = fork()) == 0){

        int taskNumber = 0;
        int servidorDisponivel = 0;

        close(pipeFiltro[1]);
        close(pipeServidorEntrada[0]);
        close(pipeServidorSaida[1]);

        while(1){

            //Recebe o numero da task
            read(pipeFiltro[0],&taskNumber,4);

            //bloqueia o envio enquanto o servidor esta a tratar um termino
            read(pipeServidorSaida[0],&servidorDisponivel,4);
            
            //Envia para o servidor o numero da task a retirar
            write(pipeServidorEntrada[1],&taskNumber,4);

            //Envia para o servidor um sinal a avisar que pode ler do pipe
            kill(pai,SIGUSR1);

        }

    }

    //Fecha a extermidade de leitura do pipe entre os processos que correm os filtros e o intermediario
    close(pipeFiltro[0]);
    close(pipeServidorSaida[0]);
    close(pipeServidorEntrada[1]);
    
    while(1){
        
        
        int on = 1;

        //Sinaliza o processo de controlo sobre a disponiblidade inicial do servidor
        write(pipeServidorSaida[1], &on, 4);


        char pipeClient[30];
        int pipeAnswer;
        Task task;
        int* filtersRequired;
        int validateTask;


        Request request = createRequest();
        read(fd,request,requestSize());

        switch(getRequestService(request)){
            case 1:

                sprintf(pipeClient,"%s%d",FIFOSERVERCLIENTS,getRequestPidProcess(request));

                        if( (pipeAnswer = open(pipeClient, O_WRONLY)) == -1){
                                perror("fifo between server and clients Read");
                        }


                task = createTask(request,filtersConfig);
                setNumberTask(task, numberOfTasks++);

                filtersRequired = getFiltersRequired(task);

                validateTask = validateTaskProcessing(filtersConfig,task);

                if(validateTask == -1){
                    //adiciona à lista de espera e informa o cliente
                    addTask(waitingTasks,task);
                    Answer a = createAnswer1(1);
                    write(pipeAnswer,a,answerSize());
                    
                }
                else if(validateTask == 1){
                    addTask(runningTasks,task);
                    
                    updateFiltersConfig(filtersConfig,filtersRequired,1);
                    
                    executaTask(task);

                }

               break;

            case 2:

             if((pid = fork()) == 0){

                sprintf(pipeClient,"%s%d",FIFOSERVERCLIENTS,getRequestPidProcess(request));

                if( (pipeAnswer = open(pipeClient, O_WRONLY)) == -1){
                    perror("fifo between server and clients Read");
                }

                Answer answer = createAnswer2(filtersConfig,runningTasks);
                  
                write(pipeAnswer,answer,answerSize());
                    
                _exit(0);
            }

        }
    }

    close(fd);
    

    return 0;

}

