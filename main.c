#include <stdio.h>
#include <time.h>
#include "math.h"
#include <sys/time.h>

long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return milliseconds;
}

static const char* LOGFILE_PATH = "log.txt";
unsigned long long startTimestamp, endTimestamp;
struct timespec spec;


int main(int argc, char *argv[]) {
    if (argc==3) {
        FILE *logFile = fopen(LOGFILE_PATH, "a");
        if (logFile == NULL) {
            printf("Nie udalo się utworzyc logu!");
            return 1;
        }

        startTimestamp = current_timestamp();
        fprintf(logFile, "---------------\n");
        fprintf(logFile, " %lu Start\n", startTimestamp);


        long long size_1, size_2;
        FILE *f1 = fopen(argv[1], "rb");

        if (f1 == NULL) {
            fclose(logFile);
            printf("Nie znaleziono pliku 1!");
            return 1;
        }

        fprintf(logFile, " %lu File 1 opened - %s\n", current_timestamp(), argv[1]);

        FILE *f2 = fopen(argv[2], "rb");
        if (f2 == NULL) {
            fclose(logFile);
            fclose(f1);
            fclose(f2);
            printf("Nie znaleziono pliku 1!");
            return 1;
        }

        fprintf(logFile, " %lu File 2 opened - %s\n", current_timestamp(), argv[1]);

        fseek(f1, 0, SEEK_END);
        size_1 = ftell(f1);
        rewind(f1);

        fseek(f2, 0, SEEK_END);
        size_2 = ftell(f2);
        rewind(f2);

        if (size_1 != size_2) {
            printf("Pliki maja rozna dlugosc!");
            fclose(logFile);
            fclose(f1);
            fclose(f2);
            return 2;
        }

        fprintf(logFile, " %lu Files ok\n", current_timestamp());

        long long errors = 0;
        for (long long i=0; i<size_1; i++) {
            errors += abs((unsigned char)fgetc(f1) - (unsigned char)fgetc(f2));
        }

        endTimestamp = current_timestamp();
        fprintf(logFile, " %lu End\n", endTimestamp);

        fprintf(logFile, "-----Result-----\n", errors);
        fprintf(logFile, "Size: %db\n", size_1 * 8ll);
        fprintf(logFile, "Bit errors: %d\n", errors);
        fprintf(logFile, "BER: %f\n", (double)errors/(double)(size_1 * 8ll));
        fprintf(logFile, "Time: %lums\n", endTimestamp - startTimestamp);
        fprintf(logFile, "-----END-----\n\n", errors);

        printf("Size: %db\n", size_1 * 8ll);
        printf("Bit errors: %d\n", errors);
        printf("BER: %f\n", (double)errors/(double)(size_1 * 8ll));
        printf("Time: %lums\n", endTimestamp - startTimestamp);


    } else {
        printf("Niepoprawna ilosc argumentow!\n");
        return 1;
    }

    return 0;
}

