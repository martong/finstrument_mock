#include "increment.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int a = 0;
int b = 0;

void increment(void) {
    ++a;
    ++b;
    ++b;
}

void fake_increment(void) {
    ++a;
    ++a;
    ++b;
}

void loop(int max) {
    for (int i = 0; i < max; ++i) {
        increment();
    }
}

void loopFp(int max) {
    for (int i = 0; i < max; ++i) {
        fp();
    }
}

void print() {
    printf("%d %d\n", a, b);
}

clock_t start_time, end_time;
void start_timer() { start_time = clock(); }
double timer() {
  end_time = clock();
  return (end_time - start_time)/ (double)(CLOCKS_PER_SEC);
}

int Main(int argc, char** argv) {
    if (argc >= 2) {
        int max = atoi(argv[1]);
        start_timer();
        loop(max);
        print();
        printf("Took %5.3f sec\n", timer());
    } else {
        fprintf(stderr,"error\n");
    }
    return 0;
}

int MainFp(int argc, char** argv) {
    if (argc >= 2) {
        int max = atoi(argv[1]);
        start_timer();
        loopFp(max);
        print();
        printf("Took %5.3f sec\n", timer());
    } else {
        fprintf(stderr,"error\n");
    }
    return 0;
}
