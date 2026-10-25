#include <stdio.h> 
#include <pthread.h> 
#include <unistd.h> 
 
// Function prototypes 
void* reader(void*); 
void* writer(void*); 
int writebuff(); 
void readbuff(int buffer); 
 
// Shared buffer 
int buff = 0; 
 
// Synchronization variables 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t can_read = PTHREAD_COND_INITIALIZER; 
pthread_cond_t can_write = PTHREAD_COND_INITIALIZER; 
 
// State variables 
int read_count = 0;       // Number of active readers 
int writer_active = 0;    // 1 if a writer is active, else 0 
 
// Reader thread function 

void* reader(void* par) 
{ 
    while(1) 
    { 
        pthread_mutex_lock(&lock); 
        // Wait while a writer is active 
        while (writer_active) 
        { 
            pthread_cond_wait(&can_read, &lock); 
        } 
        // One more reader entered 
        read_count++; 
        pthread_mutex_unlock(&lock); 
 
        // ----- Critical section (Reading) ----- 
        readbuff(buff); 
        usleep(500000); // Simulate reading time 
        // --------------------------------------- 
        pthread_mutex_lock(&lock); 
        read_count--; 
 
        // If last reader left, notify writers 
        if (read_count == 0) 
        { 
            pthread_cond_signal(&can_write); 
        } 
        pthread_mutex_unlock(&lock); 
        usleep(100000); // Let others get chance 

    } 
    return NULL; 
} 
// Writer thread function 
void* writer(void* par) 
{ 
    while (1) 
    { 
        pthread_mutex_lock(&lock); 
 
        // Wait until no readers and no writer active 
        while (read_count > 0 || writer_active) 
        { 
            pthread_cond_wait(&can_write, &lock); 
        } 
        writer_active = 1; // Mark writer as active 
        pthread_mutex_unlock(&lock); 
 
        // ----- Critical section (Writing) ----- 
        buff = writebuff(); // Get value from user 
        usleep(200000);     // Simulate writing time 
        // -------------------------------------- 
        pthread_mutex_lock(&lock); 
        writer_active = 0; 
        // First give preference to readers 
        pthread_cond_broadcast(&can_read); 
        // If no readers, allow writers 
        pthread_cond_signal(&can_write); 

        pthread_mutex_unlock(&lock); 
        usleep(100000); // Let others get chance 
    } 
    return NULL; 
} 
// Writer takes input from user 
int writebuff() 
{ 
    int wv; 
    printf("Writer: Enter a value to write in buffer: "); 
    scanf("%d", &wv); 
    return wv; 
} 
// Reader reads data from buffer 
void readbuff(int buffer) 
{ 
    printf("Reader is reading value = %d\n", buffer); 
} 
// Main function 
int main() 
{ 
    pthread_t r1, r2, w1; 
    // Create readers and writers 
    pthread_create(&r1, NULL, reader, NULL); 
    pthread_create(&r2, NULL, reader, NULL); 
    pthread_create(&w1, NULL, writer, NULL); 
 
    // Join threads (runs indefinitely) 

    pthread_join(r1, NULL); 
    pthread_join(r2, NULL); 
    pthread_join(w1, NULL); 
 
    // Cleanup 
    pthread_mutex_destroy(&lock); 
    pthread_cond_destroy(&can_read); 
    pthread_cond_destroy(&can_write); 
    return 0; 
}
