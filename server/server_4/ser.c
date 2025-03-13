/*===============================================
*   文件名称：ser.c
*   创 建 者：     
*   创建日期：2024年11月12日
*   描    述：
================================================*/
#include "./include/myhead.h"

int tcp_server_init(const char *ip, int port, int backlog)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == fd)
        return -1;
    struct sockaddr_in saddr;
    saddr.sin_family      = AF_INET;
    saddr.sin_port        = htons(port);
    saddr.sin_addr.s_addr = inet_addr(ip);

    if(-1 == bind(fd, (void *)&saddr, sizeof saddr)){
        close(fd);
        return -1;
    }

    if(-1 == listen(fd, backlog)){
        close(fd);
        return -1;
    }
    puts("listen...");
    return fd;
}

int tcp_server_wait(int fd)
{
    struct sockaddr_in caddr;
    socklen_t len = sizeof caddr;
    int nfd = accept(fd, (void *)&caddr, &len);
    if(-1 == nfd)
        return -1;
    printf("Client ip->%s:port->%d connected!\n", inet_ntoa(caddr.sin_addr), htons(caddr.sin_port));
    return nfd;
}
