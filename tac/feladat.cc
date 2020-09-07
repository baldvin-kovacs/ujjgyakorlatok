#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

#include <cstdio>
#include <iostream>
#include <random>
#include <string>

char stdout_buffer[4096];

std::string program;
std::default_random_engine rnds(0);
std::uniform_int_distribution<int> rovid_szo_hossza(4, 9);
std::uniform_int_distribution<int> hosszu_szo_hossza(100, 1000);
std::uniform_int_distribution<int> rovid_sor_hossza(0, 5);
std::uniform_int_distribution<int> hosszu_sor_hossza(100, 1000);
std::uniform_int_distribution<int> karakter_kod(static_cast<int>('a'),
                                                static_cast<int>('z'));

char karakter() { return static_cast<char>(karakter_kod(rnds)); }

void kiir_egyszeru_szot() {
  int szo_hossz = rovid_szo_hossza(rnds);
  for (int i = 0; i < szo_hossz; ++i) {
    std::cout << karakter();
  }
}

void kiir_egyszeru_sort() {
  int sor_hossz = rovid_sor_hossza(rnds);
  for (int i = 0; i < sor_hossz; ++i) {
    kiir_egyszeru_szot();
    if (i + 1 < sor_hossz) {
      std::cout << " ";
    }
  }
}

void egyszeru_feladat(bool with_line_ending) {
  for (int i = 0; i < 4; ++i) {
    kiir_egyszeru_sort();
    if (i < 3 || with_line_ending) {
      std::cout << "\n";
    }
  }
  std::cout << std::flush;
}

void rengeteg_sor() {
  for (int i = 0; i < 2e7; i++) {
    kiir_egyszeru_szot();
    std::cout << '\n';
  }
  std::cout << std::flush;
}

void hosszu_sor() {
  for (int i = 0; i < 1e7; i++) {
    std::cout << karakter();
  }
  std::cout << ' ';
  for (int i = 0; i < 1e7; i++) {
    std::cout << karakter();
  }
  kiir_egyszeru_szot();
  std::cout << std::endl;
}

void nagyon_nagy() {
  for (int i = 0; i < 2e8; i++) {
    kiir_egyszeru_szot();
    std::cout << '\n';
  }
  for (int i = 0; i < 2e8; i++) {
    std::cout << karakter();
  }
  std::cout << std::flush;
}

void usage() {
  std::cerr << "Hasznalat: " << program << " {1..7}" << std::endl;
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
  _setmode(fileno(stdout), _O_BINARY);
#endif
  std::setvbuf(stdout, stdout_buffer, _IOFBF, sizeof stdout_buffer);

  program = argv[0];

  if (argc != 2) {
    std::cerr
        << "Meg kell adni parameterben, hogy melyik feladatot generaljam.";
    usage();
    return 1;
  }

  std::string feladat(argv[1]);

  if (feladat == "1") {
    egyszeru_feladat(true);
    return 0;
  }

  if (feladat == "2") {
    egyszeru_feladat(false);
    return 0;
  }

  if (feladat == "3") {
    return 0;
  }

  if (feladat == "4") {
    rengeteg_sor();
    return 0;
  }

  if (feladat == "5") {
    hosszu_sor();
    return 0;
  }

  if (feladat == "6") {
    hosszu_sor();
    rengeteg_sor();
    return 0;
  }

  if (feladat == "7") {
    nagyon_nagy();
    return 0;
  }

  std::cerr << "Ismeretlen feladat: " << feladat << std::endl;
  usage();
  return 1;
}
