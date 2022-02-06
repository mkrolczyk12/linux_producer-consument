#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/errno.h>

#define NUMBER_OF_SEM 2

int semafor_init() {
    key_t key = ftok(".", 'B');
    if(key == -1) {
        printf("\nWystapil blad podczas tworzenia klucza\n");
        exit(EXIT_FAILURE);
    }

    int semafor = semget(key, NUMBER_OF_SEM, IPC_CREAT | 0600);
    if(semafor == -1) {
        perror("\nWystapil blad podczas uzyskiwania dostepu do semafora\n");
        exit(EXIT_FAILURE);
    }

    return semafor;
}

void semafor_prepare(int id) {
    for (int i = 0; i < NUMBER_OF_SEM; i++) {
        int status = semctl(id, i, SETVAL, 0);
        if (status == -1) {
            perror("\nNie mozna ustawic semafora: ");
            exit(EXIT_FAILURE);
        }
    }
}

int semafor_check() {
    key_t key = ftok(".", 'B');

    return semget(key, NUMBER_OF_SEM, IPC_CREAT | IPC_EXCL | 0600);
}

void semafor_p(int id, int sem) {
    struct sembuf bufor;
    bufor.sem_num = sem;
    bufor.sem_op = -1;
    bufor.sem_flg = 0;

    int status = semop(id, &bufor, 1);

    if (status == -1) {
        if(errno == EINTR) {
            perror("\nPrzerwanie podczas zamykania semafora ");
            printf("\nPonowienie proby zamkniecia...\n");
            semafor_p(id, sem);
        }

        else {
            perror("\nWystapil blad podczas zamykania semafora: ");
            exit(EXIT_FAILURE);
        }
    }
}

void semafor_v(int id, int sem) {
    struct sembuf bufor;
    bufor.sem_num = sem;
    bufor.sem_op = 1;
    bufor.sem_flg = IPC_NOWAIT;

    int status = semop(id, &bufor, 1);

    if (status == -1) {
        if(errno == EAGAIN) {
            perror("\nBlad errno ");
            perror("\nPonowienie proby otwierania semafora...\n");
            semafor_v(id, sem);
        }

        if(errno == EINTR) {
            perror("\nPrzerwanie podczas otwierania semafora");
            printf("\nPonowienie proby otworzenia...\n");
            semafor_v(id, sem);
        }

        else {
            perror("\nWystapil blad podczas otwierania semafora: ");
            exit(EXIT_FAILURE);   
        }
    }      
}

void semafor_remove(int id) {
    int value = semctl(id, 0, IPC_RMID);
    if (value == -1) {
        perror("\nWystapil blad podczas usuwania semafora ");
        exit(EXIT_FAILURE);
    }
}