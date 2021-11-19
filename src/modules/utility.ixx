module;

#include "..\inc\unity.h"
#include <exception>

export module utility;


using namespace Microsoft::WRL;

export void ThrowIfFailed(HRESULT res, std::string what) {
  if (FAILED(res)) {
    throw std::runtime_error(what.c_str());
  }
}