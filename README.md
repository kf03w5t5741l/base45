# libbase45

A simple Base45 library in C.

For details on the Base45 specification, see here: https://datatracker.ietf.org/doc/draft-faltstrom-base45/.

## Usage

```
#include <string.h>

#include "base45.h"

int main()
{
    char input[] = "abcd123";

    char* encoded = NULL;
    Base45_encode(&encoded, input, strlen(input));


    char* decoded = NULL;
    size_t written = 0;
    Base45_decode(&decoded, encoded, &written);

    int match = memcmp(input, decoded, strlen(input));

    free(encoded);
    free(decoded);

    return match;
}
```

## Build instructions
Requires CMake.

    $ git clone https://github.com/kf03w5t5741l/base45.git
    $ cd base45
    $ mkdir build
    $ cd build
    $ cmake ..
    $ cmake --build .
