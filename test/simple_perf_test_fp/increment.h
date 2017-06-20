#include <stdio.h>

typedef void(*Fp)(void);
extern Fp fp;

void increment(void);
void fake_increment(void);
int Main(int argc, char** argv);
int MainFp(int argc, char** argv);
