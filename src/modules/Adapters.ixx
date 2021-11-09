module;
#include <vector>
#include <dxgi.h>
#include <string>
export module Adapters;

export class Adapters {
 public:
	 explicit Adapters(IDXGIFactory* factory) { 
		 IDXGIAdapter* adapter = nullptr;
           while (factory->EnumAdapters(m_adaptersCount, &adapter) != DXGI_ERROR_NOT_FOUND) {
                   m_adapters.push_back(adapter);
				   ++m_adaptersCount;
		   }
  }

	 std::pair<IDXGIAdapter*, std::wstring> operator[](const UINT index) { 
				 DXGI_ADAPTER_DESC desc;
				 m_adapters[index]->GetDesc(&desc);
                 return std::make_pair(m_adapters[index], desc.Description);
	 }
         void log() {
           for (UINT i = 0; i < m_adapters.size(); ++i) {

           }
	}
 private:
  std::vector<IDXGIAdapter*> m_adapters;
  UINT m_adaptersCount = 0;
};