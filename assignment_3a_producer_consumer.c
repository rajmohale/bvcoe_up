#include <stdio.h> 
#include <semaphore.h> 
#include <pthread.h> 
#include <unistd.h> 
#include <sys/types.h> 
// Shared buffer of size 10 
char buff[10]; 
// Declare semaphores 
sem_t mutex, empty, full; 
/* 
Producer function 
Produces 10 items and puts them in the buffer 
*/ 
void* produce(void* arg) 
{ 
    int i; 
    printf("\nInside Producer"); 
    for (i = 0; i < 10; i++) 
    { 
        sem_wait(&empty);      // Wait if buffer is full (decrement empty slots) 
        sem_wait(&mutex);      // Enter critical section (lock buffer) 

        buff[i] = i;           // Produce an item (store in buffer) 
        printf("\nProduced Item : %d", buff[i]); 
        sem_post(&mutex);      // Exit critical section (unlock buffer) 
        sem_post(&full);       // Increment count of full slots 
        sleep(1);              // Simulate production time 
    } 
    pthread_exit("produce\n"); // Exit thread with status message 
} 
/* 
Consumer function 
Consumes 10 items from the buffer 
*/ 
void* consumer(void* arg) 
{ 
    int j; 
    printf("\nInside Consumer"); 
    for (j = 0; j < 10; j++) 
    { 
        sem_wait(&full);       // Wait if buffer is empty (decrement full slots) 
        sem_wait(&mutex);      // Enter critical section (lock buffer) 
        // Consume the item 
        int item = buff[j]; 
        printf("\nConsumed Item : %d", item); 
        sem_post(&mutex);      // Exit critical section (unlock buffer) 
        sem_post(&empty);      // Increment count of empty slots 
        sleep(1);              // Simulate consumption time 
    } 
    pthread_exit("consumer\n"); // Exit thread with status message 

} 
int main() 
{ 
    pthread_t tid1, tid2;      // Thread IDs 
    // Initialize semaphores 
    sem_init(&empty, 0, 10);   // Initially 10 empty slots 
    sem_init(&full, 0, 0);     // Initially 0 full slots 
    sem_init(&mutex, 0, 1);    // Binary semaphore (mutex lock) 
    void *status;              // To store exit status of threads 
    // Create producer and consumer threads 
    pthread_create(&tid1, NULL, produce, NULL); 
    pthread_create(&tid2, NULL, consumer, NULL); 
    // Wait for producer to finish 
    pthread_join(tid1, &status); 
    printf("\nThe exited status %s\n", (char*)status); 
    // Wait for consumer to finish 
    pthread_join(tid2, &status); 
    printf("\nThe exited status %s\n", (char*)status); 
    return 0; 
}
