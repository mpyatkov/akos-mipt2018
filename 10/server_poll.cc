// Код взят отсюда: https://www.youtube.com/watch?v=UKgvHcqF7GU

#include <iostream>
#include <sys/types.h>
#include <set> // set of slaves sockets
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm> // max_element
#include <poll.h>

#define POLL_SIZE 2048

int set_nonblock(int fd) {
    // TODO: Нужна проверка flags
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main(int argc, char **argv)
{
    printf("Server pid: %d\n", getpid());
    int MasterSocket = socket(AF_INET, SOCK_STREAM, 0);
    std::set<int> SlavesSet;

    struct sockaddr_in SockAddr;
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_port = htons(12345);
    SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(MasterSocket, (struct sockaddr *)(&SockAddr), sizeof(SockAddr));

    set_nonblock(MasterSocket);
    
    listen(MasterSocket, SOMAXCONN);

    struct pollfd Set[POLL_SIZE];
    Set[0].fd = MasterSocket;
    Set[0].events = POLLIN;
    
    
    while(true) {
	unsigned int Index = 1;
	for(auto Iter = SlavesSet.begin(); Iter != SlavesSet.end(); Iter++) {
	    Set[Index].fd = *Iter;
	    Set[Index].events = POLLIN;
	    Index++;
	}

	unsigned int SetSize = 1 + SlavesSet.size();
	poll(Set, SetSize, -1);
	
	for (unsigned int i = 0; i < SetSize; i++) {
	    if (Set[i].revents & POLLIN) {
		//Клиент готов к чтению
		if (i) {
		    static char Buffer[1024];
		    int RecvSize = recv(Set[i].fd, Buffer, 1024, MSG_NOSIGNAL);
		    if ((RecvSize == 0) && (errno !=EAGAIN)) {
			shutdown(Set[i].fd, SHUT_RDWR);
			close(Set[i].fd);
			SlavesSet.erase(Set[i].fd);
		    
		    } else if (RecvSize > 0) {
			send(Set[i].fd, Buffer, RecvSize, MSG_NOSIGNAL);
			write(1, Buffer, RecvSize);
		    }

		}
		// Пришел новый клиент
		else {
		    int SlaveSocket = accept(MasterSocket, NULL, NULL);
		    set_nonblock(SlaveSocket);
		    SlavesSet.insert(SlaveSocket);
		}
	    }
	}
    }

	
	return 0;
    }
