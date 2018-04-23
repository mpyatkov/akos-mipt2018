#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char * argv[]) {

     int serverSocket, clientSocket;
     ssize_t s;
     
     serverSocket  = socket(PF_INET, SOCK_STREAM, 0);

     struct sockaddr_in serverAddr;

     memset(&serverAddr, 0, sizeof(serverAddr)); 
     serverAddr.sin_family = AF_INET;
     serverAddr.sin_port = htons(12345);
     serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //0.0.0.0

     /* int optval = 1; */
     /* setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)); */
     /* setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val)); */

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

	  char buffer[100] = {0};
	  recv(clientSocket, buffer, sizeof(buffer), MSG_NOSIGNAL);
	  send(clientSocket, buffer, sizeof(buffer), MSG_NOSIGNAL);

	  shutdown(clientSocket, SHUT_RDWR);
	  close(clientSocket);
	
	  printf("%s", buffer);
	  
     }
     close(serverSocket);
     return 0;
}
