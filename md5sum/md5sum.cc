#include <iostream>
#include <string>

#include "md5.h"

int main() {
  std::string s;
  std::cin >> s;
  std::cout << md5(s) << std::endl;
}
