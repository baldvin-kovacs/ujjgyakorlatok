#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>

class buffered_reader {
 public:
  buffered_reader(std::FILE* fp) : fp(fp) {
    if (std::fseek(fp, 0, SEEK_END)) {
      throw std::runtime_error("could not seek to the end");
    }

    len = std::ftell(fp);
  }

  int file_length() const { return len; }

  char get_at(int n) {
    int remainder = n % sizeof buf;
    int slice = n - remainder;
    if (pos != slice) {
      load_slice(slice);
    }
    return buf[remainder];
  }

 private:
  std::FILE* fp;
  char buf[16384];
  int pos = -1;
  int len;

  void load_slice(int slice) {
    if (std::fseek(fp, slice, SEEK_SET)) {
      throw std::runtime_error("Could not seek.");
    }
    int num_read = len - slice;
    if (num_read > sizeof buf) {
      num_read = sizeof buf;
    }
    std::fread(buf, 1, num_read, fp);
    if (std::ferror(fp)) {
      throw std::runtime_error("Error reading the file");
    }
    if (std::feof(fp)) {
      throw std::runtime_error("Unexpected end of file.");
    }
    pos = slice;
  }
};

char stdout_buffer[4096];
char file_buffer[4096];

void print_line(buffered_reader& br, int from, int n) {
  for (int i = 0; i < n; ++i) {
    std::cout << br.get_at(from + i);
  }
  std::cout << "\n";
}

int main(int argc, char* argv[]) {
  try {
    if (argc != 2) {
      throw std::runtime_error("Must be called with 1 args.");
    }

    std::FILE* fp = std::fopen(argv[1], "rb");
    if (!fp) {
      std::perror("File opening failed");
      return EXIT_FAILURE;
    }

#ifdef _WIN32
    _setmode(fileno(stdout), _O_BINARY);
#endif
    std::setvbuf(stdout, stdout_buffer, _IOFBF, sizeof stdout_buffer);
    std::setvbuf(fp, file_buffer, _IOFBF, sizeof file_buffer);

    buffered_reader br(fp);

    if (br.file_length() == 0) {
      return EXIT_SUCCESS;
    }

    int cursor = br.file_length() - 1;
    char last_char = br.get_at(cursor);
    if (last_char == '\n') {
      --cursor;
    }

    int n = 0;
    while (true) {
      if (cursor == -1) {
        print_line(br, 0, n);
        break;
      }

      char c = br.get_at(cursor);
      --cursor;

      if (c != '\n') {
        ++n;
        continue;
      }

      print_line(br, cursor + 2, n);
      n = 0;
    }
  } catch (std::exception e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
