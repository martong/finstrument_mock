#include "increment.h"

Fp fp;

int main(int argc, char** argv) {
    fp = &fake_increment;
    return MainFp(argc, argv);
}
