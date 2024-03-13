#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#define NUM_PROCESOS 5
#define NUM_RECURSOS 3

int maximo[NUM_PROCESOS][NUM_RECURSOS] = {
    {7, 5, 3},
    {3, 2, 2},
    {9, 0, 2},
    {2, 2, 2},
    {4, 3, 3}
};

int asignado[NUM_PROCESOS][NUM_RECURSOS] = {0};
int necesidad[NUM_PROCESOS][NUM_RECURSOS];
int disponibles[NUM_RECURSOS] = {3, 3, 2};
bool finalizado[NUM_PROCESOS] = {false};

void calcularNecesidad() {
    for (int i = 0; i < NUM_PROCESOS; ++i) {
        for (int j = 0; j < NUM_RECURSOS; ++j) {
            necesidad[i][j] = maximo[i][j] - asignado[i][j];
        }
    }
}

bool solicitarRecursos(int proceso, int recursos[]) {
    for (int i = 0; i < NUM_RECURSOS; ++i) {
        if (recursos[i] > disponibles[i] || recursos[i] > necesidad[proceso][i])
            return false;
    }

    for (int i = 0; i < NUM_RECURSOS; ++i) {
        disponibles[i] -= recursos[i];
        asignado[proceso][i] += recursos[i];
        necesidad[proceso][i] -= recursos[i];
    }

    return true;
}

void liberarRecursos(int proceso) {
    for (int i = 0; i < NUM_RECURSOS; ++i) {
        disponibles[i] += asignado[proceso][i];
        asignado[proceso][i] = 0;
        necesidad[proceso][i] = maximo[proceso][i]; // Reiniciamos la necesidad al máximo
    }
}

bool esSeguro() {
    int trabajo[NUM_RECURSOS];
    bool terminado[NUM_PROCESOS];

    for (int i = 0; i < NUM_RECURSOS; ++i) {
        trabajo[i] = disponibles[i];
    }

    for (int i = 0; i < NUM_PROCESOS; ++i) {
        terminado[i] = finalizado[i];
    }

    int contador = 0;
    while (contador < NUM_PROCESOS) {
        bool asignadoRecursos = false;
        for (int i = 0; i < NUM_PROCESOS; ++i) {
            if (!terminado[i]) {
                bool sePuedeAsignar = true;
                for (int j = 0; j < NUM_RECURSOS; ++j) {
                    if (necesidad[i][j] > trabajo[j]) {
                        sePuedeAsignar = false;
                        break;
                    }
                }
                if (sePuedeAsignar) {
                    for (int j = 0; j < NUM_RECURSOS; ++j) {
                        trabajo[j] += asignado[i][j];
                    }
                    terminado[i] = true;
                    asignadoRecursos = true;
                    contador++;
                }
            }
        }
        if (!asignadoRecursos) {
            return false; // deadlock
        }
    }
    return true; // no deadlock
}

void imprimirEstado() {
    printf("Estado actual:\n");
    printf("Recursos disponibles: %d %d %d\n", disponibles[0], disponibles[1], disponibles[2]);
    printf("Asignado     Maximo       Necesidad\n");
    for (int i = 0; i < NUM_PROCESOS; ++i) {
        printf("Proceso %d: ", i);
        for (int j = 0; j < NUM_RECURSOS; ++j) {
            printf("%d ", asignado[i][j]);
        }
        printf("    ");
        for (int j = 0; j < NUM_RECURSOS; ++j) {
            printf("%d ", maximo[i][j]);
        }
        printf("    ");
        for (int j = 0; j < NUM_RECURSOS; ++j) {
            printf("%d ", necesidad[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    srand(time(NULL)); // Inicializamos la semilla aleatoria

    calcularNecesidad();
    imprimirEstado();

    // Simulación de solicitudes y liberaciones de recursos
    for (int i = 0; i < 10; ++i) {
        int proceso = rand() % NUM_PROCESOS; // Seleccionamos un proceso aleatorio
        int recursos[NUM_RECURSOS];
        for (int j = 0; j < NUM_RECURSOS; ++j) {
            recursos[j] = rand() % (necesidad[proceso][j] + 1); // Solicitamos recursos aleatorios
        }

        printf("Proceso %d solicita recursos: %d %d %d\n", proceso, recursos[0], recursos[1], recursos[2]);

        if (solicitarRecursos(proceso, recursos)) {
            if (esSeguro()) {
                printf("Recursos asignados de manera segura.\n");
            } else {
                printf("Recursos asignados, pero podría haber un deadlock.\n");
            }
        } else {
            printf("No se pueden asignar recursos al proceso.\n");
        }

        imprimirEstado();

        // Simulamos la liberación de recursos después de un tiempo
        int tiempoEspera = rand() % 3 + 1; // Esperamos entre 1 y 3 segundos
        printf("Esperando %d segundos antes de liberar recursos...\n", tiempoEspera);
        sleep(tiempoEspera);

        liberarRecursos(proceso);
        printf("Recursos liberados por el proceso %d.\n", proceso);

        imprimirEstado();
    }

    return 0;
}
