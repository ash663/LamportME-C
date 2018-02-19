//server
/*Idea: Listen on one socket for requests, open a thread for each request
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
  int socket_desc, client_sock, c, read_size;
  struct sockaddr_in server, client;
  char client_message[2000];

  socket_desc = socket(AF_INET , SOCK_STREAM , 0);

  if (socket_desc == -1) {
      printf("Couldnt create socket");
      return 1;
  }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = htons(5000);

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
  printf("New request\n");

  while((read_size = recv(client_sock, client_message, 2000 , 0)) > 0) {
    //write(client_sock, client_message, strlen(client_message));
  }
  printf(client_message);
  close(client_sock);
  //TODO: Write code to regex the request, and perform the action
  //free(socket_desc);
  return 0;
}
