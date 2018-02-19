//client program

//Idea: Spawn a thread: open TCP connections to all other clients. Keep listening. Broadcast request to all, then if everyone replies with OK, proceed to CS
//Maintain one request_queue for each file. Assume 4 files for this project

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

//Initialise stuff for Lamport's Clock (Global)
int d = 1;
int clockME = 0; //localClock
int reqQ[5][4];
int localEvent() {
  clockME += d;
  return clockME;
}

typedef struct {
    //int clientID; // or char strVal[20];
    int timeStamp;
} tuple;
tuple requestQueue0[5], requestQueue1[5], requestQueue2[5], requestQueue3[5];

int globalEvent(long ts) {
  printf("Timestamp of request: %ld\n", ts);
  if(ts+d > clockME) {
    clockME = ts+d;
  }
  return clockME;
}

void *mutex(void *vargp) {

  //Does it matter what kind of request message other client sends? Most likely not.

  return NULL;
}

int main() {
  //Idea: create socket in main program. Implement logic in thread?
  //pthread_t tid;
  //pthread_create(&tid, NULL, mutex, NULL);
  //pthread_join(tid, NULL);
  int lePort = 6000;
  //int reqQ1[5], reqQ2[5], reqQ3[5], reqQ4[5], reqQ5[5];
  int socket_desc, client_sock, c, read_size, counter, localTS;
  struct sockaddr_in server, client;
  char client_message[2000];
  char *token, *data, *req[10];
  long requestTS;
  //Create socket
  socket_desc = socket(AF_INET , SOCK_STREAM , 0);
  setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
  if (socket_desc == -1) {
      printf("Couldnt create socket");
      return 1;
  }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = htons(lePort);

  if(bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) {
    printf("Bind failed. Error");
    return 1;
  }
  listen(socket_desc , 5);
  c = sizeof(struct sockaddr_in);

  client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
  if (client_sock < 0) {
    printf("Accept failed");
    return 1;
  }

  while((read_size = recv(client_sock, client_message, 2000 , 0)) > 0) {
    counter=0;
    //Cast void* to char*
    data = (char *) client_message;
    while ((token = strsep(&data, " ")) != NULL) {
      req[counter] = token;
      counter++;
    }
    //Format: "KEYWORD FILENAME TIMESTAMP CLIENTID"
    //printf(req[2]); //<- This is aOK!
    //Identify request type, and file name
    //Convert TS string to integer
    requestTS = strtol(req[2], NULL, 10);
    printf("%ld", requestTS);
    switch(strtol(req[1], NULL, 10)) {
      case 0:
        //Update local time. Add request to queue
        localTS = globalEvent(requestTS);
        //requestQueue0[0].clientI = req[3];
        int clientID = strtol(req[3], NULL, 10);
        requestQueue0[clientID].timeStamp = localTS;
        break;
      case 1:
        //Update local time. Add request to queue
        break;
      case 2:
        //Update local time. Add request to queue
        break;
      case 3:
        //Update local time. Add request to queue
        break;
    }
  }
  exit(0);
}
