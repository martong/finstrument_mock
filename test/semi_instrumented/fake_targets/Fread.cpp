#include <stdio.h>
#include "Fread.hpp"

double Fread(void) {
    // double a[SIZE] = {1.,2.,3.,4.,5.};
    // FILE *fp = fopen("test.bin", "wb"); // must use binary mode
    // fwrite(a, sizeof *a, SIZE, fp); // writes an array of doubles
    // fclose(fp);

    double b[SIZE];
    FILE* fp = fopen("test.bin", "rb");
    if (!fp) {
        throw FerrorE{};
    }
    size_t ret_code =
        fread(b, sizeof *b, SIZE, fp); // reads an array of doubles
    double result = 0.0;
    if (ret_code == SIZE) {
        puts("Array read successfully, contents: ");
        for (int n = 0; n < SIZE; ++n) {
            printf("%f ", b[n]);
            result += b[n];
        }
        putchar('\n');
    } else { // error handling
        if (feof(fp)) {
            printf("Error reading test.bin: unexpected end of file\n");
            throw FeofE{};
        } else if (ferror(fp)) {
            perror("Error reading test.bin");
            throw FerrorE{};
        }
    }

    fclose(fp);
    return result;
}
