#pragma once

#ifdef __ANDROID__
#include <SDL_main.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#include MAY_ATTACH_ALLOCATOR_HEADER
#endif

#ifdef _WIN32
extern "C"
{
#ifdef _MSC_VER
__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x00000001;
#else
__attribute__((dllexport)) DWORD NvOptimusEnablement = 0x00000001;
__attribute__((dllexport)) int AmdPowerXpressRequestHighPerformance = 0x00000001;
#endif
}
#ifdef _MSC_VER
# pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0'  processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#if defined(_WIN32) && !defined(_DEBUG) && !defined(MAY_FORCE_CONSOLE)
#define MAY_MAIN int WINAPI wWinMain( \
        _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow)
#else
#define MAY_MAIN int main(int argc, char* argv[])
#endif
