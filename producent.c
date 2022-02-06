#include "sem.h"
#include "pam.h"

#include <stdlib.h>
#include <time.h>

int main() {
    FILE* file = fopen("input_file", "r");  // otworz plik do czytania

    if(file == NULL) {
        printf("\nBlad otwierania pliku\n");
        exit(EXIT_FAILURE);
    }    
    int sem_id;
    
    // zainicjalizowanie semaforow
    if(semafor_check() > -1) {    // sprawdz semafory (zwraca blad, jezeli dla ustalonego klucza istnieje juz zbior semaforow) 
        sem_id = semafor_init();    // inicjalizacja semaforow
        semafor_prepare(sem_id);
        semafor_v(sem_id, 1);
        printf("Oczekiwanie na konsumenta...\n");
        semafor_p(sem_id, 0);
    }

    else {
        sem_id = semafor_init();
        semafor_p(sem_id, 0);
        printf("Producent zostal przylaczony\n");
        semafor_v(sem_id, 1);
    }

    int mem_id = memory_init(); // zainicjalizowanie pamieci

    char* mem_adress = memory_attach(mem_id);    // przylaczenie do segmentu pamieci

    // -------------------SYMULACJA ROZNYCH PREDKOSCI DZIALANIA PROGRAMOW-------------------
    // srand(time(NULL));
    // int rand_time = rand() % 3;
    // -------------------SYMULACJA ROZNYCH PREDKOSCI DZIALANIA PROGRAMOW-------------------

    // wczytywanie danych z pliku do pamieci
    char data = 0;
    while (data != EOF) {
        semafor_p(sem_id, 0);   // oczekiwanie na gotowosc na wysylanie danych
        data = fgetc(file);
        *mem_adress = data;          // wysylanie

        // ------------------- SYMULACJA ROZNYCH PREDKOSCI DZIALANIA PROGRAMOW c.d -------------------
        // printf("Odczytano z pliku: %c\n", data);
        // printf("Zapisano w pamieci: '%c'\n", *mem_adress);
        // sleep(rand_time);
        // ------------------- SYMULACJA ROZNYCH PREDKOSCI DZIALANIA PROGRAMOW c.d -------------------

        semafor_v(sem_id, 1);   // informacja o przeslaniu danych
    }

    semafor_p(sem_id, 0);

    fclose(file);
    memory_unattach(mem_adress, mem_id);   // odlaczenie pamieci dzielonej
    semafor_remove(sem_id);
    
    return 0;
}   