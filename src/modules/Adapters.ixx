module;
#include <dxgi.h>

#include <format>
#include <iostream>
#include <string>
#include <vector>
export module Adapters;

export class T_Adapters {
 public:
  /// <summary>
  /// 构造Adapters 对象 便利显卡设备
  /// </summary>
  /// <param name="factory">需要从外部传入IDXGIFactory用于枚举底层设备</param>
  explicit T_Adapters(IDXGIFactory* factory);
  std::pair<IDXGIAdapter*, std::wstring> operator[](const UINT index) const;
  /// <summary>
  /// 打印当前设备中含有的显卡信息
  /// </summary>
  /// <returns></returns>
  const T_Adapters& operator*() const;
  void log() const;
  IDXGIAdapter* get(const UINT index) const;
 private:
  std::vector<IDXGIAdapter*> m_adapters;
  UINT m_adaptersCount = 0;
};

module : private;

IDXGIAdapter* T_Adapters::get(const UINT index) const {
  if (index > m_adapters.size()) {
    return nullptr;
  }
  return m_adapters[index];
}

T_Adapters::T_Adapters(IDXGIFactory* factory) {
  IDXGIAdapter* adapter = nullptr;
  while (factory->EnumAdapters(m_adaptersCount, &adapter) !=
         DXGI_ERROR_NOT_FOUND) {
    m_adapters.push_back(adapter);
    ++m_adaptersCount;
  }
}

std::pair<IDXGIAdapter*, std::wstring> T_Adapters::operator[](
    const UINT index) const {
  DXGI_ADAPTER_DESC desc;
  m_adapters[index]->GetDesc(&desc);
  return std::make_pair(m_adapters[index], desc.Description);
}

const T_Adapters& T_Adapters::operator*() const { return *this; }
void T_Adapters::log() const {
  for (UINT i = 0; i < m_adapters.size(); ++i) {
    std::wcout << "Adapters " << std::to_wstring(i)
               << " : Des : " << (*this)[i].second << std::endl;
  }
}