#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define MAXLINE 1024 //buf 크기

void *thread_recv(void *arg); //쓰레드 시작 함수
void *thread_send(void *arg);
void *thread_sendACK(void *arg);

int listen_sock, accp_sock;
pthread_t thr_id[2];
struct sockaddr_in server_addr, client_addr;
pthread_mutex_t mutex;
pthread_cond_t count_threshold_cv;

fd_set readfds, masterfds;

int nbyte;
char buf[MAXLINE+1];

int main(int argc, char *argv[]) {
    
    int addrlen = sizeof(server_addr);
    int i, status ;
    pid_t pid;
    
    if(argc != 2) {
        printf("Use %s PortNumber\n", argv[0]);
        exit(0);
    }
    
    //create socket
    if((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket Fail");
        exit(0);
    }
    
    //server
    memset(&server_addr, 0, sizeof(server_addr)); //0으로 초기화
    memset(&client_addr,0,sizeof(client_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));
    
    /*struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    
    FD_ZERO(&masterfds);
    FD_SET(listen_sock, &masterfds);
    
    memcpy(&readfds, &masterfds, sizeof(fd_set));*/
    
    //bind 호출
    if(bind(listen_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind Fail");
        exit(0);
    }
    
    
    listen(listen_sock, 10);
        
    puts("client wait....");
    
    accp_sock = accept(listen_sock, (struct sockaddr *)&client_addr, &addrlen);
    //printf("1\n");
    if(accp_sock < 0) {
        perror("accept fail");
        exit(0);
    }
    
    while(1){
        
        /*if (select(sockfd+1, &readfds, NULL, NULL, &timeout) < 0)
        {
            perror("on select");
            exit(1);
        }*/
        
        if((status = pthread_create(&thr_id[0], NULL,&thread_recv, &accp_sock))!= 0) { // 수신 스  레드
            printf("#1 Thread create error: %s\n", strerror(status));
            exit(0);
        }
        //인자로 지정한 스레드 id가 종료하기를 기다립니다.
        pthread_join(thr_id[0], NULL);
        
        if(nbyte>0){
            buf[nbyte]='\0';
            printf("Client : %s",buf);
            
            int random=rand()%2; // ACK 전달 및 타임아웃 테스트
            printf("(ACK test : %d)\n",random); // random==1 ack 전송, random==0 재수신 대기
            
            if(random==1){
                if((status = pthread_create(&thr_id[1], NULL,&thread_sendACK, &accp_sock))!= 0){ // 송신 스레드
                    printf("#3 Thread create error: %s\n", strerror(status));
                    exit(0);
                }
            }else continue;
        }
        
        /*
        if((status = pthread_create(&thr_id[1], NULL,&thread_send, &accp_sock))!= 0) { // 송신 스레드
            printf("#2 Thread create error: %s\n", strerror(status));
            exit(0);
        }
        
        pthread_join(thr_id[1], NULL);*/
        

    }
    
    close(accp_sock);
    return 0;
}

void *thread_recv(void *arg) { //수신 스레드 함수
    
    int accp_sock=(int) *((int*) arg);
    int addrlen;
    int status;
    
    addrlen=sizeof(client_addr);
    
   
    nbyte=recvfrom(accp_sock,buf,MAXLINE,0,(struct sockaddr*)&client_addr,&addrlen);
    if(nbyte<0){
        perror("Recv Fail");
        exit(0);
    }
    
    //pthread_mutex_lock(&mutex);
    
    //pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
    close(accp_sock);

}

void *thread_send(void *arg) { //송신 스레드 함수
    
    int accp_sock=(int) *((int*) arg);
    int addrlen,len;
    
    addrlen=sizeof(client_addr);
    
    printf("Server : ");
    if(fgets(buf,MAXLINE+1,stdin)==NULL) exit(0);
    len=strlen(buf);
    
    if(buf[len-1]=='\n') buf[len-1]='\0';
    if(strlen(buf)==0){
        printf("No TEXT to send.\n");
        exit(0);
    }
    
    nbyte=sendto(accp_sock,buf,strlen(buf),0,(struct sockaddr*)&client_addr,sizeof(client_addr));
    if(nbyte<0){
        perror("Send Fail");
        exit(0);
    }
        
    memset(buf,0,MAXLINE+1);

    pthread_exit(NULL);
    close(accp_sock);
    
}

void *thread_sendACK(void *arg) { //송신 스레드 함수
    
    int accp_sock=(int) *((int*) arg);
    int addrlen,len;
    char* ack="ACK : Server receive Message";
    
    addrlen=sizeof(client_addr);
    
    len=strlen(ack);
    
    nbyte=sendto(accp_sock,ack,strlen(ack),0,(struct sockaddr*)&client_addr,sizeof(client_addr));
    if(nbyte<0){
        perror("Send Fail");
        exit(0);
    }
    
    memset(buf,0,MAXLINE+1);
    
    pthread_exit(NULL);
    close(accp_sock);
    
}
