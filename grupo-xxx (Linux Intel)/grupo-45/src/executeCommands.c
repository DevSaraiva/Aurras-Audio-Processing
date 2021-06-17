#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */


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

void mySystem (char* command){

    int i;
    int pid;
    int ret;
    char* string;
    int status;
    /*
     * Para ser mais eficiente: se não sei o número de arrgumentos do executável que pretendo correr então deveria alocar memória das strings
     * Usar realloc
     * Neste caso defino como sendo 20 o número máximo de argumentos
     * Este array tem todos os argumentos do executavel que pretendo correr este é precisamente um dos argumentos a passar no execv(execvp), por isso aqui estou já a dizer que vou usar o execv (ou execvp) para executar o programa
     * */
    char* exec_args[20];

    /*
     * Separar o nome do executável e os respectivos argumentos para os colocar num array
     * Estes devem ser separados pelo caracter espaço
     * */
    string = strtok(command," ");
    i = 0;
    while(string != NULL){
        exec_args[i] = string;
        string = strtok(NULL," ");
        i++;
    }

    /*
     * No final preciso de colocar o NULL pois é assim que esta formatado o parâmetro do execvp com o s argumentos do executável
     * */
    exec_args[i] = NULL;

    /*
     * Crio um filho para executar o executável que pretendo correr e assim o processo pai poder retornar para a função que chamou a mySystem e continuar a executar o codigo
     * */
    if( (pid = fork()) == 0 ){
        ret = execv(exec_args[0],exec_args);
        perror("processo filho errou: ");
        _exit(ret);
    }
    else{
        wait(&status);
    }
}

int main(){
    int i = 0;
    int j = 0;
    int p[2];
    char buf[128];
    ssize_t n;
    pid_t pid;
    int status;
    char execName[128];
    int fd = open("../testes/testScript", O_RDONLY, 0666);

    if(fd == -1){
        perror("Erro na abertura do ficheiro de scripts:");
        return -1;
    }

    while((n=readln(fd,buf,128))!=0){
        strcpy(execName,"aurras");
        strcat(execName," ");
        strcat(execName,buf);

        if( (pid = fork() ) == 0 ){
            mySystem(execName);
            _exit(0);
        }
        strcpy(buf,"");
        i++;
    }
    printf("Já foram lidos todos os comandos da script de teste\n");
    close(fd);

    for(j=0;j<i;j++){
        wait(&status);
    }

    return 0;
}
