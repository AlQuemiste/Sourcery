// turn this on to remove the contents of include files from the preprocessed result
// eg.: g++ -DDEBUG_ -E macro_test.cpp
#ifndef DEBUG_
#include <iostream>
#endif

// This is an object-like macro
#define    PI         3.14159265358979

// This is a function-like macro.
// Note that we can use previously defined macros
// in other macro definitions (object-like or function-like)
// But watch out, its quite useful if you know what you're doing, but the
// Compiler doesnt know which type to handle, so using inline functions instead
// is quite recommended (But e.g. for Minimum/Maximum functions it is quite useful)
#define    AREA(r)    (PI*(r)*(r))

// literal concatenation of strings
#define GLUE(a,b) a ## b

// If a macro needs to cover multiple lines, however, a backslash can be used at the end of a line to indicate this. This backslash must be the last character in the line, which indicates to the preprocessor that the following line should be concatenated onto the current line, treating them as a single line. This can be used multiple times in a row.

#define TEXT "I \
am \
many \
lines at line __LINE__."

#ifdef SMALL_
  #define SHOW std::cout << "r is not 2 " << z << std::endl;
#else
  #define SHOW std::cout << "R IS NOT 2 " << z << std::endl;
#endif

#include "foo.def"

// Make function factory and use it
#define FUNCTION(name, a) int fun_##name() { return a;}

FUNCTION(abcd, 12)
FUNCTION(fff, 2)
FUNCTION(qqq, 23)

#undef FUNCTION
#define FUNCTION 34
#define OUTPUT(a) std::cout << "output: " #a << '\n'

int main() {

  double pi_macro = PI;
  double area_macro = AREA(4.6);
  int z = 13;

  std::cout << TEXT << std::endl;

  FOO(pi_macro, "HAHA");
  
  OUTPUT(million); // note the lack of quotes
  return 0;
}
