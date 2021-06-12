#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

static const int MAXSIZEMESSAGE = 100;

int parse (int argc, char ** args,char destination[MAXSIZEMESSAGE]){
    
    for (int i = 0; i < argc ;i++){
       if (strlen(destination) + strlen(args[i]) > MAXSIZEMESSAGE) return -1;
       strcat(destination,args[i]); 
       if (i == argc -1) strcat(destination,";");
       else strcat(destination," ");
    }
    
    return strlen(destination);
}

int main(int argc,char ** args){

    int fd = open("/tmp/fifo", O_WRONLY);
    char destination[MAXSIZEMESSAGE];
    int len = parse(argc,args,destination);
    if (len == -1){
        printf("Mensagem demasiado grande");
    }
    else {
        
        write(fd, destination, len);
    }
        
    
    close(fd);
    return 0;
}