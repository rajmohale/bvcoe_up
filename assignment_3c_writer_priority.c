#include<stdio.h> 
#include<pthread.h> 
#include<unistd.h> 
 
void* reader(void*); 
void* writer(void*); 
int writebuff(); 
void readbuff(int buffer); 
 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
 
int flag=0; 
int buff; 
int main() 
{ 
    pthread_t rid; 
    pthread_t wid; 
    pthread_create(&wid,NULL,writer,NULL); 
    pthread_create(&rid,NULL,reader,NULL); 
    pthread_join(wid,NULL); 
    pthread_join(rid,NULL); 
    pthread_mutex_destroy(&lock); 
    return 0; 
} 
void* reader(void* par) 

{ 
    while(1) 
    { 
        pthread_mutex_lock(&lock); 
        if(flag == 1) 
        { 
            readbuff(buff); 
            flag = 0; 
        } 
        pthread_mutex_unlock(&lock); 
        usleep(10000); 
        pthread_exit(NULL); 
    } 
} 
void* writer(void* par) 
{ 
    while(1) 
    { 
        pthread_mutex_lock(&lock); 
        if(flag == 0) 
        { 
            buff = writebuff(); 
            flag = 1; 
        } 
        pthread_mutex_unlock(&lock); 
        usleep(10000); 
    } 
    pthread_exit(NULL); 

} 
int writebuff() 
{ 
    int wv; 
    printf("Writer is writing in Buffer\n"); 
    scanf("%d",&wv); 
    return wv; 
} 
void readbuff(int buffer) 
{ 
    printf("Reader is reading from buffer = %d\n",buffer); 
}
