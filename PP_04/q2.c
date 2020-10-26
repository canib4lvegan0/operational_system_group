//#include <malloc.h>
//#include <stdlib.h>
//#include <stddef.h>
//#include <pthread.h>
#include "utility.h"
//#include <stdio.h>
//#include <semaphore.h>
#include <unistd.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

#define DELAY 1            // second(s) - interval between one action and other
#define OFFSETS_TABS "\t\t\t\t\t\t"


buffer_t B1, B2;                // buffer 1 and buffer 2 (queues)
pthread_mutex_t m_B1, m_B2;     // mutexes for buffer 1 and buffer 2
sem_t B1_items, B1_spaces;      // semaphores for items and spaces of the buffer 1, respectively
sem_t B2_items, B2_spaces;      // semaphores for items and spaces of the buffer 2, respectively


/* Thread producer X - write in buffer 1 */
void *producerX(void *id){
    char data;

    for(;;){
        sleep(DELAY);

        data = (char) (65 + ((int)random() % 26));      // takes a random character

        sem_wait(&B1_spaces);                           // wait for available space, then decrement mutex
        pthread_mutex_lock(&m_B1);                      // enter critical region - buffer 1

        buffer_append(&B1, data);                       // append data to the buffer
        printf("'%c' has appended at Buffer 1 - [producerX th%d]\n", data, (int)id);
        buffer_print(&B1, "B1");                 // print current buffer

        pthread_mutex_unlock(&m_B1);                    // leave critical region - buffer 1
        sem_post(&B1_items);                            // increment items mutex
    }
    pthread_exit(NULL);                          // not necessary
}

/* Thread producer and consumer Y - read buffer 1 and write in buffer 2 */
void *producer_consumerY(void *id){
    char data;

    for(;;){
        sleep(DELAY);

        sem_wait(&B1_items);                            // wait for available items, then decrement mutex
        pthread_mutex_lock(&m_B1);                      // enter critical region - buffer 2

        buffer_remove(&B1, &data);                      // remove data to the buffer
        printf("%s%c has removed at Buffer 1 - [consumerY th%d]\n", OFFSETS_TABS, data, (int) id);

        pthread_mutex_unlock(&m_B1);                    // leave critical region - buffer 1
        sem_post(&B1_spaces);                           // increment available items

        sleep(DELAY);

        sem_wait(&B2_spaces);                           // wait for available space, then decrement mutex
        pthread_mutex_lock(&m_B2);                      // enter critical region - buffer 2

        buffer_append(&B2, data);                               // append data to the buffer
        printf("%s%c has appended at Buffer 2 - [producerY th%d]\n", OFFSETS_TABS, data, (int) id);
        printf("%s", OFFSETS_TABS);
        buffer_print(&B2, "B2");                 // print current buffer

        pthread_mutex_unlock(&m_B2);                    // leave critical region - buffer 1
        sem_post(&B2_items);                            // increment items mutex
    }
    pthread_exit(NULL);
}

/* Thread consumer Z - read buffer 2 */
void *consumerZ(void *id) {
    int i;
    char data;

    for (;;) {
        sleep(DELAY);

        sem_wait(&B2_items);                            // wait for available items, then decrement mutex
        pthread_mutex_lock(&m_B2);                      // enter critical region - buffer 2

        buffer_remove(&B2, &data);                      // remove data to the buffer
        printf("%s%s%c has removed at Buffer 2 - [consumerZ th%d]\n", OFFSETS_TABS, OFFSETS_TABS, data, (int) id);

        pthread_mutex_unlock(&m_B2);                    // leave critical region - buffer 2
        sem_post(&B2_spaces);                           // increment available items
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    if(argc != 6){
        printf("Invalid param!\n");
        printf("num_thX n_thY n_thZ sz_B1 sz_B2\n");
        exit(-1);
    }

    /* Take the number of threads and buffers size by command line and set them */
    int n_thX = atoi(argv[1]);
    int n_thY = atoi(argv[2]);
    int n_thZ = atoi(argv[3]);
    int sz_B1 = atoi(argv[4]);
    int sz_B2 = atoi(argv[5]);

    /* */
    pthread_t threadsX[n_thX], threadsY[n_thY], threadsZ[n_thZ];

    /* starts buffer mutexes */
    pthread_mutex_init(&m_B1, NULL);
    pthread_mutex_init(&m_B2, NULL);

    /* starts semaphores */
    sem_init(&B1_items, 0, 0);
    sem_init(&B1_spaces, 0, sz_B1);
    sem_init(&B2_items, 0, 0);
    sem_init(&B2_spaces, 0, sz_B2);

    /* starts buffers */
    buffer_init(&B1, sz_B1);
    buffer_init(&B2, sz_B2);

    /* creating threads for producer X */
    for(int i = 0; i < n_thX; i++) {
        if(pthread_create(&threadsX[i], NULL, producerX, (void *)i)){
            perror("pthread_create");
            exit(-1);
        }
    }

    /* creating threads for consumer-producer Y */
    for(int i = 0; i < n_thY; i++) {
        if(pthread_create(&threadsY[i], NULL, producer_consumerY, (void *)i)){
            perror("pthread_create");
            exit(-1);
        }
    }

    /* creating threads for consumer Z */
    for(int i = 0; i < n_thZ; i++) {
        if(pthread_create(&threadsZ[i], NULL, consumerZ, (void *)i)){
            perror("pthread_create");
            exit(-1);
        }
    }

/*
    sem_destroy(&B1_items);
    sem_destroy(&B1_spaces);
    sem_destroy(&B2_spaces);
    sem_destroy(&B2_items);
    pthread_mutex_destroy(&m_B1);
    pthread_mutex_destroy(&m_B2);
*/
    pthread_exit(NULL);
}

#pragma clang diagnostic pop