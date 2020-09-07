#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <cmath>
#include <cstdio>

using std::chrono::high_resolution_clock;

char stdout_buffer[4096];

int num_bytes = 0;
int last_printed_num_bytes = 0;
high_resolution_clock::time_point start_time;
high_resolution_clock::time_point last_status_time;


void print_status(bool force) {
    high_resolution_clock::time_point now = high_resolution_clock::now();
    if (!force) {
        if (num_bytes < last_printed_num_bytes + sizeof stdout_buffer) {
            return;
        }
        if (now - last_status_time < std::chrono::seconds(1)) {
            return;
        }
    }
    last_status_time = now;
    high_resolution_clock::duration elapsed_time = now - start_time;
    double elapsed_seconds = double(std::chrono::nanoseconds(elapsed_time).count()) / double(1e9);
    double avg = double(num_bytes) / elapsed_seconds;
    std::cerr << elapsed_seconds << " " << num_bytes << " " << avg << std::endl;
}

int main() {
#ifdef _WIN32
    _setmode(fileno(stdout), _O_BINARY);
    _setmode(fileno(stdin), _O_BINARY);
#endif

    std::setvbuf(stdout, stdout_buffer, _IOFBF, sizeof stdout_buffer);

    std::cin.tie(nullptr);
    std::cout.sync_with_stdio(false);

    char c[sizeof stdout_buffer];
    last_status_time = start_time = high_resolution_clock::now();

    while (std::cin.read(c, sizeof c)) {
        std::streamsize count = std::cin.gcount();
        num_bytes += count;
        print_status(false);
        if (!std::cout.write(c, count)) break;
    }
    print_status(true);
}
