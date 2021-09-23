module;
#include "../inc/unity.h"
export module Debug;
namespace Ace {
// 输出四维向量
// @param os 标准输出
// @param vec XMVECTOR 对象
export std::ostream& XM_CALLCONV operator<<(std::ostream& os,
    DirectX::XMVECTOR vec) {
  DirectX::XMFLOAT4 dest;
  DirectX::XMStoreFloat4(&dest, vec);
  os << '(' << dest.x << ", " << dest.y << ", " << dest.z << ")" << std::endl;
  return os;
}
}
