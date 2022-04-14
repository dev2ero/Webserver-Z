#include <iostream>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <sys/syscall.h>

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
