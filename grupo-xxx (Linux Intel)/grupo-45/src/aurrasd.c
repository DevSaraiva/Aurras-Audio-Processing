#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(char ** args, int argc){
    
    if(mkfifo("/tmp/fifo", 0666) == -1){
            perror("mkfifo");
    
    }

    int fd = open("/tmp/fifo", O_RDONLY);
    int hold_fifo = open("/tmp/fifo",O_WRONLY);
    int bytesRead = 0;
    char buffer[1024];
    

        while((bytesRead = read(fd, buffer, 1024)) > 0) {
            write(1, buffer, 1024);
            
        }

    
   
    close(fd);
    close(hold_fifo);
    return 0;
    
}