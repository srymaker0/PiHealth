/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: Wed 20 Oct 2021 10:09:45 PM CST
 ************************************************************************/

#include "head.h"

char server_ip[20];
int server_port;
char token[100];
char *config = "./pihealth.conf";

int main(int argc, char **argv) {
    int sockfd, opt;
    while ((opt = getopt(argc, argv, "s:p:t")) != -1) {
        switch (opt) {
            case 's':
                strcpy(server_ip, optarg);
                break;
            case 'p':
                server_port = atoi(optarg);
                break;
            case 't':
                strcpy(token, optarg);
                break;
            default:
                fprintf(stderr, "Usage : %s -p port -s server_ip!\n", argv[0]);
                exit(1);
        }
    }
    if (!server_port) server_port = atoi(get_conf_value(config, "SERVER_PORT"));
    if (!strlen(server_ip)) strcpy(server_ip, get_conf_value(config, "SERVER_IP"));
    if (!strlen(token)) strcpy(token, get_conf_value(config, "TOKEN"));
    
    if ((sockfd = socket_connect(server_ip, server_port)) < 0) {
        perror("socket_connect");
        exit(1);
    }
    // 发送token
    if (send(sockfd, token, strlen(token), 0) < 0) {
        close(sockfd);
        perror("send token");
        exit(1);
    }
    // 使用ack来验证token是否是对的 这里应该使用respond和request报文
    int ack = 0;
    if (recv(sockfd, &ack, sizeof(ack), 0) < 0) {
        close(sockfd);
        perror("recv ack");
        exit(1);
    }
    // 写成这样更准确表示token是正确的
    if (ack != 1) {
        fprintf(stderr, RED("Server Response Error!\n"));
    }
    sleep(1000);
    return 0;
}
