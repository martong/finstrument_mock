/*
    Copyright 2007-2008 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
    
    Shared source file for timing, used by all the benchmarks
*/

/******************************************************************************/

#include <time.h>

/******************************************************************************/

/* Yes, this would be easier with a class or vector
 but it needs to work for both C and C++ code
*/

/*  simple timer functions */
clock_t start_time, end_time;

void start_timer() { start_time = clock(); }

double timer() {
  end_time = clock();
  return (end_time - start_time)/ (double)(CLOCKS_PER_SEC);
}

extern "C" {

void __cyg_profile_func_enter(void *this_fn, void *call_site)
    __attribute__((no_instrument_function));
void __cyg_profile_func_enter(void *this_fn, void *call_site) {
    // printf("ENTER: %p, from %p\n", this_fn, call_site);
} /* __cyg_profile_func_enter */

void __cyg_profile_func_exit(void *this_fn, void *call_site)
    __attribute__((no_instrument_function));
void __cyg_profile_func_exit(void *this_fn, void *call_site) {
    // printf("EXIT:  %p, from %p\n", this_fn, call_site);
} /* __cyg_profile_func_enter */

}

/******************************************************************************/
