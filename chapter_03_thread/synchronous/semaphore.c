#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define PRODUCER_COUNT 2
#define CONSUMER_COUNT 2
#define PRODUCE_ITEMS 10

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

void *producer(void *arg) 
{
    int id = *((int *)arg);
    for (int i = 0; i < PRODUCE_ITEMS; i++) {
        int item = id * 100 + i;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        printf("Producer %d: Produced %d\n", id, item);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(rand() % 3);
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) 
{
    int id = *((int *)arg);
    for (int i = 0; i < PRODUCE_ITEMS; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        printf("Consumer %d: Consumed %d\n", id, item);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(rand() % 3);
    }
    pthread_exit(NULL);
}

int main() 
{
    pthread_t producers[PRODUCER_COUNT];
    pthread_t consumers[CONSUMER_COUNT];
    int producer_ids[PRODUCER_COUNT];
    int consumer_ids[CONSUMER_COUNT];

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    
    for (int i = 0; i < PRODUCER_COUNT; i++) {
        producer_ids[i] = i + 1;
        if (pthread_create(&producers[i], NULL, producer, &producer_ids[i]) != 0) {
            perror("Failed to create producer thread");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < CONSUMER_COUNT; i++) {
        consumer_ids[i] = i + 1;
        if (pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]) != 0) {
            perror("Failed to create consumer thread");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < PRODUCER_COUNT; i++) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < CONSUMER_COUNT; i++) {
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    printf("All producers and consumers have finished.\n");
    return 0;
}
