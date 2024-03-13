#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_RECURSOS 2

// Semáforos para los dos recursos
sem_t recursos[NUM_RECURSOS];

// Función para el proceso que sigue el orden normal
void *proceso_normal(void *arg) {
    while (1) {
        // Intenta adquirir el recurso 0
        sem_wait(&recursos[0]);
        printf("Proceso normal adquirió recurso 0.\n");
        sleep(1);
        
        // Intenta adquirir el recurso 1
        sem_wait(&recursos[1]);
        printf("Proceso normal adquirió recurso 1.\n");
        
        // Libera los recursos
        sem_post(&recursos[0]);
        sem_post(&recursos[1]);
        
        sleep(1);
    }
    return NULL;
}

// Función para el proceso que invierte el orden
void *proceso_invertido(void *arg) {
    while (1) {
        // Intenta adquirir el recurso 1
        sem_wait(&recursos[1]);
        printf("Proceso invertido adquirió recurso 1.\n");
        sleep(1);
        
        // Intenta adquirir el recurso 0
        sem_wait(&recursos[0]);
        printf("Proceso invertido adquirió recurso 0.\n");

        // Libera los recursos
        sem_post(&recursos[0]);
        sem_post(&recursos[1]);
        
        sleep(1);
    }
    return NULL;
}

int main() {
    // Inicializar semáforos
    for (int i = 0; i < NUM_RECURSOS; ++i) {
        if (sem_init(&recursos[i], 0, 1) != 0) {
            perror("Error al inicializar el semáforo");
            exit(EXIT_FAILURE);
        }
    }

    // Crear los hilos para los procesos
    pthread_t hilo_normal, hilo_invertido;
    if (pthread_create(&hilo_normal, NULL, proceso_normal, NULL) != 0 ||
        pthread_create(&hilo_invertido, NULL, proceso_invertido, NULL) != 0) {
        perror("Error al crear el hilo");
        exit(EXIT_FAILURE);
    }

    // Esperar a que los hilos terminen (esto nunca debería ocurrir)
    pthread_join(hilo_normal, NULL);
    pthread_join(hilo_invertido, NULL);

    // Destruir semáforos
    for (int i = 0; i < NUM_RECURSOS; ++i) {
        sem_destroy(&recursos[i]);
    }

    return 0;
}
