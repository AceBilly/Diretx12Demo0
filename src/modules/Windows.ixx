module;

#include <windows.h>

export module Windows;


export class Win32Windows {
 public:
  HWND getMainWindowHandle() const;
 private:
  HWND m_windowHandle;
};

module : private;

HWND Win32Windows::getMainWindowHandle() const { return m_windowHandle; }