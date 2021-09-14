/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: Tue 14 Sep 2021 10:14:02 PM CST
 ************************************************************************/

#include "head.h"

static int port;
char *config = "./pihealthd.conf";
char token[100];


int main(int argc, char **argv) {
    int opt;
    // 实现守护进程
    int server_listen, sockfd, epollfd;
    pthread_t login_tid;
    while ((opt = getopt(argc, argv, "p:t:")) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg);
                break;
            case 't':
                strcpy(token, optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -p port!\n", argv[0]);
                exit(1);
        }
    }
    if (!port) port = atoi(get_conf_value(config, "PORT"));
    if (!strlen(token)) strcpy(token, get_conf_value(config, "TOKEN"));
    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create");
        exit(1);
    }
    // 登陆线程
    pthread_create(&login_tid, NULL, do_login, (void *)&server_listen);

}
