all: server client

server: bin/aurrasd

client: bin/aurras

GLIB = $(shell pkg-config --cflags --libs glib-2.0)


bin/aurrasd: src/aurrasd.c src/task.c src/listTasks.c src/filtersConfig.c src/request.c src/answer.c src/processMusic.c headers/task.h headers/listTasks.h headers/request.h headers/filtersConfig.h headers/answer.h headers/processMusic.h
	gcc -Wall -g -o bin/aurrasd src/aurrasd.c src/task.c src/listTasks.c src/filtersConfig.c src/request.c src/answer.c src/processMusic.c $(GLIB)


bin/aurras: src/aurras.c src/task.c src/listTasks.c src/request.c src/answer.c src/filtersConfig.c src/processMusic.c headers/task.h headers/listTasks.h headers/request.h headers/filtersConfig.h headers/answer.h headers/processMusic.h
	gcc -Wall -g -o bin/aurras src/aurras.c src/task.c src/listTasks.c src/filtersConfig.c src/request.c src/answer.c src/processMusic.c $(GLIB)


clean:
	rm obj/* tmp/* bin/{aurras,aurrasd}

test:
	bin/aurras samples/sample-1.mp3 tmp/sample-1.mp3
	bin/aurras samples/sample-2.mp3 tmp/sample-2.mp3

