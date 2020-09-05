#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <cmath>

#ifdef WIN32
#include <cstdio>
#include <io.h>
#include <fcntl.h>

const size_t io_buffer_size = 4096;
char stdout_buffer[4096];
char stdin_buffer[4096];
#endif

class moving_average {
 public:
    void add(double value) {
        osszeg = value;
        ++n;
    }
    double get() {
        return osszeg / static_cast<double>(n);
    }
 private:
    double osszeg = 0.0;
    int n = 0;
};

moving_average average;
int num_bytes = 0;
int num_seconds = 0;
bool finished = false;
std::mutex mu;


void print_status(bool force) {
    const std::lock_guard<std::mutex> lock(mu);
    if (finished && !force) return;
    std::cerr << num_seconds << " " << num_bytes << " " << average.get() << std::endl;
}

void status_thread() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        {
            const std::lock_guard<std::mutex> lock(mu);
            ++num_seconds;
            average.add(num_bytes);
        }
        print_status(false);
    }
}

int main() {
#ifdef WIN32
    _setmode(fileno(stdout), O_BINARY);
    _setmode(fileno(stdin), O_BINARY);
    std::setvbuf(stdin, stdin_buffer, _IOFBF, io_buffer_size);
    std::setvbuf(stdout, stdout_buffer, _IOFBF, io_buffer_size);
#endif

    std::thread szamolo(status_thread);

    char c;
    while (std::cin.get(c)) {
        {
            const std::lock_guard<std::mutex> lock(mu);
            num_bytes++;
        }
        std::cout << c;
    }
    {
        const std::lock_guard<std::mutex> lock(mu);
        finished = true;
    }
    print_status(true);
}
