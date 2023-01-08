#pragma once

/**
 * \brief 그래픽스 엔진에서 많이 사용하는 헤더들을 정리해 둔 유틸 헤더
 *
 * => 이후에 미리 컴파일된 헤더나 framework.h에 이전가능
 * 2022.06.14(화) Dev.BlackFoot
 */
//#define _XM_NO_INTRINSICS_
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#pragma warning(disable : 4251)

 // 각종 유틸 매크로를 작성해둔 헤더 파일 목록.
#include <Windows.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include "SimpleMath.h"

// 라이브러리 추가
#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3dcompiler")

// 각종 helper 필요할 때마다 추가, Math, Light, or Vertex 구조체(클래스로 변경 가능)
#include "Util/LightHelper.h"
#include "Util/VertexStruct.h"
#include "Util/MathHelper.h"

// Effect, Bulider Manager 등 스태틱으로 사용하는 녀석들..
	// 순환 참조 일어난다. 사용하지 말자..
/*
#include "InputLayout.h"
#include "Effects.h"
#include "BuilderManger.h"
*/

#include <filesystem>
namespace fs = std::filesystem;
using namespace Microsoft::WRL;
using namespace DirectX;

// 스트링, 벡터 등..
// 필요한 헤더 파일 include
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>

// boost
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/archive/binary_iarchive.hpp> 
#include <boost/archive/binary_oarchive.hpp> 
#include <boost/asio/streambuf.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/device/file.hpp>

// Recast Navigation
#include "Recast/Include/Recast.h"
#include "Recast/Include/RecastAlloc.h"
#include "Recast/Include/RecastAssert.h"
#include "DetourTileCache/Include/DetourTileCache.h"
#include "DetourTileCache/Include/DetourTileCacheBuilder.h"
#include "Detour/Include/DetourNavMesh.h"
#include "Detour/Include/DetourStatus.h"

// FBXBinaryLayout
//#include "FBXBinaryLayout.h"

// COM 포인터 Release를 위한 매크로
#define ReleaseCOM(x) { if(x){ x->Release();x = 0; } }

// Delete 매크로.
#define SafeDelete(x) { delete x; x = 0; }
#define SafeReset(x) { if(x){ x.reset(); x = 0; } }


// HRESULT용 매크로.
// Debug 모드 일때
#if defined(DEBUG) || defined(_DEBUG)
#ifndef HR
#define HR(x)											\
{														\
	HRESULT hr = (x);									\
	if(FAILED(hr))										\
	{													\
		LPWSTR output;									\
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |		\
			FORMAT_MESSAGE_IGNORE_INSERTS |				\
			FORMAT_MESSAGE_ALLOCATE_BUFFER,				\
			NULL,										\
			hr,											\
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	\
			(LPTSTR)&output,							\
			0,											\
			NULL);										\
		MessageBox(NULL, output, L"Error", MB_OK);		\
	}													\
}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif

// effect 사용을 위한 lib 추가
/*#ifdef _DEBUG
#pragma comment( lib, "../Lib/Effects11d.lib" )
#else
#pragma comment( lib, "../Lib/Effects11.lib" )
#endif*/

// DirectXTex => 텍스쳐 라이브러리 추가
#include "DirectXTex.h"
#include "SpriteFont.h"
#include "SpriteBatch.h"

// dxtk 사용을 위한 lib 추가.
#if defined(DEBUG) || defined(_DEBUG)
#if defined(_WIN32) && !defined(_WIN64)
#pragma comment(lib, "DirectXTex_x86_Debug.lib")
#pragma comment(lib, "DirectXTK_x86_Debug.lib")
#else
#pragma comment(lib, "DirectXTex_x64_Debug.lib")
#pragma comment(lib, "DirectXTK_x64_Debug.lib")
#endif
#else
#if defined(_WIN32) && !defined(_WIN64)
#pragma comment(lib, "DirectXTex_x86_Release.lib")
#pragma comment(lib, "DirectXTK_x86_Release.lib")
#else
#pragma comment(lib, "DirectXTex_x64_Release.lib")
#pragma comment(lib, "DirectXTK_x64_Release.lib")
#endif
#endif

/// Dll 매크로
#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif


// ATL 관련 헤더 파일
#include <atlbase.h>
#include <atlstr.h>
#include <atlconv.h>

#pragma comment(lib, "atls.lib")