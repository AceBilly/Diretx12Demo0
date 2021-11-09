module;
#include "..\inc\unity.h"
export module Applicatoin;
import utility;
using namespace Microsoft::WRL;
// global const variable
constexpr bool _debugFlag = true;  // Enable/Disable directx12 debug layers

export 
class Application {
 public:
 public:
  void init() { 

	  if constexpr (_debugFlag) {
	     enableDebugLayers(_debugFlag);
	  }
	  // devices

  }

 private:
  void enableDebugLayers(bool flag) {
    if (flag && SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_debugController)))) {
      m_debugController->EnableDebugLayer();
    }
  }
  void createHardWare() {
    HRESULT hardWareRes = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, );
  }
 private:
  ComPtr<ID3D12Debug> m_debugController;
};