module;

#include <windows.h>

export module Windows;


export class Windows {
 public:
  HWND getMainWindowHandle() const;
 private:
  HWND m_windowHandle;
};

module : private;

HWND Windows::getMainWindowHandle() const { return m_windowHandle; }