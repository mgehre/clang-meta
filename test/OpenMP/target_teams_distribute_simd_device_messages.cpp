// RUN: %clang_cc1 -verify -fopenmp %s

void foo() {
}

bool foobool(int argc) {
  return argc;
}

struct S1; // expected-note {{declared here}}

int main(int argc, char **argv) {
  int i;
#pragma omp target teams distribute simd device // expected-error {{expected '(' after 'device'}}
  for (i = 0; i < argc; ++i) foo();
#pragma omp target teams distribute simd device ( // expected-error {{expected expression}} expected-error {{expected ')'}} expected-note {{to match this '('}}
  for (i = 0; i < argc; ++i) foo();
#pragma omp target teams distribute simd device () // expected-error {{expected expression}}
  for (i = 0; i < argc; ++i) foo();
#pragma omp target teams distribute simd device (argc // expected-error {{expected ')'}} expected-note {{to match this '('}}
  for (i = 0; i < argc; ++i) foo();
#pragma omp target teams distribute simd device (argc)) // expected-warning {{extra tokens at the end of '#pragma omp target teams distribute simd' are ignored}}
  for (i = 0; i < argc; ++i) foo();
#pragma omp target teams distribute simd device (argc > 0 ? argv[1] : argv[2]) // expected-error {{expression must have integral or unscoped enumeration type, not 'char *'}}
  for (i = 0; i < argc; ++i) foo();
#pragma omp target teams distribute simd device (argc + argc)
  for (i = 0; i < argc; ++i) foo();
#pragma omp target teams distribute simd device (argc), device (argc+1) // expected-error {{directive '#pragma omp target teams distribute simd' cannot contain more than one 'device' clause}}
  for (i = 0; i < argc; ++i) foo();
#pragma omp target teams distribute simd device (S1) // expected-error {{'S1' does not refer to a value}}
  for (i = 0; i < argc; ++i) foo();
#pragma omp target teams distribute simd device (-2) // expected-error {{argument to 'device' clause must be a non-negative integer value}}
  for (i = 0; i < argc; ++i) foo();
#pragma omp target teams distribute simd device (-10u)
  for (i = 0; i < argc; ++i) foo();
#pragma omp target teams distribute simd device (3.14) // expected-error {{expression must have integral or unscoped enumeration type, not 'double'}}
  for (i = 0; i < argc; ++i) foo();

  return 0;
}
