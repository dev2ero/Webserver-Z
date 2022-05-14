#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/syscall.h>
#include <sys/socket.h>

#include "utils.h"

std::ostream& logmsg(int flag) {
    long tid = syscall(SYS_gettid);
    switch(flag) {
	case INFO:
	    std::cout << tid << ": [INFO]\t";
	    return std::cout;
	case ERRO:
	    std::cerr << "\x1b[31m"<< tid << ": [ERRO]\x1b[0m\t";
	    return std::cerr;
	default:
	    LOG(ERRO) << "Wrong LOG type" << std::endl;
	    abort();
    }
}

void handle_SIGPIPE() {
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    if(sigaction(SIGPIPE, &sa, NULL) == -1)
	LOG(ERRO) << "Ignore SIGPIPE failed! " << strerror(errno) << std::endl;
}

int bind_listen_socket(int port) {
    int listen_fd = 0;
    if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return -1;
    sockaddr_in server_addr;
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons((unsigned short)port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int opt = 1;
    if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        return -1;
    if(bind(listen_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1)
        return -1;
    if(listen(listen_fd, 1024) == -1)
        return -1;
    return listen_fd;
}

bool set_socket_no_block(int fd) {
    int flag = fcntl(fd, F_GETFD);
    if(flag == -1)
        return -1;
    flag |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flag) == -1)
        return false;
    return true;
}

bool set_socket_no_delay(int fd) {
    int enable = 1;
    if(setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void*)&enable, sizeof(enable)) == -1)
        return false;
    return true;
}
