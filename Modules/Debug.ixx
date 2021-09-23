module;
#include "../inc/unity.h"
export module Debug;
namespace Ace {
// �����ά����
// @param os ��׼���
// @param vec XMVECTOR ����
export std::ostream& XM_CALLCONV operator<<(std::ostream& os,
    DirectX::XMVECTOR vec) {
  DirectX::XMFLOAT4 dest;
  DirectX::XMStoreFloat4(&dest, vec);
  os << '(' << dest.x << ", " << dest.y << ", " << dest.z << ")" << std::endl;
  return os;
}
}
