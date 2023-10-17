#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

typedef struct {
    struct sockaddr_in sock;
    int socket;
    pthread_t thread_ID;
    struct Client * next;
}Client;

void addClient(Client** list){
    Client * item = malloc(sizeof(Client));
    item->next = *list;
    *list = item;
}

void* receive_thread_function(void* arg){
    int* socket = arg;
    char buffer[1024];
    while(1) {
        recv(*socket, buffer, 1024, 0);
        printf("Message received: %s\n", buffer);
    }
    return NULL;
}

int main() {
    int sockfd;
    int res;
    struct sockaddr_in serverAddr;

    Client *clients;
    addClient(&clients);
    socklen_t addr_size;

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serverAddr, '\0', sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4444);
    serverAddr.sin_addr.s_addr = inet_addr("10.40.216.97");

    res = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(res < 0) return -1;

    listen(sockfd, 5);
    addr_size = sizeof(clients->sock);

    pthread_t thread_ID;

    while (1){
        clients->socket = accept(sockfd, (struct sockaddr*)&(clients->sock), &addr_size);
        pthread_create(&(clients->thread_ID), NULL, receive_thread_function, (void*)&(clients->socket));
        addClient(clients);
    }

    return 0;
}