#ifndef UTILS_H
#define UTILS_H

#include <iostream>

#define INFO	1
#define ERRO	2
#define LOG(x)	logmsg(x)
std::ostream& logmsg(int flag);

void handle_SIGPIPE();
int bind_listen_socket(int port);
bool set_socket_no_block(int fd);
bool set_socket_no_delay(int fd);
ssize_t readn(int fd, void* buf, size_t len, bool is_block = true, bool is_read = false);
ssize_t writen(int fd, void* buf, size_t len, bool is_write = false);

#endif
