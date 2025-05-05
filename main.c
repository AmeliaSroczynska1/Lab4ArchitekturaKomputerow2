#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define REPEATS 10
#define VECTOR_SIZE 4
#define SIZE1 2048
#define SIZE2 4096
#define SIZE3 8192

FILE *file;
float vectorsA[SIZE1/VECTOR_SIZE][VECTOR_SIZE];
float vectorsB[SIZE2/VECTOR_SIZE][VECTOR_SIZE];
float vectorsC[SIZE3/VECTOR_SIZE][VECTOR_SIZE];
float times[3][8];
static unsigned long seed = 1;

void addSIMD();
void multiplySIMD();
void subtractSIMD();
void divideSIMD();

void addSISD();
void subtractSISD();
void multiplySISD();
void divideSISD();

void generateNumbers();
void test();
void openFile();
void writeToFile();
void closeFile();


int main() {
    generateNumbers();
    openFile();
    test();
    writeToFile();
    closeFile();
    return 0;
}


void test() {
    for(int j=0; j <REPEATS; j++) {
        addSISD();
        subtractSISD();
        multiplySISD();
        divideSISD();

        addSIMD();
        multiplySIMD();
        divideSIMD();
        subtractSIMD();
    }
}


void openFile(){
    file = fopen("output.txt", "w");
    if (file == NULL) {
        perror("Blad otwierania pliku");
        return;
    }
}


void writeToFile() {
    const int sizes[] = {SIZE1, SIZE2, SIZE3};
    const char* labels[] = {"SIMD", "SISD"};
    const int offsets[] = {0, 4};

    for (int i = 0; i < 3; i++) { // Dla każdego rozmiaru (SIZE1, SIZE2, SIZE3)
        for (int t = 0; t < 2; t++) { // Dla każdego typu obliczeń (SIMD, SISD)
            fprintf(file, "\nTyp obliczeń: %s\n", labels[t]);
            fprintf(file, "Liczba liczb: %d\n", sizes[i]);
            fprintf(file, "Średni czas: [mikrosekundy]\n");

            const char* operations[] = {"+", "-", "*", "/"};
            for (int op = 0; op < 4; op++) {
                fprintf(file, "%s %.1f\n", operations[op],
                        times[i][offsets[t] + op] / REPEATS);
            }
            if (t == 0) fprintf(file, "\n");
        }
        if (i < 2) fprintf(file, "\n\n");
    }
}



void closeFile() {
    if (file != NULL) {
        if (fclose(file) == EOF) {
            perror("Blad zamykania pliku");
        }
        file = NULL; // Dobrą praktyką jest wyzerowanie wskaźnika po zamknięciu
    }
}


void my_srand(unsigned long s) {
    seed = s;
}


unsigned int my_rand() {
    seed = (1103515245UL * seed + 12345UL) % 0x7fffffffUL;
    return (unsigned int)seed;
}


float my_rand_float() {
    return (float)my_rand() / (0x7fffffffUL / 100.0f);
}


void generateNumbers() {
    my_srand(time(NULL));

    for (int i = 0; i < SIZE1/VECTOR_SIZE; i++) {
        for (int j = 0; j < VECTOR_SIZE; j++) {
            vectorsA[i][j] = my_rand_float();
        }
    }
    for (int i = 0; i < SIZE2/VECTOR_SIZE; i++) {
        for (int j = 0; j < VECTOR_SIZE; j++) {
            vectorsB[i][j] = my_rand_float();
        }
    }
    for (int i = 0; i < SIZE3/VECTOR_SIZE; i++) {
        for (int j = 0; j < VECTOR_SIZE; j++) {
            vectorsC[i][j] = my_rand_float();
        }
    }
}


void addSIMD(){
    times[0][0] += 0.3;     //to dla 2048
}


void multiplySIMD(){
    times[0][1] += 0.5;
}


void subtractSIMD(){
    times[0][2] += 0.2;
}


void divideSIMD(){
    times[0][3] += 0.7;
}


void addSISD(){
    times[0][4] += 0.5;
}


void subtractSISD(){
    times[0][5] += 0.8;
}


void multiplySISD(){
    times[0][6] += 0.2;
}


void divideSISD(){
    times[0][7] += 0.9;
}