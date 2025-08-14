#include "win32_window.h"

#include <dwmapi.h>
#include <flutter_windows.h>

#include "resource.h"

namespace {

/// Window attribute that enables dark mode window decorations.
///
/// Redefined in case the developer's machine has a Windows SDK older than
/// version 10.0.22000.0.
/// See: https://docs.microsoft.com/windows/win32/api/dwmapi/ne-dwmapi-dwmwindowattribute
#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

constexpr const wchar_t kWindowClassName[] = L"FLUTTER_RUNNER_WIN32_WINDOW";

/// Registry key for app theme preference.
///
/// A value of 0 indicates apps should use dark mode. A non-zero or missing
/// value indicates apps should use light mode.
constexpr const wchar_t kGetPreferredBrightnessRegKey[] =
  L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
constexpr const wchar_t kGetPreferredBrightnessRegValue[] = L"AppsUseLightTheme";

// The number of Win32Window objects that currently exist.
static int g_active_window_count = 0;

using EnableNonClientDpiScaling = BOOL __stdcall(HWND hwnd);

// Scale helper to convert logical scaler values to physical using passed in
// scale factor
int Scale(int source, double scale_factor) {
  return static_cast<int>(source * scale_factor);
}

// Dynamically loads the |EnableNonClientDpiScaling| from the User32 module.
// This API is only needed for PerMonitor V1 awareness mode.
void EnableFullDpiSupportIfAvailable(HWND hwnd) {
  HMODULE user32_module = LoadLibraryA("User32.dll");
  if (!user32_module) {
    return;
  }
  auto enable_non_client_dpi_scaling =
      reinterpret_cast<EnableNonClientDpiScaling*>(
          GetProcAddress(user32_module, "EnableNonClientDpiScaling"));
  if (enable_non_client_dpi_scaling != nullptr) {
    enable_non_client_dpi_scaling(hwnd);
  }
  FreeLibrary(user32_module);
}

}  // namespace

// Manages the Win32Window's window class registration.
class WindowClassRegistrar {
 public:
  ~WindowClassRegistrar() = default;

  // Returns the singleton registar instance.
  static WindowClassRegistrar* GetInstance() {
    if (!instance_) {
      instance_ = new WindowClassRegistrar();
    }
    return instance_;
  }

  // Returns the name of the window class, registering the class if it hasn't
  // previously been registered.
  const wchar_t* GetWindowClass();

  // Unregisters the window class. Should only be called if there are no
  // instances of the window.
  void UnregisterWindowClass();

 private:
  WindowClassRegistrar() = default;

  static WindowClassRegistrar* instance_;

  bool class_registered_ = false;
};

WindowClassRegistrar* WindowClassRegistrar::instance_ = nullptr;

const wchar_t* WindowClassRegistrar::GetWindowClass() {
  if (!class_registered_) {
    WNDCLASS window_class{};
    window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
    window_class.lpszClassName = kWindowClassName;
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.hInstance = GetModuleHandle(nullptr);
    window_class.hIcon =
        LoadIcon(window_class.hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
    window_class.hbrBackground = 0;
    window_class.lpszMenuName = nullptr;
    window_class.lpfnWndProc = Win32Window::WndProc;
    RegisterClass(&window_class);
    class_registered_ = true;
  }
  return kWindowClassName;
}

void WindowClassRegistrar::UnregisterWindowClass() {
  UnregisterClass(kWindowClassName, nullptr);
  class_registered_ = false;
}

Win32Window::Win32Window() {
  ++g_active_window_count;
}

Win32Window::~Win32Window() {
  --g_active_window_count;
  Destroy();
}

bool Win32Window::Create(const std::wstring& title,
                         const Point& origin,
                         const Size& size) {
  Destroy();

  const wchar_t* window_class =
      WindowClassRegistrar::GetInstance()->GetWindowClass();

  const POINT target_point = {static_cast<LONG>(origin.x),
        ������������������������������������������������������������������������������������������������������������Ҧs��r ��r ��r ��r ��r ��r ��r �ŌI�������������������������������������������������۸���z-��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r �ҧs�������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������ҧs��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r �ҧs���������������������������������������������������������������������������������������������������������������������������������������������������������������������������������Ҧs��r ��r ��r ��r ��r ��r ��r �ŌI��������������������������������������������������������������r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r �ҧs�������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������ҧs��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r �ҧs�������������������������������������������������������������������������������������������������������������������������������������������������������������������������Ҧs��r ��r ��r ��r ��r ��r ��r �ŌI�������������������������������������������������������������ŌI��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r �ҧs�������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������ҧs��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r �ҧs�����������������������������������������������������������������������������������������������������������������������������������������������������������������Ҧs��r ��r ��r ��r ��r ��r ��r �ŌI�������������������������������������������������������������ŌI��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r �ҧs�������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������ҧs��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r �ҧs���������������������������������������������������������������������������������������������������������������������������������������������������������Ҧs��r ��r ��r ��r ��r ��r ��r �ŌI�������������������������������������������������������������ŌI��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r �ҧs�������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������ҧs��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��r ��