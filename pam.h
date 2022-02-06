#ifndef PAM_H_
#define PAM_H_

#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>

int memory_init() {
    key_t key = ftok(".", 'A');
    if(key == -1) {
        printf("\nWystapil blad podczas tworzenia klucza\n");
        exit(EXIT_FAILURE);
    }

    int mem_id = shmget(key, 1, 0600|IPC_CREAT);
    if(mem_id == -1) {
        perror("\nWystapil blad podczas wydzielania pamieci: ");
        exit(EXIT_FAILURE);
    }
    
    return mem_id;
}

char* memory_attach(int memory_id) {
    char* adress = shmat(memory_id, 0, 0);
    if(*adress < 0) {
        perror("\nWystapil blad podczas dolaczania pamieci: ");
        exit(EXIT_FAILURE);
    }

    return adress;
}

void memory_unattach(char* adress, int memory_id) {
    int rem = shmctl(memory_id, IPC_RMID, 0);   // odznaczenie segmentu pamieci do usuniecia
    if(rem < 0) {
        perror("\nWystapil blad zwiazany z usunieciem pamieci: ");
    }

    int unattach = shmdt(adress);   // sukces 0, porazka -1
    if(unattach < 0) {
        perror("\nWystapil blad zwiazany z odlaczeniem pamieci: ");
    }
}

#endif 