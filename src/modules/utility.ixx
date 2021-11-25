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
namespace Ace {

    export struct AceCpuDescriptorHandle : public D3D12_CPU_DESCRIPTOR_HANDLE {
  AceCpuDescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE _handle)
      : D3D12_CPU_DESCRIPTOR_HANDLE(_handle) {}
      AceCpuDescriptorHandle& offset(INT offset, UINT descriptorSize);
    };

    export struct AceHeapProperties : public D3D12_HEAP_PROPERTIES{
      explicit AceHeapProperties(D3D12_HEAP_TYPE _heapType) {
        Type = _heapType;
        CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        CreationNodeMask = 1;
        VisibleNodeMask = 1;
      }
    };

    export struct AceResourceBarrier : public D3D12_RESOURCE_BARRIER {
     static inline AceResourceBarrier Transition(
          ID3D12Resource* pResource, D3D12_RESOURCE_STATES stateBefore,
          D3D12_RESOURCE_STATES stateAfter,
          UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
          D3D12_RESOURCE_BARRIER_FLAGS flags =
              D3D12_RESOURCE_BARRIER_FLAG_NONE) noexcept;
    };
    }  // namespace Ace
module : private;
Ace::AceCpuDescriptorHandle& Ace::AceCpuDescriptorHandle::offset(INT offset, UINT descriptorSize) {
  ptr = SIZE_T(INT64(ptr) + INT64(offset) * INT64(descriptorSize));
  return *this;
}

Ace::AceResourceBarrier Ace::AceResourceBarrier::Transition(
    ID3D12Resource* pResource, D3D12_RESOURCE_STATES stateBefore,
    D3D12_RESOURCE_STATES stateAfter,
    UINT subresource,
    D3D12_RESOURCE_BARRIER_FLAGS flags) noexcept  {

    AceResourceBarrier result = {};
    D3D12_RESOURCE_BARRIER& barrier = result;
    result.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    result.Flags = flags;
    barrier.Transition.pResource = pResource;
    barrier.Transition.StateBefore = stateBefore;
    barrier.Transition.StateAfter = stateAfter;
    barrier.Transition.Subresource = subresource;
    return result;
  }