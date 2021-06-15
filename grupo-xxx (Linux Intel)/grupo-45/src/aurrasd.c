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

#define FIFOSERVERCLIENTS "/tmp/fifo"
#define CONFIGFILENAME "../etc/aurrasd.conf"


static int numberOfTasks = 0;

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

int countLinesFile(char* fileName){

    char buff;
    int linhas = 0;
    int bytesRead = 0;
    int fd;

    if( (fd = open(fileName, O_RDONLY, 0666)) == -1){
        perror("open file to count lines");
    }

    while((bytesRead = read(fd, &buff, 1)) > 0){
        if(buff == '\n') linhas++;
    }

    close(fd);
    return linhas;
}


FiltersConfig readConfigFile(){
    int fdConfig;
    int numFiltros;
    char line[1024];

    numFiltros = countLinesFile(CONFIGFILENAME);

    FiltersConfig filtersConfig = createFiltersConfig();

    if( (fdConfig = open(CONFIGFILENAME, O_RDONLY, 0666)) == -1){
        perror("open config file");
    }

    for(int i = 0; i < numFiltros; i++){
        Filter filter = createFilter();
        readln(fdConfig, line, 1024);
        char* identificador = strdup(strtok(line," "));
        char* exec =  strdup(strtok(NULL," "));
        int maxExec = atoi(strtok(NULL," "));
        setIdentificadorFilter(filter,identificador);
        setExecutavelFilter(filter, exec);
        setMaxExecucaoFilter(filter, maxExec);
        addFilterConfig(filtersConfig,filter);
    }
    close(fdConfig);

    return filtersConfig;
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
    int fd;
    setbuf(stdout,NULL);

    unlink(FIFOSERVERCLIENTS);

    if(mkfifo(FIFOSERVERCLIENTS, 0666) == -1){
            perror("fifo between server and clients");

    }

    if(mkfifo(FIFOSERVERCLIENTS, 0666) == -1){
            perror("fifo between server and clients");

    }

    /*
     * Guardar a informação sobre os filtros do ficheiros config
     * */
    
    FiltersConfig filtersConfig = readConfigFile();

    //printFilterConfig(filtersConfig);

    if( (fd = open(FIFOSERVERCLIENTS, O_RDONLY)) == -1){
            perror("fifo between server and clients Read");
    }

    /*
     * Abrimos a extremidade de escrita do pipe que comunica com os clientes
     * para que caso nenhum cliente esteja associado ao servidor este pipe não desapareça
     * */
    
    /* Lista de tasks em espera */
    ListTasks waittingTask = createListTasks();
    /* Lista de tasks em execucao */
    ListTasks runningTasks = createListTasks();

    /* Lista de tasks em espera */
//    ListTasks waitingTasks = createListTasks();


    while(1){
        pid_t pid,terminated_pid;
        int status;
        char pipeClient[30];
        int pipeAnswer;
        Task task;

        Request request = createRequest();
        read(fd,request,requestSize());

        switch(getRequestService(request)){
            case 1:
                task = createTask(request,filtersConfig);
                setNumberTask(task, numberOfTasks++);

                /* Apenas para teste do status*/
                addTask(runningTasks,task);
                //updateFiltersConfig(filtersConfig,task);

                // O servidor tem de ir buscar o estado atual do servidor e enviar para o client
                // A leitura do estado tem de ser uma operação atómica
                // ao criar um processo flho o estado do processo filho não é mais alterado pelo processo pai
                // por isso o estado do servidor no processo filho é estático
                //

                //verificar se posso fazer o processamento
                //se sim criar processo filho para fazer o processamento e atualizar os filtro e colocar numa fila de tasks em execução
                //se não, colocar o processamento numa fila de espera, e passar para o próximo request

/*
                char filtrosRequest[numfilters] = filtersNeeded(request);
                filtersExist(filtrosRequest);
*/
                
                break;

            case 2:

                    if((pid = fork()) == 0){

                        sprintf(pipeClient,"%s%d",FIFOSERVERCLIENTS,getRequestPidProcess(request));

                        if( (pipeAnswer = open(pipeClient, O_WRONLY)) == -1){
                                perror("fifo between server and clients Read");
                        }

                        Answer answer = createAnswer2(filtersConfig,runningTasks);
                    
                        write(pipeAnswer,answer,answerSize());
                    
                    }

               
                break;


        }
    }

    close(fd);


    return 0;

}

