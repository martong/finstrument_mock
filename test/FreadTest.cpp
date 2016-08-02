#include <stdio.h>
#include <cassert>
#include <array>
#include <numeric>

#include "FooFixture.hpp"
#include "Fread.hpp"

namespace t1 {
FILE f;

FILE *fake_fopen(const char *, const char *) { return &f; }

std::array<double, SIZE> a = {1., 2., 3., 4., 5.};
size_t fake_fread(void *buf, size_t, size_t count, FILE *) {
    assert(count == SIZE);
    double *b = reinterpret_cast<double *>(buf);
    for (int i = 0; i < 5; ++i) {
        b[i] = a[i];
    }
    return count;
}

int fake_feof(FILE *) { return 0; }

int fake_ferror(FILE *) { return 0; }

int fake_fclose(FILE *) { return 0; }
} // namespace t1

TEST_F(FooFixture, FreadSumIsOk) {
    ::fake::subs.insert({address(fopen), address(t1::fake_fopen)});
    ::fake::subs.insert({address(fread), address(t1::fake_fread)});
    ::fake::subs.insert({address(feof), address(t1::fake_feof)});
    ::fake::subs.insert({address(ferror), address(t1::fake_ferror)});
    ::fake::subs.insert({address(fclose), address(t1::fake_fclose)});

    auto res = Fread();
    EXPECT_EQ(res, std::accumulate(t1::a.begin(), t1::a.end(), 0.0));
}


namespace t2 {
FILE f;

FILE *fake_fopen(const char *, const char *) { return &f; }

size_t fake_fread(void *buf, size_t, size_t count, FILE *) {
    return 0;
}

int fake_feof(FILE *) { return 1; } // indicate EOF !

int fake_ferror(FILE *) { return 0; }

int fake_fclose(FILE *) { return 0; }
} // namespace t2

TEST_F(FooFixture, FreadHandles_feof) {
    ::fake::subs.insert({address(fopen), address(t2::fake_fopen)});
    ::fake::subs.insert({address(fread), address(t2::fake_fread)});
    ::fake::subs.insert({address(feof), address(t2::fake_feof)});
    ::fake::subs.insert({address(ferror), address(t2::fake_ferror)});
    ::fake::subs.insert({address(fclose), address(t2::fake_fclose)});

    EXPECT_THROW(Fread(), FeofE);
}
