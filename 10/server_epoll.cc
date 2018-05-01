// Код взят отсюда: https://www.youtube.com/watch?v=UKgvHcqF7GU

#include <iostream>
#include <sys/types.h>
#include <set> // set of slaves sockets
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm> // max_element
#include <sys/epoll.h>

#define MAX_EVENTS 32

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

    int EPoll = epoll_create1(0);

    struct epoll_event Event;
    Event.data.fd = MasterSocket;
    Event.events = EPOLLIN;
    epoll_ctl(EPoll, EPOLL_CTL_ADD, MasterSocket, &Event);

    while(true) {
	struct epoll_event Events[MAX_EVENTS];
	int N = epoll_wait(EPoll, Events, MAX_EVENTS, -1);

	for (unsigned i = 0; i < N; i++) {
	    if (Events[i].data.fd == MasterSocket) {
		int SlaveSocket = accept(MasterSocket, NULL, NULL);
		set_nonblock(SlaveSocket);
		Event.data.fd = SlaveSocket;
		Event.events = EPOLLIN;
		epoll_ctl(EPoll, EPOLL_CTL_ADD,SlaveSocket,&Event);
	    }
	    else {
		static char Buffer[1024];
		int RecvSize = recv(Events[i].data.fd, Buffer, 1024, MSG_NOSIGNAL);
		if ((RecvSize == 0) && (errno !=EAGAIN)) {
		    shutdown(Events[i].data.fd, SHUT_RDWR);
		    close(Events[i].data.fd);
		} else if (RecvSize > 0) {
		    send(Events[i].data.fd, Buffer, RecvSize, MSG_NOSIGNAL);
		    write(1, Buffer, RecvSize);
		}
		
	    }
	    
	}
	
    }
    
    return 0;
}
