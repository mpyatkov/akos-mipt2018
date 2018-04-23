#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>

int main(int argc, const char * argv[]) {

     int serverSocket, clientSocket;
     ssize_t s;

     serverSocket  = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
    
     struct sockaddr_in serverAddr;

     memset(&serverAddr, 0, sizeof(serverAddr)); 
     serverAddr.sin_family = AF_INET;
     serverAddr.sin_port = htons(12345);
     serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

     if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
	  perror("bind");
	  exit(1);
     }

     if(listen(serverSocket, 5) < 0){
	  perror("listen");
	  exit(1);
     }

     
     while(1)
     {
	  clientSocket = accept(serverSocket, NULL, NULL);

	  if (!fork()) {
	      close(serverSocket);
	      char buffer[10] = {0};
	      do {
		  memset(&buffer, 0, sizeof(buffer));
		  int s = recv(clientSocket, buffer, sizeof(buffer), MSG_NOSIGNAL);
		  if (s == 0)
		      break;
		  send(clientSocket, buffer, sizeof(buffer), MSG_NOSIGNAL);
		  printf("pid: %d send %s\n", getpid(), buffer);
	      } while (strcmp(buffer, "quit\n") );
	      shutdown(clientSocket, SHUT_RDWR);
	      close(clientSocket);
	      _exit(0);
	  }
     }
     close(serverSocket);
     return 0;
}
