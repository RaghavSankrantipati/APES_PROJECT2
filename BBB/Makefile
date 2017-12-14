#main.out: 
#	gcc decision.c main.c queue.c socket.c logger.c threads.c -o main.out -pthread -lrt

#clean:
#	rm main.out

VPATH=SOURCES

main.out : decision.c main.c queue.c socket.c logger.c threads.c
	gcc $^ -I INCLUDES -o $@ -lrt -lpthread -g

clean:
	rm main.out
