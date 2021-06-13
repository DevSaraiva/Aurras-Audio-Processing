all: server client

server: bin/aurrasd

client: bin/aurras

GLIB = $(shell pkg-config --cflags --libs glib-2.0)


bin/aurrasd: src/aurrasd.c src/task.c src/waitingTasks.c src/request.c headers/task.h headers/waitingTasks.h headers/request.h 
	gcc -Wall -g -o bin/aurrasd src/aurrasd.c src/task.c src/waitingTasks.c src/request.c $(GLIB)


bin/aurras: src/aurras.c src/task.c src/waitingTasks.c src/request.c headers/task.h headers/waitingTasks.h headers/request.h 
	gcc -Wall -g -o bin/aurras src/aurras.c src/task.c src/waitingTasks.c src/request.c $(GLIB)


clean:
	rm obj/* tmp/* bin/{aurras,aurrasd}

test:
	bin/aurras samples/sample-1.mp3 tmp/sample-1.mp3
	bin/aurras samples/sample-2.mp3 tmp/sample-2.mp3

