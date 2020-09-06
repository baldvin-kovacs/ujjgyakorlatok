#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include <Wincrypt.h>

#include "md5.h"

#include <exception>
#include <sstream>
#include <string>
#include <iomanip>

std::runtime_error win32_error(std::string func_name) {
  DWORD dwStatus = GetLastError();
  std::ostringstream err;
  err << func_name << " failed: %d" << dwStatus;
  return std::runtime_error(err.str());
}

class handles {
 public:
  HCRYPTPROV hProv = 0;
  HCRYPTHASH hHash = 0;
  ~handles() {
    if (hProv != 0) {
      CryptReleaseContext(hProv, 0);
    }
    if (hHash != 0) {
      CryptDestroyHash(hHash);
    }
  }
};

std::string md5(std::string s) {
  handles hs;

  // Get handle to the crypto provider
  if (!CryptAcquireContext(&hs.hProv, NULL, NULL, PROV_RSA_FULL,
                           CRYPT_VERIFYCONTEXT)) {
    throw win32_error("CryptAcquireContext");
  }

  if (!CryptCreateHash(hs.hProv, CALG_MD5, 0, 0, &hs.hHash)) {
    throw win32_error("CryptCreateHash");
  }

  assert(sizeof std::string::value_type == 1);
  if (!CryptHashData(hs.hHash, reinterpret_cast<const BYTE*>(s.c_str()), s.size(), 0)) {
    throw win32_error("CryptHashData");
  }

  BYTE hash[16];
  DWORD hash_len = sizeof hash;
  if (!CryptGetHashParam(hs.hHash, HP_HASHVAL, hash, &hash_len, 0)) {
    throw win32_error("CryptGetHashParam");
  }

  std::ostringstream res;
  for (DWORD i = 0; i < hash_len; i++) {
    res << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
  }
  return res.str();
}