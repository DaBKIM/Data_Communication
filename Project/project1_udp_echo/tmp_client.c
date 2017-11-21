#include <sys/types.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

struct sockaddr_in server_addr;

#define BUFSIZE 1024

void *thread_recv(void *arg); //쓰레드 시작 함수
void *thread_send(void *arg);
pthread_t thr_id[2];

int main(int argc, char **argv)
{
    int sock;
    char message[BUFSIZE];
    int message_len, recv_len, recv_num;

    int status;
    
    if (argc != 3)
    {
        printf("usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    
    /* Create Socket */
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        exit(0);
    
    /* Address Setting */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));
    
    /* Connect to Server */
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr))
        == -1)
        exit(-1);
    
    while(1){
        
        if((status = pthread_create(&thr_id[0], NULL,&thread_send, &sock))!= 0) {// 송신 스레드
            printf("#1 Thread create error: %s\n", strerror(status));
            exit(0);
        }
        
        //인자로 지정한 스레드 id가 종료하기를 기다립니다.
        pthread_join(thr_id[0], NULL);
        
        if((status = pthread_create(&thr_id[1], NULL,&thread_recv, &sock))!= 0) {// 수신 스레드
            printf("#2 Thread create error: %s\n", strerror(status));
            exit(0);
        }
        
        pthread_join(thr_id[1], NULL);
        
        
    }
    
    close(sock);
    
    return 0;
}

char buf[BUFSIZE+1];
int nbyte;

void *thread_send(void *arg) {// 송신 스레드 함수
    
    int accp_sock=(int) *((int*) arg);
    int len;
    int status;
    
    printf("Please type the TEXT : ");
    if(fgets(buf,BUFSIZE+1,stdin)==NULL) exit(0);
    len=strlen(buf);
    
    if(buf[len-1]=='\n') buf[len-1]='\0';
    if(strlen(buf)==0){
        printf("No TEXT to send.\n");
        exit(0);
    }
    
    nbyte=sendto(accp_sock,buf,strlen(buf),0,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(nbyte<0){
        perror("Send Fail");
        exit(0);
    }
    
    pthread_exit(NULL);
    close(accp_sock);
    
}

void *thread_recv(void *arg) { // 수신 스레드 함수
    
    int accp_sock=(int) *((int*) arg);
    int addrlen;
    
    addrlen=sizeof(server_addr);
    
    nbyte=recvfrom(accp_sock,buf,BUFSIZE,0,(struct sockaddr*)&server_addr,&addrlen);
    if(nbyte<0){
        perror("Recv Fail");
        exit(0);
    }
    
    buf[nbyte]='\0';
    printf("[Echo] %s\n",buf);
    
    memset(buf,0,BUFSIZE+1);
    
    pthread_exit(NULL);
    close(accp_sock);
    
}
