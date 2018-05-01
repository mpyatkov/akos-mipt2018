// Код взят отсюда: https://www.youtube.com/watch?v=UKgvHcqF7GU

#include <iostream>
#include <sys/types.h>
#include <set> // set of slaves sockets
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm> // max_element

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

    while(true) {
	fd_set Set; 
	FD_ZERO(&Set);
	FD_SET(MasterSocket, &Set); // Добавляем серверный сокет для подключения новых клиентов

	for(auto Iter = SlavesSet.begin(); Iter != SlavesSet.end(); Iter++){
	    FD_SET(*Iter, &Set);
	}

	int Max = std::max(MasterSocket, *std::max_element(SlavesSet.begin(),
							   SlavesSet.end()));

	select(Max+1, &Set, NULL, NULL, NULL);

	for (auto Iter = SlavesSet.begin(); Iter != SlavesSet.end(); Iter++){
	    if(FD_ISSET(*Iter, &Set)) {
		static char Buffer[1024];
		int RecvSize = recv(*Iter, Buffer, 1024, MSG_NOSIGNAL);

		// Клиент отвалился
		if ((RecvSize == 0) && (errno !=EAGAIN)) {
		    shutdown(*Iter, SHUT_RDWR);
		    close(*Iter);
		    SlavesSet.erase(Iter);
		    
		}
		// Клиент готов принимать сообщения
		else if (RecvSize != 0) {
		    send(*Iter, Buffer, RecvSize, MSG_NOSIGNAL);
		    write(1, Buffer, RecvSize);
		}
		    
	    }
	}
	// Подлкючаем нового клиента
	if (FD_ISSET(MasterSocket, &Set)) {
	    int SlaveSocket = accept(MasterSocket, NULL, NULL);
	    set_nonblock(SlaveSocket);
	    SlavesSet.insert(SlaveSocket);
	}
    }
    
    return 0;
}
