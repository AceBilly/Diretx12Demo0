module;
#include "..\inc\unity.h"
#include <memory>
export module Applicatoin;
import utility;
import Adapters;
import Windows;

using namespace Microsoft::WRL;
// global const variable
constexpr bool _debugFlag = true;  // Enable/Disable directx12 debug layers

export 
class Application {
 public:
  explicit Application(UINT width = 700, UINT height = 700);
 public:
  void init();

 private:
  void enableDebugLayers(bool flag);
  void createHardWare(const UINT adaptersListIndex);
  void createCommandQueue();
  void createSwapChain();

 private:
  std::unique_ptr<Windows> mp_windows;
 private:
  UINT m_bufferCount = 2;  // 使用的缓冲区数量，默认采用双缓冲
  UINT m_width;  // 缓冲区宽高
  UINT m_height;
 private:
  ComPtr<ID3D12Debug> m_debugController;
  ComPtr<IDXGIFactory4> m_factory;
  ComPtr<IDXGIAdapter> m_adapter;
  ComPtr<ID3D12Device > m_device;

  ComPtr<ID3D12CommandQueue> m_commandQueue;
  ComPtr<IDXGISwapChain> m_swapChain;
};

module : private;

void Application::createSwapChain() { 
    // TODO: 未完成
    DXGI_SWAP_CHAIN_DESC sc_desc {
    .BufferDesc = {.Width = m_width,
                   .Height = m_height,
                   .Format = DXGI_FORMAT_R8G8B8A8_UNORM},
    .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
    .BufferCount = m_bufferCount, 
    .OutputWindow = mp_windows->getMainWindowHandle(),
    .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
    }; 
}

void Application::createCommandQueue() {
  D3D12_COMMAND_QUEUE_DESC cq_desc = {
      .Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
      .Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE
      };
  ThrowIfFailed(
      m_device->CreateCommandQueue(&cq_desc, IID_PPV_ARGS(&m_commandQueue)),
      "Create failed command queue");
}

void Application::init() {
    // 启用调试层
  if constexpr (_debugFlag) {
    enableDebugLayers(_debugFlag);
  }
  // devices
  // TODO：---
  createHardWare(1000);
}

void Application::enableDebugLayers(bool flag) {
  if (flag && 
      SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_debugController)))) {
    m_debugController->EnableDebugLayer();
  }
}
Application::Application(UINT width, UINT height)
    : m_width(width), m_height(height), mp_windows(std::make_unique<Windows>()) {
  // 创建factory
  ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_factory)),
                "create factory failed");
  if (mp_windows == nullptr) {
    throw std::runtime_error("Create Windows object failed");
  }
}
void Application::createHardWare(const UINT adapterIndex) {
  T_Adapters adapters(m_factory.Get());
  // 创建硬件设备 如果创建失败则回滚到warp设备
  if (HRESULT hr = D3D12CreateDevice(adapters.get(adapterIndex), D3D_FEATURE_LEVEL_11_0,
                                     IID_PPV_ARGS(&m_device));
      FAILED(hr)) {
    ThrowIfFailed(m_factory->EnumWarpAdapter(IID_PPV_ARGS(&m_adapter)), "Enum warp adapters failed!");
    ThrowIfFailed(D3D12CreateDevice(m_adapter.Get(), D3D_FEATURE_LEVEL_11_0,
                                    IID_PPV_ARGS(&m_device)),
                  "Create device failed from wrap adapters");
    
  }
}