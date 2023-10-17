#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

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
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serverAddr, '\0', sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4444);
    serverAddr.sin_addr.s_addr = inet_addr("10.40.216.97");

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr))< 0) {
        printf("Not connected\n");
        return -1;
    }

    pthread_t thread_ID;
    pthread_create(&thread_ID, NULL, receive_thread_function, (void*)&clientSocket);

    while(1) {
        printf("Enter a message: ");
        fgets(buffer, 1024, stdin);
        send(clientSocket, buffer, strlen(buffer), 0);
    }

    return 0;
}
