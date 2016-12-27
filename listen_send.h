#ifndef CLIENT_DESCRIPTOR_H
#define CLIENT_DESCRIPTOR_H
#endif // CLIENT_DESCRIPTOR_H

#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>


int readdata(unsigned int exist_client, struct pollfd client_fd[],int argc);
int assign_client_descriptor(int server_socket,int argc);
