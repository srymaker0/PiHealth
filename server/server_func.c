/*************************************************************************
	> File Name: server_func.c
	> Author: 
	> Mail: 
	> Created Time: Tue 14 Sep 2021 10:28:05 PM CST
 ************************************************************************/

#include "head.h"

extern char token[100];
extern struct client_ds *clients;
extern int epollfd, max;

int check_online(struct sockaddr_in *addr) {
    int flag = 0;
    for (int i = 0; i < max; i++) {
        if (addr->sin_addr.s_addr == clients[i].addr.sin_addr.s_addr) {
            flag = 1;
            break;
        }
    }
    return flag;
}

void *do_login(void *arg) {
    int server_listen, sockfd;
    server_listen = *(int *)arg;
    while (1) {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        sockfd = accept(server_listen, (struct sockaddr *)&client, &len);
        if (sockfd < 0) {
            perror("accept");
            exit(1);
        }
        // 定时器，客户没发数据直接断开
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(sockfd, &rfds);
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 10000;
        if (select(sockfd + 1, &rfds, NULL, NULL, &tv) <= 0) {
            close(sockfd);
            DBG(RED("<Errorr> : time out from token !\n"));
            continue;
        }

        // 验证客户的token值
        char token_tmp[100] = {0};
        if (recv(sockfd, token_tmp, sizeof(token_tmp), 0) <= 0) {
            close(sockfd);
            continue;
        }
        if (strcmp(token_tmp, token)) {
            close(sockfd);
            DBG(RED("<token> : Error!\n"));
            continue;
        }
        // 判断是否已经在线（应对用户疯狂点击, 可用数据结构hash等） 
        int ack = 1;
        if (check_online(&client)) {
            ack = 0;    
        }
        
        if (send(sockfd, (void *)&ack, sizeof(int), 0) < 0) {
            close(sockfd);
            DBG(RED("<Error> : send ack error!\n"));
            continue;
        }

        if (ack == 0) {
            close(sockfd);
            DBG(YELLOW("<Warning> : %s already login!\n"), inet_ntoa(client.sin_addr));
            continue;
        }

        clients[sockfd].sockfd = sockfd;
        clients[sockfd].isonline = 10;
        clients[sockfd].addr = client; 
        struct epoll_event ev;
        ev.data.fd = sockfd;
        ev.events = EPOLLIN;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);
        DBG(YELLOW("<client> : ---!\n"));
        // 代码到这sockfd其实就是客户

    }
}

