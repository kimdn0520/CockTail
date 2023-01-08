#pragma once

// 엔진 사용을 위한 lib 추가.
#if defined(DEBUG) || defined(_DEBUG)
#if defined(_WIN32) && !defined(_WIN64)
#pragma comment(lib, "FootGraphicsEngine_Debug_x86.lib")
#else
#pragma comment(lib, "FootGraphicsEngine_Debug_x64.lib")
#endif
#else
#if defined(_WIN32) && !defined(_WIN64)
#pragma comment(lib, "FootGraphicsEngine_Release_x86.lib")
#else
#pragma comment(lib, "FootGraphicsEngine_Release_x64.lib")
#endif
#endif

// dxtk 사용을 위한 lib 추가.
#if defined(DEBUG) || defined(_DEBUG)
#if defined(_WIN32) && !defined(_WIN64)
#pragma comment(lib, "DirectXTK_x86_Debug.lib")
#else
#pragma comment(lib, "DirectXTK_x64_Debug.lib")
#endif
#else
#if defined(_WIN32) && !defined(_WIN64)
#pragma comment(lib, "DirectXTK_x86_Release.lib")
#else
#pragma comment(lib, "DirectXTK_x64_Release.lib")
#endif
#endif

// Recast
#pragma comment(lib, "Recast.lib")
#pragma comment(lib, "DetourTileCache.lib")
#pragma comment(lib, "Detour.lib")