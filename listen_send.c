#include "listen_send.h"
#define client_MAX 1000

int assign_client_descriptor(int server_socket,int argc)
{
    struct sockaddr_in client_addr; // socket description
    struct pollfd client_fd[client_MAX];

    int nfds;
    int client_socket;

    unsigned int client_size = sizeof(client_addr);
    unsigned int exist_client = 0;
    unsigned int i; // just loop variable

    client_fd[0].fd = server_socket;
    client_fd[0].events = POLLIN;    // read event;

    for(i=1;i<client_MAX;i++)
        client_fd[i].fd = -1;        // for empty decription
    /*
    static sn = 0;
    printf("%d poll return : %d\n",sn++, nfds);
    */

    while(1)
    {
        if(nfds = poll(client_fd, (sizeof(client_fd)/sizeof(unsigned long int)) ,10) == 0)
            continue;  
        else if(nfds == -1)
            printf("poll error %d\n",errno);
        else
        {
            if(client_fd[0].revents & POLLIN)
            {
                if((client_socket = accept(server_socket,(struct sockaddr*)&client_addr,&client_size)) == -1)
                {
                    printf("accept error! %d\n",errno);
                    continue;
                }
                else
                {
                    for(i=1;i<client_MAX;i++)
                    {
                        if(client_fd[i].fd < 0)
                        {
                            client_fd[i].fd = client_socket;        // assign client description
                            client_fd[i].events = POLLIN;
                            break;
                        }
                    }
                    if(i > exist_client)
                    {
                        exist_client = i;
                        printf("accept! %d\n",exist_client);
                    }

                    if(i==client_MAX)
                    {
                       printf("saturation!\n");
                       continue;
                    }
                }
            }
        }
        readdata(exist_client, client_fd,argc);
    }
    close(client_socket);
    return 0;
}

int readdata(unsigned int exist_client, struct pollfd client_fd[],int argc)
{
    unsigned int i; // just loop variable
    static const int BUFFER_SIZE = 128;
    ssize_t res_byte;
    char receive_buffer[BUFFER_SIZE];

    for(i=1;i<=exist_client;i++)
    {
        if(client_fd[i].fd < 0)
            continue;
        if(client_fd[i].revents & POLLIN)
        {
            // read data
            if((res_byte = read(client_fd[i].fd,receive_buffer,BUFFER_SIZE-1))==-1)
            {
                if(errno == 104)
                {
                    printf("disconnected\n");
                    client_fd[i].fd = -1;
                    continue;
                }
                printf("read error! %d\n",errno);
                return errno;
            }
            else
            {
                // write data
                if(write(client_fd[i].fd,receive_buffer,BUFFER_SIZE-1) == -1)
                {
                    printf("write error! %d\n",errno);
                    return errno;
                }
                receive_buffer[strlen(receive_buffer)] = '\0';

                // diconnect mode
                if(strncmp(receive_buffer,"quit",strlen("quit")) == 0)
                {
                    printf("disconnected\n");
                    client_fd[i].fd = -1;
                    continue;
                }
                // broadcast mode
                else if(argc == 3 && strlen(receive_buffer)>0)
                {
                    for(i=1;i<=exist_client;i++)
                    {
                        if(client_fd[i].fd < 0)
                            continue;
                        else
                            send(client_fd[i].fd,receive_buffer,strlen(receive_buffer),0);
                    }
                    printf("%s",receive_buffer);
                    return 0;
                }
                // general mode
                else
                    printf("%s",receive_buffer);

            }
        }
    }
    return 0;
}
