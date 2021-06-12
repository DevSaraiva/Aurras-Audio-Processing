#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(char ** args, int argc){

    char buffer[1024];
    int bytesRead = 0;
    int fd = open("/tmp/fifo", O_WRONLY);

    while((bytesRead = read(STDIN_FILENO, buffer, 1024)) > 0) {
       
        write(fd, buffer, bytesRead);
        
    }
    close(fd);
    return 0;
}
