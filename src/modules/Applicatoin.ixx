﻿module;
#include "..\inc\unity.h"
#include <memory>
#include <vector>
export module Applicatoin;
import utility;
import Adapters;
import Windows;

using namespace Microsoft::WRL;
// global const variable
constexpr bool _debugFlag = true;  // Enable/Disable directx12 debug layers
namespace Ace {
export class Application {
 public:
  explicit Application(UINT width = 700, UINT height = 700);

 public:
  void init();

 private:
  void enableDebugLayers(bool flag);
  void createHardWare(const UINT adaptersListIndex);
  void createCommandQueue();
  void createSwapChain();
  void createDescriptorHeaps();
  void createRTV();

  void currentBackBufferView();

 private:
  std::unique_ptr<Win32Windows> mp_windows;  // 指向窗口对象。
 private:
  UINT m_bufferCount = 2;  // 默认采用双缓冲
  UINT m_width;            // 缓冲区宽高
  UINT m_height;
  UINT m_currentBackBuffer = 0;  // 当前后台缓冲区。
 private:
  // 基础对象
  ComPtr<ID3D12Debug> m_debugController;
  ComPtr<IDXGIFactory4> m_factory;
  ComPtr<IDXGIAdapter> m_adapter;
  ComPtr<ID3D12Device> m_device;
  // 命令队列
  ComPtr<ID3D12CommandQueue> m_commandQueue;
  ComPtr<ID3D12CommandAllocator> m_commandAllocator;
  ComPtr<ID3D12GraphicsCommandList> m_commandList;
  // 交换链
  ComPtr<IDXGISwapChain> m_swapChain;
  // 描述符堆
  ComPtr<ID3D12DescriptorHeap> m_RTVHeap;  // 用于渲染目标
  ComPtr<ID3D12DescriptorHeap> m_DSVHeap;  // 用于深度视图

  std::vector<ComPtr<ID3D12Resource>> m_swapChainBuffer;
};
}  // namespace Ace
module : private;

/// <summary>
/// 这个函数用来创建交换链，其中很多的参数采取的是直接给定一个常量
/// TODO: 引入更多的变量，让其可以仔运行时期更改缓冲区配置，而不用在重新编译。
/// </summary>
void Ace::Application::createSwapChain() { 
    DXGI_SWAP_CHAIN_DESC sc_desc {
    .BufferDesc = {.Width = m_width,
                   .Height = m_height,
                   .RefreshRate = {.Numerator = 60, .Denominator = 1},
                   .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
                   .ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED},
    // TODO: 抗锯齿 暂不做考虑
        .SampleDesc = {.Count = 1,  // 采样率
                       .Quality = 0},
    .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
    .BufferCount = m_bufferCount,
    .OutputWindow = mp_windows->getMainWindowHandle(), 
    .Windowed = true,
    .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
    .Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
    }; 

    ThrowIfFailed(m_factory->CreateSwapChain(m_commandQueue.Get(), &sc_desc,
                                             &m_swapChain),
                  "Create Swap Chain failed");
}
/// <summary>
///  创建命令列表 commandQueue CommandAllocator
/// </summary>
void Ace::Application::createCommandQueue() {
  D3D12_COMMAND_QUEUE_DESC cq_desc = {
      .Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
      .Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE
      };
  ThrowIfFailed(
      m_device->CreateCommandQueue(&cq_desc, IID_PPV_ARGS(&m_commandQueue)),
      "Create failed command queue");
  ThrowIfFailed(
      m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                       IID_PPV_ARGS(m_commandAllocator.GetAddressOf())),
      "Create failed command allocator");

  // TODO: 对于多个GPU 的情况没有考虑
  ThrowIfFailed(
      m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(),
                    nullptr, IID_PPV_ARGS(m_commandList.GetAddressOf())),
                "Create failed command list");
  m_commandList->Close();
}

void Ace::Application::init() {
    // 启用调试层
  if constexpr (_debugFlag) {
    enableDebugLayers(_debugFlag);
  }
  // 初始化缓冲区
  m_swapChainBuffer.reserve(m_bufferCount);
  std::fill(m_swapChainBuffer.begin(), m_swapChainBuffer.end(), nullptr);
  // devices
  // TODO：---
  createHardWare(1000);
}

void Ace::Application::enableDebugLayers(bool flag) {
  if (flag && 
      SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_debugController)))) {
    m_debugController->EnableDebugLayer();
  }
}
Ace::Application::Application(UINT width, UINT height)
    : m_width(width), m_height(height), mp_windows(std::make_unique<Win32Windows>()) {
  // 创建factory
  ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_factory)),
                "create factory failed");
  if (mp_windows == nullptr) {
    throw std::runtime_error("Create Windows object failed");
  }
}
void Ace::Application::createHardWare(const UINT adapterIndex) {
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

void Ace::Application::createDescriptorHeaps() {
  D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{
        .Type   = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
        .NumDescriptors = m_bufferCount,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
        .NodeMask = 0
  };
  ThrowIfFailed(m_device->CreateDescriptorHeap(
                    &rtvHeapDesc, IID_PPV_ARGS(m_RTVHeap.GetAddressOf())),
                "Create failed DescriptorHeap for Render target viewer");
  D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{
      .Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
      .NumDescriptors = 1,
      .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
      .NodeMask = 0};
  ThrowIfFailed(m_device->CreateDescriptorHeap(
                    &dsvHeapDesc, IID_PPV_ARGS(m_DSVHeap.GetAddressOf())),
                "Create failed DescriptorHeap for Depth/Stencil view");
}

void Ace::Application::currentBackBufferView() {
    
}

void Ace::Application::createRTV() {
    
}