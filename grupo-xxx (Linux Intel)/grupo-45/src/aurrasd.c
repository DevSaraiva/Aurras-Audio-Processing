#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct filtro{
    
    char * identificador;
    char * executavel;
    int tamFilaEspera;
    int emEspera;


} filtroConfig;


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
    
    int fdConfig  = open("../etc/aurrasd.conf", O_RDONLY, 0666);
    

    for(int i = 0; i < numFiltros; i++){

        readln(fdConfig, line, 1024);
        configs[i].identificador = strdup(strtok(line," "));
        configs[i].executavel = strdup(strtok(NULL," "));
        configs[i].tamFilaEspera = atoi(strtok(NULL," "));
        
    }
    
     
}


int main(int argc, char ** args){
    
    setbuf(stdout,NULL);

    unlink("/tmp/fifo");

    if(mkfifo("/tmp/fifo", 0666) == -1){
            perror("mkfifo");
    
    }
    
    int fdConfig = open("../etc/aurrasd.conf", O_RDONLY, 0666);
     

    //int fd = open("/tmp/fifo", O_RDONLY);
  
    //int hold_fifo = open("/tmp/fifo",O_WRONLY);
    
    //int bytesRead = 0;
    
    //char buffer[1024];
    
    
    //Leitura da configuração
    

    int numFiltros = contaLinhas(fdConfig);
   
    filtroConfig configs[numFiltros];

    criaConfigs(configs, numFiltros);

    printf("%s",configs[0].identificador);


    


        //while((bytesRead = read(fd, buffer, 1024)) > 0) {
        //    write(1, buffer, bytesRead);
            
        //}

    
    //close(fd);
    //close(hold_fifo);
    return 0;
    
}