#ifndef UTILS_H
#define UTILS_H

#include <iostream>

#define INFO	1
#define ERRO	2
#define LOG(x)	logmsg(x)
std::ostream& logmsg(int flag);

void handle_SIGPIPE();
int bind_listen_socket(int port);

#endif
