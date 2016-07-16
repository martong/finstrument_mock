//int square(int a) { return a*a; }

void RvoidPint(int);
void RvoidPint_2(int);
void RvoidPint_3(int);

void RvoidPintCaller() {
  RvoidPint(13);
  RvoidPint_2(15);
  //RvoidPint(square(3));
}

void RvoidPintCaller_callsTwice() {
  RvoidPint_3(13);
  RvoidPint_3(42);
}
