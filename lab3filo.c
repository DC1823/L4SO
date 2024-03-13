#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t forks[NUM_PHILOSOPHERS];
sem_t mutex;

void *philosopher(void *arg) {
    int id = *((int *)arg);
    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;

    while (1) {
        // Pensar
        printf("Philosopher %d is thinking\n", id);
        usleep(1000000);

        // Esperar turno para tomar los tenedores
        sem_wait(&mutex);

        // Tomar el tenedor izquierdo
        pthread_mutex_lock(&forks[left_fork]);
        printf("Philosopher %d took fork %d\n", id, left_fork);

        // Tomar el tenedor derecho
        pthread_mutex_lock(&forks[right_fork]);
        printf("Philosopher %d took fork %d\n", id, right_fork);

        sem_post(&mutex);

        // Comer
        printf("Philosopher %d is eating\n", id);
        usleep(1000000);

        // Liberar los tenedores
        pthread_mutex_unlock(&forks[left_fork]);
        pthread_mutex_unlock(&forks[right_fork]);
    }
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    sem_init(&mutex, 0, 1);

    // Inicializar los tenedores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Crear hilos para los filósofos
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Esperar a que los filósofos terminen
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Liberar recursos
    sem_destroy(&mutex);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    return 0;
}
