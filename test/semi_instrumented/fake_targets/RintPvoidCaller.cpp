int RintPvoid();
void RintPvoid_2();

int RintPvoidCaller() {
  RintPvoid_2();
  return RintPvoid();
}
