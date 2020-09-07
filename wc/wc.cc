#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#include <iostream>
#include <cctype>
#include <cstdio>

int main() {
#ifdef _WIN32
    _setmode(fileno(stdin), _O_BINARY);
#endif

    int nl = 0, nw = 0, nc = 0;
    char c;
    bool in_word = false;
    bool in_line = false;
    while (std::cin.get(c)) {
        nc++;
        if (c == '\n') {
            nl++;
            in_word = false;
            in_line = false;
            continue;
        }
        in_line = true;
        if (std::isspace(c)) {
            if (in_word) {
                in_word = false;
            }
            continue;
        }
        if (!in_word) {
            nw++;
            in_word = true;
        }
    }
    if (in_line) {
        nl++;
    }
    std::cout << nl << " " << nw << " " << nc << std::endl;
}
