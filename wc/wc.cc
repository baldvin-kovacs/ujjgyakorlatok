#include <iostream>
#include <cctype>

#ifdef WIN32
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

const size_t io_buffer_size = 4096;
char stdin_buffer[4096];
#endif

int main() {
#ifdef WIN32
    _setmode(fileno(stdin), O_BINARY);
    std::setvbuf(stdin, stdin_buffer, _IOFBF, io_buffer_size);
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
