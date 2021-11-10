module;
#include "..\inc\unity.h"
export module Applicatoin;
import utility;
import Adapters;

using namespace Microsoft::WRL;
// global const variable
constexpr bool _debugFlag = true;  // Enable/Disable directx12 debug layers

export 
class Application {
 public:
 public:
  void init();

 private:
  void enableDebugLayers(bool flag);
  void createHardWare();
 private:
  ComPtr<ID3D12Debug> m_debugController;
  ComPtr<IDXGIFactory4> m_factory;
  ComPtr<IDXGIAdapter> m_adapter;
  ComPtr<IDXGIDevice> m_device;
};

module : private;

void Application::init() {
    // 启用调试层
  if constexpr (_debugFlag) {
    enableDebugLayers(_debugFlag);
  }
  // devices
  // TODO：---
  createHardWare();
}

void Application::enableDebugLayers(bool flag) {
  if (flag &&
      SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_debugController)))) {
    m_debugController->EnableDebugLayer();
  }
}

void Application::createHardWare() {
  // 创建factory
  ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_factory)),
                "create factory failed");
  // TODO：default adapters
  // 创建硬件设备 如果创建失败则回滚到warp设备
  if (HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0,
                                     IID_PPV_ARGS(&m_device));
      FAILED(hr)) {
    ThrowIfFailed(m_factory->EnumWarpAdapter(IID_PPV_ARGS(&m_adapter)), "Enum warp adapters failed!");
    ThrowIfFailed(D3D12CreateDevice(m_adapter.Get(), D3D_FEATURE_LEVEL_11_0,
                                    IID_PPV_ARGS(&m_device)),
                  "Create device failed from wrap adapters");
    
  }
}