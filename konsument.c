#include "sem.h"
#include "pam.h"

#include <stdlib.h>
#include <time.h>

int main() {
    FILE* file = fopen("output_file", "w"); // otworz plik do pisania

    if(file == NULL) {
        printf("\nBlad otwierania pliku\n");
        exit(EXIT_FAILURE);
    }    
    int sem_id;

    // zainicjalizowanie semaforow
    if(semafor_check() > -1) {    // sprawdz semafory (zwraca blad, jezeli dla ustalonego klucza istnieje juz zbior semaforow) 
        sem_id = semafor_init();    // inicjalizuj semafory
        semafor_prepare(sem_id);
        semafor_v(sem_id, 0);
        printf("Oczekiwanie na producenta...\n");
        semafor_p(sem_id, 1);
    }

    else {
        sem_id = semafor_init();
        semafor_p(sem_id, 1);
        printf("Konsument zostal przylaczony\n");
        semafor_v(sem_id, 0);
    }

    int mem_id = memory_init(); // zainicjalizowanie pamieci
    
    char* mem_adress = memory_attach(mem_id);    // przylaczenie do segmentu pamieci

    semafor_v(sem_id, 0);   // zasygnalizowanie gotowosci do przyjmowania informacji

    // -------------------SYMULACJA ROZNYCH PREDKOSCI DZIALANIA PROGRAMOW-------------------
    // srand(time(NULL));
    // int rand_time = rand() % 3;
    // -------------------SYMULACJA ROZNYCH PREDKOSCI DZIALANIA PROGRAMOW-------------------

    // wczytywanie danych z pamieci dzielonej
    while (1) {
        semafor_p(sem_id, 1);   // oczekiwanie na informacje o przeslaniu danych przez producenta

        if(*mem_adress == EOF) {     // sprawdzenie czy to nie jest koniec pliku
            semafor_v(sem_id, 0);   // informacja o zakonczeniu wczytywania danych przez konsumenta
            break;
        }

        fprintf(file, "%c", *mem_adress);

        // ------------------- SYMULACJA ROZNYCH PREDKOSCI DZIALANIA PROGRAMOW c.d -------------------
        // printf("Odczytano z pamieci: %c\n", *mem_adress);
        // printf("Zapisano w pliku wyjsciowym: '%c'\n", *mem_adress);
        // sleep(rand_time);
        // ------------------- SYMULACJA ROZNYCH PREDKOSCI DZIALANIA PROGRAMOW c.d -------------------

        semafor_v(sem_id, 0);           // informacja o wczytaniu danych i zasygnalizowanie gotowosci na odbior danych
    }
    
    fclose(file);
    memory_unattach(mem_adress, mem_id);   // odlaczenie pamieci dzielonej

    return 0;
}   