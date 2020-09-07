#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <stdexcept>

std::FILE* fp;
char stdout_buffer[4096];
char file_buffer[4096];

void step_back(int n) {
  if (std::fseek(fp, -n, SEEK_CUR)) {
    throw std::runtime_error("Error while stepping back.");
  }
}

char read_char() {
  int c = std::fgetc(fp);
  if (c == EOF) {
    throw std::runtime_error("Unexpected EOF.");
  }
  return static_cast<char>(c);
}

void print_line(int n) {
  for (int i = 0; i < n; ++i) {
    std::cout << read_char();
  }
  std::cout << "\n";
  step_back(n);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    throw std::runtime_error("Must be called with 1 args.");
  }

  fp = std::fopen(argv[1], "rb");
  if (!fp) {
    std::perror("File opening failed");
    return EXIT_FAILURE;
  }

#ifdef _WIN32
  _setmode(fileno(stdout), _O_BINARY);
#endif
  std::setvbuf(stdout, stdout_buffer, _IOFBF, sizeof stdout_buffer);
  std::setvbuf(fp, file_buffer, _IOFBF, sizeof file_buffer);

  if (std::fseek(fp, 0, SEEK_END)) {
    throw std::runtime_error("could not seek to the end");
  }

  if (std::ftell(fp) == 0) {
    return 0;
  }

  step_back(1);

  char last_char = read_char();
  if (last_char == '\n') {
    step_back(1);
  }

  int n = 0;
  while (true) {
    if (std::ftell(fp) == 0) {
      print_line(n);
      break;
    }
    step_back(1);

    char c = read_char();

    if (c != '\n') {
      ++n;
      step_back(1);
      continue;
    }

    print_line(n);
    n = 0;
    step_back(1);
  }
}
