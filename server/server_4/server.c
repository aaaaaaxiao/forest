#include "./include/myhead.h"

Msg *msg;
int main(int argc, char *argv[])
{ 
    if((msg = (Msg*)malloc(sizeof(Msg))) == NULL)
    {
        perror("msg malloc:");
	    exit(1);            
    }
    pthread_t tid;
    int listenfd = tcp_server_init("0", 8888, 50);
    if(-1 == listenfd){
        perror("init");
        return -1;
    }
    while(1)
    {
        int cfd = tcp_server_wait(listenfd);
        if(-1 == cfd){
            perror("wait");
            return -1;
        }
        pthread_create(&tid, NULL, pthread_read, (void *)&cfd);
    }
    close(listenfd);
    return 0;
} 
