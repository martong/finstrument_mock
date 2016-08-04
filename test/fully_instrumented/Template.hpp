template <typename T>
struct TemplateS {

  int foo(int p) { return bar(p); }
  int bar(int p) { return p; }

  template <typename U>
  U fooT(U p) { return barT(p); }
  template <typename U>
  U barT(U p) { return p; }

};
