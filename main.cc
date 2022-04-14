#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    if(argc != 2) {
	LOG(ERRO) << "Usage: " << argv[0] << " <port>" << endl;
	exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);

    handle_SIGPIPE();

    int listen_fd = -1;
    if((listen_fd == bind_listen_socket(port)) == -1) {
        LOG(ERRO)  << "Bind " << port << " port failed: "
                   << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    for(;;) {
	
    }
}
