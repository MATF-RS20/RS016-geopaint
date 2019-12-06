#define CATCH_CONFIG_RUNNER

#include "catch.hpp"

// Catch ne uspeva da napravi svoj
// main kada ga prevodi MinGW, tako da
// je prilozen korisnicki definisan
int main(int argc, char* argv[])
{
    return Catch::Session().run(argc, argv);
}
