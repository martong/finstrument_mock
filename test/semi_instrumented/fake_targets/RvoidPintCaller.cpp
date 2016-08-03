void RvoidPint(int);
void RvoidPint_2(int);
void RvoidPint_3(int);
void RvoidPint_4(int);

void RvoidPintCaller() {
  RvoidPint(13);
  RvoidPint_2(15);
}

void RvoidPintCaller_callsTwice() {
  RvoidPint_3(13);
  RvoidPint_3(42);
}

// NOTE, we can't handle return values, yet
//static int square(int a) { return a*a; }
static void square(int a, int& result) { result = a*a; }

void RvoidPintCaller_lvalue(int a) {
    int res;
    square(a, res);
    // NOTE, we can't handle return values, yet
    //RvoidPint_4(square(a));
    RvoidPint_4(res);
}
