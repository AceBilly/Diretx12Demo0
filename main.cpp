#include "inc/unity.h"

import Debug;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Ace;

int main() {
  DirectX::XMVECTOR v = DirectX::XMVectorSet(1., 2., 3., 4.0);
  std::cout << "v: " << v << std::endl;
  return 0;  
}