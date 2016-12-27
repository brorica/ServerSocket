#include "listen_send.h"
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    /*  Just Debug mode
    argc = 3;
    argv[1] = "12345";
    argv[2] = "-eb";
    */
    if(argc < 2 || argc >3)
    {
        printf("usgae1 <port> [-eb]\n");
        exit(0);
    }
    if(argc==3 && strncmp(argv[2],"-eb",3) != 0)
    {
        printf("usgae2 <port> [-eb]\n");
        exit(0);
    }

    int server_socket = socket(AF_INET,SOCK_STREAM, 0);      // socket description
    if(server_socket==-1)
    {
        printf("socket description error! %d \n",errno);
        exit(0);
    }

    struct sockaddr_in server_addr;                      // socket address
    server_addr.sin_family= AF_INET;                     // use version 4
    server_addr.sin_addr.s_addr = htonl(0x7F000001) ;    // IP address
    server_addr.sin_port= htons(atoi(argv[1]));     // port number

    if(bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)) == -1)
    {
           printf("bind error %d\n",errno);
           exit(0);
    }
    printf("binding success!\n");

    if(listen(server_socket,10) == -1)        // loop listening
    {
        printf("listen error! %d \n",errno);
        exit(0);
    }

    assign_client_descriptor(server_socket,argc);
    close(server_socket);
    return 0;
}
