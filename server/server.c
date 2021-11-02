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
struct client_ds *clients;
int epollfd, max;

int main(int argc, char **argv) {
    int opt;
    // 需要先实现守护进程
    pthread_t login_tid, reactor_tid;
    while ((opt = getopt(argc, argv, "p:t:m")) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg);
                break;
            case 't':
                strcpy(token, optarg);
                break;
            case 'm':
                max = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -p port!\n", argv[0]);
                exit(1);
        }
    }
    int server_listen, sockfd;

    // 需要检查配置文件的完整性
    // 去config中取值初始化
    if (!port) port = atoi(get_conf_value(config, "PORT"));
    if (!max) max = atoi(get_conf_value(config, "MAXCLIENTS"));
    if (!strlen(token)) strcpy(token, get_conf_value(config, "TOKEN"));
    
    // 时钟信号触发心跳线程, 判断客户端是否掉线
    signal(SIGALRM, heart_beat);

    clients = (struct client_ds *)calloc(max, sizeof(struct client_ds));

    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create");
        exit(1);
    }
    
    if ((epollfd = epoll_create(1)) < 0) {
        perror("epoll_create");
        exit(1);
    }
    
    // 设置时钟信号的时间间隔
    struct itimerval itimer;
    itimer.it_interval.tv_sec = 1;          // 间隔时间
    itimer.it_interval.tv_usec = 0;
    itimer.it_value.tv_sec = 10;            // 起始时间    
    itimer.it_value.tv_usec = 0;
    
    setitimer(ITIMER_REAL, &itimer, NULL);
    // 登陆线程
    pthread_create(&login_tid, NULL, do_login, (void *)&server_listen);
    pthread_create(&reactor_tid, NULL, work_on_reactor, NULL);
    
    while (1) {
        sleep(10);
    }    
}
