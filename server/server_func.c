/*************************************************************************
	> File Name: server_func.c
	> Author: 
	> Mail: 
	> Created Time: Tue 14 Sep 2021 10:28:05 PM CST
 ************************************************************************/

#include "head.h"

void *do_login(void *arg) {
    int server_listen, sockfd;
    server_listen = *(int *)arg;
    while (1) {
        sockfd = accept(server_listen, NULL, NULL);
        if (sockfd < 0) {
            perror("accept");
            exit(1);
        }
        // 验证客户的token值
        char token_tmp[100] = {0};
        if (recv(sockfd, token_tmp, sizeof(token_tmp), 0) <= 0) {
            close(sockfd);
            continue;
        }
        if (strcmp(token_tmp, token)) {
            close(sockfd);
            continue;
        }
        // 代码到这sockfd其实就是客户
    }
}

