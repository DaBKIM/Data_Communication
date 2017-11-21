#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#define MAXLINE 1024 //buf 크기

void *thread_recv(void *arg); //쓰레드 시작 함수
void *thread_send(void *arg);
int listen_sock, accp_sock;
pthread_t thr_id[2];
struct sockaddr_in server_addr, client_addr;
pthread_mutex_t mutex;
pthread_cond_t count_threshold_cv;

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
        
       // pthread_mutex_lock(&mutex);
        if((status = pthread_create(&thr_id[0], NULL,&thread_recv, &accp_sock))!= 0) { // 수신 스레드
            printf("#1 Thread create error: %s\n", strerror(status));
            exit(0);
        }
        //pthread_mutex_unlock(&mutex);
        //printf("2\n");
    
    
    //인자로 지정한 스레드 id가 종료하기를 기다립니다.
        //printf("3\n");
        pthread_join(thr_id[0], NULL);
        
        if((status = pthread_create(&thr_id[1], NULL,&thread_send, &accp_sock))!= 0) { // 송신 스레드
            printf("#2 Thread create error: %s\n", strerror(status));
            exit(0);
        }
        
        pthread_join(thr_id[1], NULL);
        

    }
    
    close(accp_sock);
    return 0;
}

char buf[MAXLINE+1];
int nbyte;

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
    
    buf[nbyte]='\0';
    printf("Recv from 'Client' : %s\n",buf);
    
    //pthread_mutex_lock(&mutex);
    
    //pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
    close(accp_sock);

}

void *thread_send(void *arg) { //송신 스레드 함수
    
    int accp_sock=(int) *((int*) arg);
    int addrlen;
    
    addrlen=sizeof(client_addr);
    
    nbyte=sendto(accp_sock,buf,nbyte,0,(struct sockaddr*)&client_addr,sizeof(client_addr));
    if(nbyte<0){
        perror("Send Fail");
        exit(0);
    }
    printf("Send to 'Client' : %s\n",buf);
        
    memset(buf,0,MAXLINE+1);

    pthread_exit(NULL);
    close(accp_sock);
    
}
